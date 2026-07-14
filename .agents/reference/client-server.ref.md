# Client/Server Mechanics

Operational description of the file-transfer system. For current architectural
intent (not yet binding), see
[`adr/0001-c-client-server-architecture.adr.md`](../adr/0001-c-client-server-architecture.adr.md).

> **Status:** Early development. No source code or wire protocol is implemented
> yet. Design is intentionally loose — update this doc as requirements firm up.

## Components

| Piece | Planned shape | Responsibility |
|-------|---------------|----------------|
| Binary | `file-transfer` (name TBD) | Single executable; **server** or **client** mode via CLI |
| Server mode | subcommand / flag TBD | Listen, accept **concurrent** connections, validate requests, read files, send bytes |
| Client mode | subcommand / flag TBD | Connect once, one file request, receive bytes, write to file or stdout, exit |
| Shared code | `common/` or `lib/` (TBD) | Protocol framing, error codes, helpers used by both modes |

## Planned interaction

**Per client invocation** (one connection):

```
  Client mode                     Server mode
    |                                |
    | -------- TCP connect --------> |  (server may serve other clients in parallel)
    |                                |
    | ---- file request (name) ----> |
    |                                | validate path under serve root
    |                                | read file from disk
    | <------- file contents ------- |
    |                                |
    | -------- close / EOF --------> |
```

**Server runtime:** one long-lived server process; many clients may connect and
transfer files **at the same time**. How concurrency is implemented (threads,
process-per-connection, event loop, etc.) is **not decided**.

Transport is assumed to be TCP until an ADR chooses otherwise. Port, framing,
and error responses are **not decided** — track in
[`plans/backlog.plan.md`](../plans/backlog.plan.md).

## Planned source layout

Exact paths will be confirmed when the first commit adds source files. Expected
shape:

```
file-transfer/
  src/
    main.c          # CLI dispatch: server vs client mode
    server/         # listen, accept, per-connection handling
    client/         # connect, request, receive
    common/         # protocol + shared utilities
  Makefile          # or CMakeLists.txt — TBD
```

## Configuration (planned)

| Setting | Mode | Purpose |
|---------|------|---------|
| Serve root path | Server | Directory files are read from |
| Listen address/port | Server | Where clients connect |
| Server address | Client | Where to connect |
| Output path | Client | Local file to write; omit for stdout |

CLI flag names belong in [`guides/commands.md`](../guides/commands.md) once
implemented.

## Security notes

- Treat every file request as untrusted input.
- Resolve the requested name under the serve root and reject `..` and absolute
  paths that escape the root.
- Do not follow symlinks outside the serve root unless a future ADR allows it.
