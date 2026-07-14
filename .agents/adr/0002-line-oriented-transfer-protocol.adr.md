# ADR-0002: Line-Oriented Transfer Protocol

## Status

Accepted

## Context

TCP provides a reliable byte stream but no message boundaries. We need a minimal
application framing for one file request per connection. Alternatives considered:
fixed binary headers, and a minimal HTTP/1.1 subset.

## Decision

Use a **line-oriented text header** plus an optional **raw binary body**:

1. Client sends: `GET <filename>\n` where `<filename>` is a bare filename.
2. On success, server sends: `OK <size>\n` followed by exactly `<size>` file
   bytes (binary-safe). `<size>` is a decimal integer byte length.
3. On failure, server sends: `ERR <message>\n` and no body.
4. The connection carries a single request, then closes.
5. Transfer file payload with a fixed-size buffer loop (chunked I/O); do not
   require loading the whole file into memory.

## Consequences

- Protocol helpers belong in `src/common/` and are shared by both modes.
- Reviewers and operators can inspect the header lines with simple tools; the
  body remains opaque bytes.
- Empty files are valid (`OK 0\n` with no body bytes).
- Efficient re-transfer of nearly identical files (delta sync) is out of scope
  until a future ADR.

## References

- Architecture: [`0001-c-client-server-architecture.adr.md`](0001-c-client-server-architecture.adr.md)
- Spec: [`plans/file-transfer.spec.md`](../plans/file-transfer.spec.md)
- Mechanics: [`reference/client-server.ref.md`](../reference/client-server.ref.md)
