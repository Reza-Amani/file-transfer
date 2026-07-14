/*
 * client.c — Connect, GET one file, write --out, exit.
 *
 * Design (ADR-0001 / ADR-0002):
 *   - Outbound TCP only (typical client firewall).
 *   - --out is required; overwrite if it exists.
 *   - On mid-transfer failure, delete the partial --out so no corrupt file
 *     is left behind.
 */

#include "client.h"

#include "../common/net.h"
#include "../common/path.h"
#include "../common/protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_client_usage(void)
{
    fprintf(stderr,
            "Usage: file-transfer client --host <addr> [--port <port>] "
            "--file <name> --out <path>\n"
            "  --host   Server address (required)\n"
            "  --port   Server port (default %d)\n"
            "  --file   Bare filename to request (required)\n"
            "  --out    Local path to write (required; overwritten if present)\n",
            FT_DEFAULT_PORT);
}

/*
 * Parse flags after the "client" subcommand.
 * Returns 0 on success, -1 on bad/missing args.
 */
static int parse_client_args(int argc, char **argv, const char **host_out,
                             unsigned short *port_out, const char **file_out,
                             const char **out_out)
{
    int i;
    const char *host = NULL;
    unsigned short port = FT_DEFAULT_PORT;
    const char *file = NULL;
    const char *out = NULL;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--host") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing value for --host\n");
                return -1;
            }
            host = argv[++i];
        } else if (strcmp(argv[i], "--port") == 0) {
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
        } else if (strcmp(argv[i], "--file") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing value for --file\n");
                return -1;
            }
            file = argv[++i];
        } else if (strcmp(argv[i], "--out") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Missing value for --out\n");
                return -1;
            }
            out = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_client_usage();
            return -1;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            print_client_usage();
            return -1;
        }
    }

    if (host == NULL || host[0] == '\0') {
        fprintf(stderr, "--host is required\n");
        print_client_usage();
        return -1;
    }
    if (file == NULL || file[0] == '\0') {
        fprintf(stderr, "--file is required\n");
        print_client_usage();
        return -1;
    }
    if (out == NULL || out[0] == '\0') {
        fprintf(stderr, "--out is required\n");
        print_client_usage();
        return -1;
    }

    *host_out = host;
    *port_out = port;
    *file_out = file;
    *out_out = out;
    return 0;
}

/* Close sock, remove partial out_path if non-NULL, return exit_code. */
static int fail_cleanup(SOCKET sock, FILE *fp, const char *out_path, int exit_code)
{
    if (fp != NULL) {
        fclose(fp);
    }
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    if (out_path != NULL) {
        remove(out_path);
    }
    return exit_code;
}

int run_client(int argc, char **argv)
{
    const char *host;
    unsigned short port;
    const char *file;
    const char *out_path;
    SOCKET sock = INVALID_SOCKET;
    char req[FT_MAX_LINE];
    char line[FT_MAX_LINE];
    char err_msg[FT_MAX_LINE];
    uint64_t size = 0;
    FILE *fp = NULL;
    char chunk[FT_CHUNK_SIZE];
    uint64_t remaining;

    if (parse_client_args(argc, argv, &host, &port, &file, &out_path) != 0) {
        return 1;
    }

    /* Local sanity check; server will also reject illegal names. */
    if (!path_is_bare_filename(file)) {
        fprintf(stderr, "Invalid --file (bare filename required): %s\n", file);
        return 1;
    }

    sock = net_connect(host, port);
    if (sock == INVALID_SOCKET) {
        return 1;
    }

    if (protocol_format_get(req, sizeof(req), file) != 0) {
        fprintf(stderr, "Filename too long for protocol line\n");
        return fail_cleanup(sock, NULL, NULL, 1);
    }
    if (net_send_all(sock, req, strlen(req)) != 0) {
        fprintf(stderr, "Failed to send GET request\n");
        return fail_cleanup(sock, NULL, NULL, 1);
    }

    if (net_recv_line(sock, line, sizeof(line)) != 0) {
        fprintf(stderr, "Failed to read server response\n");
        return fail_cleanup(sock, NULL, NULL, 1);
    }

    if (protocol_parse_err(line, err_msg, sizeof(err_msg)) == 0) {
        fprintf(stderr, "Server error: %s\n", err_msg);
        return fail_cleanup(sock, NULL, NULL, 1);
    }

    if (protocol_parse_ok(line, &size) != 0) {
        fprintf(stderr, "Unexpected server response: %s\n", line);
        return fail_cleanup(sock, NULL, NULL, 1);
    }

    printf("Receiving %s (%llu bytes) -> %s\n", file,
           (unsigned long long)size, out_path);
    fflush(stdout);

    fp = fopen(out_path, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open --out for writing: %s\n", out_path);
        return fail_cleanup(sock, NULL, NULL, 1);
    }

    remaining = size;
    while (remaining > 0) {
        size_t to_recv = FT_CHUNK_SIZE;
        size_t nwritten;
        if ((uint64_t)to_recv > remaining) {
            to_recv = (size_t)remaining;
        }
        if (net_recv_exact(sock, chunk, to_recv) != 0) {
            fprintf(stderr, "Connection dropped mid-transfer\n");
            return fail_cleanup(sock, fp, out_path, 1);
        }
        nwritten = fwrite(chunk, 1, to_recv, fp);
        if (nwritten != to_recv) {
            fprintf(stderr, "Failed writing to %s\n", out_path);
            return fail_cleanup(sock, fp, out_path, 1);
        }
        remaining -= (uint64_t)to_recv;
    }

    if (fclose(fp) != 0) {
        fprintf(stderr, "Failed flushing %s\n", out_path);
        fp = NULL;
        return fail_cleanup(sock, fp, out_path, 1);
    }
    fp = NULL;

    closesocket(sock);
    printf("Done.\n");
    return 0;
}
