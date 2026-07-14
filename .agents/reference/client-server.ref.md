# Client/Server Mechanics

Operational description of the file-transfer system. For decisions, see
[`adr/0001-c-client-server-architecture.adr.md`](../adr/0001-c-client-server-architecture.adr.md)
and
[`adr/0002-line-oriented-transfer-protocol.adr.md`](../adr/0002-line-oriented-transfer-protocol.adr.md).
For a short design summary, see [`design.ref.md`](design.ref.md).

## Components

| Piece | Shape | Responsibility |
|-------|-------|----------------|
| Binary | `file-transfer.exe` (Windows) | Single executable; **server** or **client** mode via subcommand |
| Server mode | `server --port <p> --dir <root>` | Listen, accept, thread per connection, validate bare name, send file |
| Client mode | `client --host --port --file --out` | Connect once, one request, write `--out`, exit |
| Shared code | `src/common/` | Protocol framing, Winsock helpers, path validation |

## Interaction

**Per client invocation** (one connection):

```text
  Client mode                     Server mode
    |                                |
    | -------- TCP connect --------> |  (other clients may run in parallel)
    |                                |
    | ---- GET <filename>\n -------> |
    |                                | validate bare filename under serve root
    |                                | open and read file
    | <----- OK <size>\n ----------- |
    | <----- <size> bytes ---------- |  (chunked)
    |                                |
    | -------- close --------------> |
```

On failure the server sends `ERR <message>\n` and no body.

**Server runtime:** one long-lived process; each accepted connection is handled
on its own thread.

**Transport:** TCP, IPv4. Default port **9000**. Server binds `0.0.0.0`.

## Source layout

```text
file-transfer/
  src/
    main.c
    server/server.c  server.h
    client/client.c  client.h
    common/
      protocol.c  protocol.h
      net.c       net.h
      path.c      path.h
  build.ps1
  Makefile          # optional / legacy; Windows canonical build is build.ps1
```

Soft rule: keep each source file well under ~500 lines.

## Configuration

| Setting | Mode | Flag | Purpose |
|---------|------|------|---------|
| Serve root | Server | `--dir` | Directory files are read from |
| Listen port | Server | `--port` | TCP port (default 9000) |
| Server host | Client | `--host` | Address to connect to |
| Server port | Client | `--port` | TCP port |
| Remote file | Client | `--file` | Bare filename to request |
| Output path | Client | `--out` | Local path to write (required) |

CLI details and examples: [`guides/commands.md`](../guides/commands.md).

## Security notes

- Treat every file request as untrusted input.
- Accept **bare filenames** only; reject `/`, `\`, and `..`.
- Open files only under the serve root (`--dir`).
- Do not follow a design that serves paths outside that root.
