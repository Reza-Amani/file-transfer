/*
 * server.c — Listen, accept, thread-per-connection file serve.
 *
 * Design (ADR-0001 / ADR-0002):
 *   - Bind 0.0.0.0 so other machines on the LAN can connect.
 *   - Each accepted socket is handled on a dedicated thread, then closed.
 *   - One GET per connection; respond with OK + body or ERR; no keep-alive.
 *
 * Clarity over cleverness: a blocking accept loop + CreateThread, not an
 * event-loop state machine.
 */

#include "server.h"

#include "../common/net.h"
#include "../common/path.h"
#include "../common/protocol.h"

#include <process.h> /* _beginthreadex */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

/* Max path we build for serve_root\filename. */
#define FT_MAX_PATH 4096

/* Per-connection state passed to the worker thread (heap-allocated). */
typedef struct {
    SOCKET sock;
    char serve_root[FT_MAX_PATH];
} conn_ctx_t;

static void print_server_usage(void)
{
    fprintf(stderr,
            "Usage: file-transfer server [--port <port>] --dir <serve-root>\n"
            "  --port   Listen port (default %d)\n"
            "  --dir    Directory to serve bare filenames from (required)\n",
            FT_DEFAULT_PORT);
}

/*
 * Parse flags after the "server" subcommand.
 * Returns 0 on success, -1 on bad/missing args.
 */
static int parse_server_args(int argc, char **argv, unsigned short *port_out,
                             const char **dir_out)
{
    int i;
    unsigned short port = FT_DEFAULT_PORT;
    const char *dir = NULL;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--port") == 0) {
            unsigned long p;
            char *end = NULL;
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing value for --port\n");
                return -1;
            }
            p = strtoul(argv[++i], &end, 10);
            if (end == argv[i] || *end != '\0' || p == 0 || p > 65535) {
                fprintf(stderr, "Invalid --port: %s\n", argv[i]);
                return -1;
            }
            port = (unsigned short)p;
        } else if (strcmp(argv[i], "--dir") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing value for --dir\n");
                return -1;
            }
            dir = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_server_usage();
            return -1;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            print_server_usage();
            return -1;
        }
    }

    if (dir == NULL || dir[0] == '\0') {
        fprintf(stderr, "--dir is required\n");
        print_server_usage();
        return -1;
    }

    *port_out = port;
    *dir_out = dir;
    return 0;
}

/* Send ERR <message>\n and return. */
static void send_err(SOCKET sock, const char *message)
{
    char line[FT_MAX_LINE];
    if (protocol_format_err(line, sizeof(line), message) == 0) {
        (void)net_send_all(sock, line, strlen(line));
    }
}

/*
 * Serve one request on sock from serve_root, then return.
 * The caller closes the socket.
 */
static void handle_client(SOCKET sock, const char *serve_root)
{
    char line[FT_MAX_LINE];
    char filename[FT_MAX_FILENAME + 1];
    char fullpath[FT_MAX_PATH];
    FILE *fp = NULL;
    uint64_t size;
    char ok_line[FT_MAX_LINE];
    char chunk[FT_CHUNK_SIZE];

    if (net_recv_line(sock, line, sizeof(line)) != 0) {
        return;
    }

    if (protocol_parse_get(line, filename, sizeof(filename)) != 0) {
        send_err(sock, FT_ERR_INVALID_PATH);
        return;
    }

    //secutity check; serve root only.
    if (!path_is_bare_filename(filename)) {
        send_err(sock, FT_ERR_INVALID_PATH);
        return;
    }

    if (path_join_under_root(serve_root, filename, fullpath, sizeof(fullpath)) != 0) {
        send_err(sock, FT_ERR_INVALID_PATH);
        return;
    }

    fp = fopen(fullpath, "rb");
    if (fp == NULL) {
        send_err(sock, FT_ERR_NOT_FOUND);
        return;
    }

    /* File size via 64-bit seek (supports large files; empty file => 0). */
    if (_fseeki64(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        send_err(sock, FT_ERR_NOT_FOUND);
        return;
    }
    {
        __int64 sz = _ftelli64(fp);
        if (sz < 0) {
            fclose(fp);
            send_err(sock, FT_ERR_NOT_FOUND);
            return;
        }
        size = (uint64_t)sz;
    }
    if (_fseeki64(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        send_err(sock, FT_ERR_NOT_FOUND);
        return;
    }

    if (protocol_format_ok(ok_line, sizeof(ok_line), size) != 0) {
        fclose(fp);
        return;
    }
    if (net_send_all(sock, ok_line, strlen(ok_line)) != 0) {
        fclose(fp);
        return;
    }

    /* Chunked send: never require the whole file in memory. */
    {
        uint64_t remaining = size;
        while (remaining > 0) {
            size_t to_read = FT_CHUNK_SIZE;
            size_t nread;
            if ((uint64_t)to_read > remaining) {
                to_read = (size_t)remaining;
            }
            nread = fread(chunk, 1, to_read, fp);
            if (nread == 0) {
                break; /* short read / error — peer will see incomplete body */
            }
            if (net_send_all(sock, chunk, nread) != 0) {
                break;
            }
            remaining -= (uint64_t)nread;
        }
    }

    fclose(fp);
}

/* Thread entry: owns ctx (frees it) and closes the socket. */
static unsigned __stdcall connection_thread(void *arg)
{
    conn_ctx_t *ctx = (conn_ctx_t *)arg;
    handle_client(ctx->sock, ctx->serve_root);
    closesocket(ctx->sock);
    free(ctx);
    return 0;
}

int run_server(int argc, char **argv)
{
    unsigned short port;
    const char *dir;
    SOCKET listen_sock;
    DWORD attr;

    if (parse_server_args(argc, argv, &port, &dir) != 0) {
        return 1;
    }

    /* Refuse to start if --dir is not an existing directory. */
    attr = GetFileAttributesA(dir);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        fprintf(stderr, "Serve root is not a directory: %s\n", dir);
        return 1;
    }

    if ((size_t)strlen(dir) >= FT_MAX_PATH) {
        fprintf(stderr, "--dir path is too long\n");
        return 1;
    }

    listen_sock = net_listen(port);
    if (listen_sock == INVALID_SOCKET) {
        return 1;
    }

    printf("Listening on 0.0.0.0:%u, serving %s\n", (unsigned)port, dir);
    fflush(stdout);

    for (;;) {
        SOCKET client = accept(listen_sock, NULL, NULL);
        conn_ctx_t *ctx;
        uintptr_t thr;

        if (client == INVALID_SOCKET) {
            fprintf(stderr, "accept() failed: %d\n", WSAGetLastError());
            continue;
        }

        ctx = (conn_ctx_t *)malloc(sizeof(*ctx));
        if (ctx == NULL) {
            fprintf(stderr, "Out of memory for connection\n");
            closesocket(client);
            continue;
        }
        ctx->sock = client;
        memcpy(ctx->serve_root, dir, strlen(dir) + 1);

        /*
         * _beginthreadex so the CRT is initialized for fopen/fread in the
         * worker. Close the thread handle immediately; the thread frees ctx.
         */
        thr = _beginthreadex(NULL, 0, connection_thread, ctx, 0, NULL);
        if (thr == 0) {
            fprintf(stderr, "Failed to start connection thread\n");
            closesocket(client);
            free(ctx);
            continue;
        }
        CloseHandle((HANDLE)thr);
    }

    /* Unreachable in normal use (Ctrl+C ends the process). */
    closesocket(listen_sock);
    return 0;
}
