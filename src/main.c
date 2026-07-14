/*
 * main.c — Argument parse and mode dispatch.
 *
 * One binary, two modes (ADR-0001):
 *   file-transfer server ...
 *   file-transfer client ...
 *
 * Winsock is initialized here once for the process lifetime.
 */

#include "client/client.h"
#include "common/net.h"
#include "common/protocol.h"
#include "server/server.h"

#include <stdio.h>
#include <string.h>

static void print_usage(const char *prog)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s server [--port <port>] --dir <serve-root>\n"
            "  %s client --host <addr> [--port <port>] --file <name> --out <path>\n"
            "\n"
            "Default port: %d\n",
            prog, prog, FT_DEFAULT_PORT);
}

int main(int argc, char **argv)
{
    int rc;

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        return 0;
    }

    if (net_init() != 0) {
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        rc = run_server(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "client") == 0) {
        rc = run_client(argc - 2, argv + 2);
    } else {
        fprintf(stderr, "Unknown mode: %s\n", argv[1]);
        print_usage(argv[0]);
        rc = 1;
    }

    net_cleanup();
    return rc;
}
