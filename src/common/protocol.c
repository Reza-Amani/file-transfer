/*
 * protocol.c — Format and parse GET / OK / ERR header lines.
 *
 * Kept free of sockets and filesystem so both modes share one framing layer.
 */

#include "protocol.h"

#include <stdio.h>
#include <string.h>

int protocol_format_get(char *buf, size_t buflen, const char *filename)
{
    int n;

    if (buf == NULL || filename == NULL) {
        return -1;
    }

    n = snprintf(buf, buflen, "GET %s\n", filename);
    if (n < 0 || (size_t)n >= buflen) {
        return -1;
    }
    return 0;
}

int protocol_parse_get(const char *line, char *name_out, size_t name_len)
{
    const char *p;
    size_t len;

    if (line == NULL || name_out == NULL || name_len == 0) {
        return -1;
    }

    if (strncmp(line, "GET ", 4) != 0) {
        return -1;
    }
    p = line + 4;

    /* Trim trailing CR/LF if present. */
    len = strlen(p);
    while (len > 0 && (p[len - 1] == '\n' || p[len - 1] == '\r')) {
        len--;
    }
    if (len == 0 || len >= name_len) {
        return -1;
    }

    /* Reject embedded whitespace (filename must be a single token). */
    if (memchr(p, ' ', len) != NULL || memchr(p, '\t', len) != NULL) {
        return -1;
    }

    memcpy(name_out, p, len);
    name_out[len] = '\0';
    return 0;
}

int protocol_format_ok(char *buf, size_t buflen, uint64_t size)
{
    int n;

    if (buf == NULL) {
        return -1;
    }

    n = snprintf(buf, buflen, "OK %llu\n", (unsigned long long)size);
    if (n < 0 || (size_t)n >= buflen) {
        return -1;
    }
    return 0;
}

int protocol_parse_ok(const char *line, uint64_t *size_out)
{
    unsigned long long size;
    char extra;

    if (line == NULL || size_out == NULL) {
        return -1;
    }

    /* Accept optional trailing newline; reject leftover non-whitespace. */
    if (sscanf(line, "OK %llu %c", &size, &extra) == 1) {
        *size_out = (uint64_t)size;
        return 0;
    }
    if (sscanf(line, "OK %llu\n", &size) == 1) {
        *size_out = (uint64_t)size;
        return 0;
    }
    return -1;
}

int protocol_format_err(char *buf, size_t buflen, const char *message)
{
    int n;

    if (buf == NULL || message == NULL) {
        return -1;
    }

    n = snprintf(buf, buflen, "ERR %s\n", message);
    if (n < 0 || (size_t)n >= buflen) {
        return -1;
    }
    return 0;
}

int protocol_parse_err(const char *line, char *msg_out, size_t msg_len)
{
    const char *p;
    size_t len;

    if (line == NULL || msg_out == NULL || msg_len == 0) {
        return -1;
    }

    if (strncmp(line, "ERR ", 4) != 0) {
        return -1;
    }
    p = line + 4;

    len = strlen(p);
    while (len > 0 && (p[len - 1] == '\n' || p[len - 1] == '\r')) {
        len--;
    }
    if (len == 0 || len >= msg_len) {
        return -1;
    }

    memcpy(msg_out, p, len);
    msg_out[len] = '\0';
    return 0;
}
