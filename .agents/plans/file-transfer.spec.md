# File Transfer Spec

> Produced by a grilling session on 2026-07-14. Status: ready for planning.

## Goal

Implement a simple, readable C client/server file transfer. A single Windows binary runs in **server** or
**client** mode: the server serves files from a configured directory over TCP
with concurrent connections; the client requests one file by name and writes it
to the local filesystem.

The code should demonstrate clear architecture, competence with sockets
and concurrency, and enough comments for reviewers — not maximal features.
Delta / efficient re-transfer when the client already has a similar copy is
**out of scope** for this spec.

## Decisions

1. **One binary, two modes** — Subcommands `server` and `client` (not two
   installable programs). Matches [ADR-0001](../adr/0001-c-client-server-architecture.adr.md)
   intent; CLI shape fixed here.

2. **Windows / Winsock only** — Develop and demo with the existing
   [`build.ps1`](../../build.ps1) / MinGW path. No POSIX dual-stack layer for
   this code. Document the platform assumption in README /
   [`design.ref.md`](../reference/design.ref.md).

3. **TCP, IPv4** — Default port **9000**, configurable. Server binds
   `0.0.0.0` so another laptop can connect. Client uses outbound TCP only
   (matches typical client firewall: inbound closed, outbound open).

4. **Line-oriented application protocol** — After TCP connect:
   - Client sends: `GET <filename>\n`
   - Server success: `OK <size>\n` then exactly `<size>` raw file bytes
   - Server failure: `ERR <message>\n` (no body)
   - One request per connection; then close.

5. **Concurrency** — Server accept loop spawns **one thread per connection**
   (`CreateThread` / `_beginthreadex`), then returns to `accept`. Prefer
   clarity over an event-loop state machine.

6. **Serve root via CLI** — `file-transfer server --port <port> --dir <serve-root>`.
   Files are read only from that directory.

7. **Bare filenames only** — Request name must not contain `/`, `\`, or `..`.
   No subpaths, no directory listing.

8. **Client CLI** —
   `file-transfer client --host <addr> --port <port> --file <name> --out <path>`.
   `--out` is **required**. If the path exists, **overwrite**. On mid-transfer
   failure, **delete** the partial `--out` so a corrupt file is not left behind.

9. **Chunked I/O** — Fixed-size buffer loop (e.g. 8–64 KiB) for disk ↔ socket.
   Do not require the whole file in memory. Binary files and empty files
   (`OK 0\n`) are supported. Rely on TCP for reliability; no app-level ACK
   protocol in v1.

10. **Source layout** —
    ```text
    src/
      main.c              # argv parse, dispatch
      server/server.c/.h  # listen, accept, threads, per-connection serve
      client/client.c/.h  # connect, request, receive, write --out
      common/
        protocol.c/.h     # GET/OK/ERR helpers, constants
        net.c/.h          # Winsock init/cleanup, send_all / recv_line
        path.c/.h         # bare-filename validation
    ```
    Soft rule: no source file longer than ~500 lines (expected sizes far below).

11. **Documentation package** —
    - Generous inline comments + top-of-file design blurbs on each `.c` / key `.h`
    - [`.agents/reference/design.ref.md`](../reference/design.ref.md) — design
      and assumptions (create during implementation planning)
    - [`.agents/guides/setup.md`](../guides/setup.md) — install / toolchain
    - [`.agents/guides/commands.md`](../guides/commands.md) — build, run,
      two-laptop examples
    - Root [README](../../README.md) — short build/run/two-laptop duplicate,
      links to setup + commands + design; one line that
      [`routing.rule.md`](../rules/routing.rule.md) /
      [`manifest-structure.rule.md`](../rules/manifest-structure.rule.md) are
      for **agents**, not required reading for human reviewers

12. **Testing** — Manual server+client demo is required. Optional stretch: a
    PowerShell smoke script that transfers a known file and checks bytes if
    time allows.

13. **Errors** —

    | Case | Behavior |
    |------|----------|
    | Missing file | `ERR File not found\n`; client non-zero exit |
    | Illegal name | `ERR Invalid path\n`; no disk access outside validation |
    | Empty file | `OK 0\n`, success |
    | `--out` exists | Overwrite |
    | Connection drop mid-transfer | Client deletes partial `--out`, non-zero exit |

## Scope

**In scope**

- Working Windows C binary with server and client modes as above
- Path validation under serve root (bare name → file under `--dir`)
- Concurrent transfers via threads
- Readable architecture, comments, and docs
- Update build (`build.ps1`) to compile the new tree

**Out of scope (explicit)**

- Delta / rsync-like efficient re-transfer when the client has an old copy
- Encryption, authentication, access control beyond serve-root / bare-name checks
- POSIX / Linux port, IPv6 dual-stack
- HTTP protocol, UDP
- Directory listing, uploads, multi-file sessions
- Full unit-test framework / CI (optional smoke script only)

## Details

### Suggested CLI examples

```powershell
# Server (on machine with open inbound ports)
.\build\file-transfer.exe server --port 9000 --dir C:\share

# Client (outbound to server)
.\build\file-transfer.exe client --host 192.168.1.10 --port 9000 --file report.pdf --out .\report.pdf
```

### Protocol sketch

```text
Client                         Server
  |-- TCP connect -------------->|
  |-- GET report.pdf\n --------->|
  |                              | validate name; open under serve root
  |<- OK 1234\n -----------------|
  |<- <1234 bytes> --------------|
  |-- close -------------------->|
```

### UX

- Server prints a one-line listening message
- Client prints brief progress (e.g. size / done)
- No logging framework

### Dependencies

- Normal system libraries only (Winsock, C runtime). No third-party packages.

## Constraints

- Keep code simple and readable; prefer obvious control flow over cleverness
- Soft cap ~500 lines per source file
- Do not implement delta / efficient re-transfer in this workstream
- Prefer linking existing ADRs/terms over duplicating them in new docs.
  Implementation follows this spec together with Accepted ADRs 0001 and 0002.

## Open questions

- Exact buffer size (8 vs 64 KiB) — implementer choice; document in
  `design.ref.md` or a constant comment
- Whether the optional PowerShell smoke script is written — decide by remaining
  time after core transfer works

## Implementation note

Plan and implement this spec in a **separate** chat/session from grilling.
Start from [`plans/backlog.plan.md`](backlog.plan.md) and this file.
Manifest docs (ADRs, terms, references, guides, README) were aligned to this
spec after the grilling session.
