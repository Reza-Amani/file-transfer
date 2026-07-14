# Project Terms

Project-wide terminology rules and conversational agreements. These definitions
apply across code, schemas, CLI flags, configuration, documentation, and agent
output. Use one name for one concept everywhere.

> **How to use this file:** Add a short entry for any term that has caused
> confusion or that must be used consistently.

## Server mode vs Client mode

- **Server mode** is when the binary listens for network connections, receives
  file requests, and reads files from its configured **serve root**. One process
  can handle **many connections at the same time** (one thread per connection).
- **Client mode** is when the binary connects to a remote server, sends one
  **file request**, and writes received bytes to a local **output path**. **One
  client invocation uses one connection**, then exits.
- **Server** and **Client** name roles or modes, not separate programs. Do not
  call client mode "the server" or vice versa.

## File request

- **File request** is the client's ask for a single file by **bare filename**
  (no directories). It is not a directory listing, upload, or sync operation
  unless a future ADR explicitly adds that capability.

## Bare filename

- A **bare filename** is a single path segment with no `/`, `\`, or `..`. The
  server resolves it only under the **serve root**.

## Serve root

- **Serve root** is the directory on the server host from which files may be
  served. Set with the server CLI flag `--dir`. The server must reject names that
  are not bare filenames.

## Output path

- **Output path** is the local filesystem path where client mode writes the
  received file. Set with `--out` (required). On mid-transfer failure the client
  deletes a partial output file.

## Transfer vs sync

- **Transfer** means a one-way copy of a file from server to client for a single
  request. It does **not** mean bidirectional reconciliation; use **sync** only
  if a future feature explicitly reconciles state both ways. Efficient re-transfer
  when the client already has a similar copy is out of scope until a future ADR.

## Component names in code

- Use `server` and `client` for mode-specific logic (directories `server/` and
  `client/`, or `run_server` / `run_client` entry paths).
- Use `common` for code shared by both modes (protocol helpers, Winsock helpers,
  path validation). Do not duplicate protocol logic.
