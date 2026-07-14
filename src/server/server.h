/*
 * server.h — Server mode entry point.
 *
 * Listens on TCP, accepts connections, and serves one file request per
 * connection on its own thread (ADR-0001).
 */

#ifndef FT_SERVER_H
#define FT_SERVER_H

/*
 * Run server mode. argc/argv are the flags after the "server" subcommand.
 * Expected: [--port <n>] --dir <serve-root>
 * Returns process exit code (0 success, non-zero failure).
 */
int run_server(int argc, char **argv);

#endif /* FT_SERVER_H */
