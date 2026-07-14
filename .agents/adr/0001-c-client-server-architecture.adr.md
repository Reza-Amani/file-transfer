# ADR-0001: C Client/Server Architecture

## Status

Accepted

## Context

We need a simple way to move files over a network: a program exposes files from
a directory when run in server mode, and fetches a file by name when run in
client mode. The implementation should stay small, readable, and free of
third-party dependencies beyond normal system libraries.

Alternatives considered, but not acted upon: two separate binaries; POSIX-only or dual Windows/POSIX
sockets; event-loop concurrency instead of threads.

## Decision

1. Implement the system in **C** (C11).
2. Ship **one binary** with two modes selected by subcommands: `server` and
   `client` (see [`terms.rule.md`](../rules/terms.rule.md)).
3. Target **Windows with Winsock** for the first implementation. Document that
   assumption; a POSIX port would need a later ADR.
4. In **server** mode: listen on TCP IPv4 (default port 9000, bind `0.0.0.0`),
   accept connections, spawn **one thread per connection**, validate each
   **file request** as a **bare filename** under `--dir` (**serve root**), read
   the file, send contents with chunked I/O, then close that connection.
5. In **client** mode: connect once, send one file request, receive data with
   chunked I/O, write to the required `--out` path, then exit. Overwrite if the
   path exists; delete a partial file on failure.
6. Shared protocol and utility code lives under `src/common/`; do not duplicate
   protocol logic between modes.

## Consequences

- `main` dispatches on the `server` / `client` subcommand; do not ship two
  separate `main` executables unless a future ADR changes this.
- Server code must use a thread-per-connection model (or superseding ADR).
- Wire protocol details are in
  [`0002-line-oriented-transfer-protocol.adr.md`](0002-line-oriented-transfer-protocol.adr.md)
  and [`reference/client-server.ref.md`](../reference/client-server.ref.md).
- Security review must treat the serve-root / bare-filename boundary as a trust
  boundary. No auth or encryption is required for the current threat model
  (non-sensitive content).

## References

- Spec: [`plans/file-transfer.spec.md`](../plans/file-transfer.spec.md)
- Design overview: [`reference/design.ref.md`](../reference/design.ref.md)
- Mechanics: [`reference/client-server.ref.md`](../reference/client-server.ref.md)
- Product overview: [root `README.md`](../../README.md)
