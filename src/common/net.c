/*
 * net.c — Winsock init and byte-stream helpers.
 *
 * TCP gives a reliable stream with no message boundaries; send_all / recv_exact
 * and recv_line restore the framing we need for headers and fixed-size bodies.
 */

#include "net.h"

#include <stdio.h>
#include <string.h>

int net_init(void)
{
    WSADATA wsa;
    int rc = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (rc != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", rc);
        return -1;
    }
    return 0;
}

void net_cleanup(void)
{
    WSACleanup();
}

int net_send_all(SOCKET sock, const void *buf, size_t len)
{
    const char *p = (const char *)buf;
    size_t sent = 0;

    while (sent < len) {
        int n = send(sock, p + sent, (int)(len - sent), 0);
        if (n == SOCKET_ERROR || n == 0) {
            return -1;
        }
        sent += (size_t)n;
    }
    return 0;
}

int net_recv_exact(SOCKET sock, void *buf, size_t len)
{
    char *p = (char *)buf;
    size_t got = 0;

    while (got < len) {
        int n = recv(sock, p + got, (int)(len - got), 0);
        if (n == SOCKET_ERROR || n == 0) {
            return -1;
        }
        got += (size_t)n;
    }
    return 0;
}

int net_recv_line(SOCKET sock, char *buf, size_t buflen)
{
    size_t i = 0;

    if (buf == NULL || buflen < 2) {
        return -1;
    }

    for (;;) {
        char c;
        int n = recv(sock, &c, 1, 0);
        if (n == SOCKET_ERROR || n == 0) {
            return -1;
        }
        if (c == '\n') {
            break;
        }
        /* Drop CR so "\r\n" still yields a clean line. */
        if (c == '\r') {
            continue;
        }
        if (i + 1 >= buflen) {
            return -1; /* line too long */
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return 0;
}

SOCKET net_listen(unsigned short port)
{
    SOCKET sock;
    struct sockaddr_in addr;
    BOOL reuse = TRUE;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    /* Allow quick restart after the previous process exits. */
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* 0.0.0.0 — reachable on LAN */
    addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        fprintf(stderr, "bind() failed: %d\n", WSAGetLastError());
        closesocket(sock);
        return INVALID_SOCKET;
    }

    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "listen() failed: %d\n", WSAGetLastError());
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

SOCKET net_connect(const char *host, unsigned short port)
{
    SOCKET sock;
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct addrinfo *rp;
    char port_str[16];
    int rc;

    if (host == NULL) {
        return INVALID_SOCKET;
    }

    snprintf(port_str, sizeof(port_str), "%u", (unsigned)port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    rc = getaddrinfo(host, port_str, &hints, &result);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo(%s): %s\n", host, gai_strerror(rc));
        return INVALID_SOCKET;
    }

    sock = INVALID_SOCKET;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock == INVALID_SOCKET) {
            continue;
        }
        if (connect(sock, rp->ai_addr, (int)rp->ai_addrlen) == 0) {
            break;
        }
        closesocket(sock);
        sock = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "connect to %s:%u failed\n", host, (unsigned)port);
    }
    return sock;
}
