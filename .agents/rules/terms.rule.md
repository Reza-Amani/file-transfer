# Project Terms

Project-wide terminology rules and conversational agreements. These definitions
apply across code, schemas, CLI flags, configuration, documentation, and agent
output. Use one name for one concept everywhere.

> **How to use this file:** Add a short entry for any term that has caused
> confusion or that must be used consistently.

## Server mode vs Client mode

- **Server mode** is when the binary listens for network connections, receives
  file requests, and reads files from its configured **serve root**. One process
  can handle **many connections at the same time**.
- **Client mode** is when the binary connects to a remote server, sends one
  **file request**, and writes received bytes to disk or stdout. **One client
  invocation uses one connection**, then exits.
- **Server** and **Client** name roles or modes, not separate programs. Do not
  call client mode "the server" or vice versa.

## File request

- **File request** is the client's ask for a single file by name (relative to the
  serve root). It is not a directory listing, upload, or sync operation unless
  a future ADR explicitly adds that capability.

## Serve root

- **Serve root** is the directory on the server host from which files may be
  served. The server must reject paths that escape this directory (e.g. `../`
  segments). Configuration key name TBD when CLI is implemented.

## Transfer vs sync

- **Transfer** means a one-way copy of a file from server to client for a single
  request. It does **not** mean bidirectional reconciliation; use **sync** only
  if a future feature explicitly reconciles state both ways.

## Component names in code

- Use `server` and `client` for mode-specific logic (e.g. `server/` and
  `client/` source directories, or `run_server` / `run_client` entry paths).
- Use `common` (or `lib`) for code shared by both modes (protocol helpers,
  error codes). Do not duplicate protocol logic.
