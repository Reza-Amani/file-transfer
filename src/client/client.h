/*
 * client.h — Client mode entry point.
 *
 * One connection per invocation: request a bare filename, write --out, exit.
 */

#ifndef FT_CLIENT_H
#define FT_CLIENT_H

/*
 * Run client mode. argc/argv are the flags after the "client" subcommand.
 * Expected: --host <addr> [--port <n>] --file <name> --out <path>
 * Returns process exit code (0 success, non-zero failure).
 */
int run_client(int argc, char **argv);

#endif /* FT_CLIENT_H */
