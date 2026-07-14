/*
 * net.h — Winsock lifecycle and reliable send/recv helpers.
 *
 * All socket I/O used by server and client goes through these helpers so
 * partial sends/recvs and line framing stay in one place.
 */

#ifndef FT_NET_H
#define FT_NET_H

#include <stddef.h>
#include <stdint.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

/*
 * Initialize Winsock (WSAStartup). Call once at process start.
 * Returns 0 on success, -1 on failure (message printed to stderr).
 */
int net_init(void);

/* Tear down Winsock (WSACleanup). Safe to call after net_init failure. */
void net_cleanup(void);

/*
 * Send exactly len bytes (loop until done or error).
 * Returns 0 on success, -1 on failure or peer close mid-send.
 */
int net_send_all(SOCKET sock, const void *buf, size_t len);

/*
 * Receive exactly len bytes into buf.
 * Returns 0 on success, -1 on failure or unexpected peer close.
 */
int net_recv_exact(SOCKET sock, void *buf, size_t len);

/*
 * Read one line ending in '\n' into buf (nul-terminated, newline stripped).
 * Returns 0 on success, -1 on error / EOF / line too long for buflen.
 */
int net_recv_line(SOCKET sock, char *buf, size_t buflen);

/*
 * Create a listening TCP IPv4 socket bound to 0.0.0.0:port.
 * Returns INVALID_SOCKET on failure.
 */
SOCKET net_listen(unsigned short port);

/*
 * Connect a TCP IPv4 socket to host:port (host may be dotted IPv4 or name).
 * Returns INVALID_SOCKET on failure.
 */
SOCKET net_connect(const char *host, unsigned short port);

#endif /* FT_NET_H */
