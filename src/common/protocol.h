/*
 * protocol.h — Wire protocol constants and helpers (ADR-0002).
 *
 * Framing is line-oriented text, then an optional raw body:
 *   Client:  GET <bare-filename>\n
 *   Success: OK <byte-size>\n  + exactly <byte-size> bytes
 *   Failure: ERR <message>\n   (no body)
 */

#ifndef FT_PROTOCOL_H
#define FT_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

/* Default listen / connect port (TCP, IPv4). */
#define FT_DEFAULT_PORT 9000

/* Chunk size for disk <-> socket copies (64 KiB). Documented in design.ref.md. */
#define FT_CHUNK_SIZE (64 * 1024)

/* Max bare filename length we accept on the wire. */
#define FT_MAX_FILENAME 255

/* Max length of a protocol header line including the trailing newline. */
#define FT_MAX_LINE 512

/* Canonical error messages from the spec. */
#define FT_ERR_NOT_FOUND "File not found"
#define FT_ERR_INVALID_PATH "Invalid path"

/*
 * Format a client request into buf. Returns 0 on success, -1 if filename
 * is too long for the buffer.
 */
int protocol_format_get(char *buf, size_t buflen, const char *filename);

/*
 * Parse "GET <name>\n" (or without trailing newline already stripped).
 * Copies the filename into name_out (nul-terminated).
 * Returns 0 on success, -1 on malformed input.
 */
int protocol_parse_get(const char *line, char *name_out, size_t name_len);

/*
 * Format "OK <size>\n" into buf. Returns 0 on success, -1 on overflow.
 */
int protocol_format_ok(char *buf, size_t buflen, uint64_t size);

/*
 * Parse "OK <size>" (newline optional / already stripped).
 * Returns 0 on success, -1 on malformed input.
 */
int protocol_parse_ok(const char *line, uint64_t *size_out);

/*
 * Format "ERR <message>\n" into buf. Returns 0 on success, -1 on overflow.
 */
int protocol_format_err(char *buf, size_t buflen, const char *message);

/*
 * Parse "ERR <message>" (newline optional). Copies message into msg_out.
 * Returns 0 on success, -1 on malformed input.
 */
int protocol_parse_err(const char *line, char *msg_out, size_t msg_len);

#endif /* FT_PROTOCOL_H */
