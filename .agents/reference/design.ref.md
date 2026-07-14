# Design Overview

High-level design and assumptions for the file-transfer program. Binding
decisions live in ADRs; this document explains the shape for readers of the
codebase.

> **Authority:** [`adr/0001`](../adr/0001-c-client-server-architecture.adr.md),
> [`adr/0002`](../adr/0002-line-oriented-transfer-protocol.adr.md). Spec:
> [`plans/file-transfer.spec.md`](../plans/file-transfer.spec.md).

## Goals

- Move a file from a server directory to a client path over the network
- Keep the code simple, readable, and lightly commented at file and function level
- Use only normal system libraries (Winsock + C runtime on Windows)
- Support multiple clients connected to the server at the same time

## Non-goals

- Encryption or authentication
- Directory listing, uploads, or multi-file sessions
- POSIX / Linux as a first-class target
- Delta / efficient re-transfer when the client already has a similar copy

## Architecture

One binary, two modes (`server` / `client` subcommands):

```text
src/
  main.c              # parse argv, dispatch mode
  server/             # listen, accept, thread per connection
  client/             # connect, request, write --out
  common/             # protocol, Winsock helpers, path checks
```

```text
  +--------+         TCP / IPv4          +--------+
  | Client | ---- GET filename -------> | Server |
  |  mode  | <--- OK size + bytes ----- |  mode  |
  +--------+                            +--------+
                                         |
                                    serve root (--dir)
```

Server concurrency: **one thread per accepted connection**. Client: **one
connection per process invocation**, then exit.

## Protocol (summary)

See [ADR-0002](../adr/0002-line-oriented-transfer-protocol.adr.md) and
[`client-server.ref.md`](client-server.ref.md).

- Request: `GET <bare-filename>\n`
- Success: `OK <byte-size>\n` + raw bytes
- Error: `ERR <message>\n`

## Assumptions

- Content is not sensitive; the network may be a LAN or similar
- The server host allows inbound TCP on the chosen port (default **9000**)
- The client host needs outbound TCP only
- Filenames are bare names under `--dir` (no subdirectories in the request)
- Binary files are supported; transfer is opaque bytes
- Limited bandwidth / latency is handled by TCP plus chunked send/recv buffers,
  not by an application-level ACK protocol
- Chunk size is **64 KiB** (`FT_CHUNK_SIZE` in `src/common/protocol.h`)

## Error handling (client `--out`)

- Missing or invalid remote file → non-zero exit; no useful output file
- Mid-transfer failure → delete the partial `--out` file
- Existing `--out` → overwrite
