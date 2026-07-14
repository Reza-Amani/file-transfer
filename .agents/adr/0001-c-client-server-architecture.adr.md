# ADR-0001: C Client/Server Architecture

## Status

Proposed

## Context

We need a simple way to move files over a network: a program exposes files from
a directory when run in server mode, and fetches a file by name when run in
client mode.

Early direction (not final):

- **C** for portability and a small runtime surface.
- **One binary, two modes** — server and client distinguished by CLI, not separate
  installable programs.
- **One connection per client invocation** — each client run does one file
  request over one connection, then exits.
- **Concurrent server** — server mode accepts and serves multiple client
  connections at the same time while the process keeps running.

The wire protocol, build system, source tree, and exact CLI shape are not fixed.
This ADR records current intent until requirements are defined.

## Decision

1. Implement the system in **C**.
2. Ship **one binary** with two modes: **server** and **client** (see
   [`terms.rule.md`](../rules/terms.rule.md)). Mode is selected via CLI.
3. In **server** mode: listen on the network, accept concurrent connections,
   validate each **file request** against a configured **serve root**, read the
   file, send contents, handle that connection (exact lifecycle TBD).
4. In **client** mode: connect once, send one file request, receive data, write
   to a local file or stdout, then exit.
5. Shared protocol and utility code lives in one codebase (e.g. a `common/` or
   `lib/` area); do not duplicate protocol logic between mode implementations.

## Consequences

- Scaffold one main binary with server/client code paths or subcommands, not two
  separate `main` executables, unless a future ADR changes this.
- Server implementation must allow **multiple simultaneous connections** (e.g.
  per-connection handler, thread, or async model — choice TBD).
- Protocol details belong in a future ADR once the on-the-wire format is chosen.
- Security review must treat the serve-root boundary as a trust boundary.

## References

- Product overview: [root `README.md`](../../README.md)
- Planned mechanics: [`reference/client-server.ref.md`](../reference/client-server.ref.md)
