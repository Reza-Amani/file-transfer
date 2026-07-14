---
name: Project Backlog
overview: Canonical backlog for file-transfer — a minimal C client/server file transfer system in early development.
todos:
  - id: source-layout
    content: Define source tree (single binary, server/client modes, common/) and add first Makefile or CMake build
    status: pending
  - id: wire-protocol-adr
    content: Write ADR for on-the-wire protocol (framing, errors, connection model)
    status: pending
  - id: implement-server
    content: Implement server mode — listen, concurrent connections, validate serve root, read file, send bytes
    status: pending
  - id: implement-client
    content: Implement client mode — one connection per run, request file by name, write to disk or stdout
    status: pending
  - id: document-commands
    content: Fill guides/commands.md with real build/run flags once CLI exists
    status: pending
  - id: manifest-applied
    content: Bootstrap .agents/ manifest from manifest-template
    status: completed
isProject: true
---

# Project Backlog

> **Multi-session tracker.** Keep the YAML `todos` block and the checklists below
> in sync. ADRs remain authoritative when a tracker item conflicts with
> architecture.

## Purpose

- **Role:** Simple network file transfer — client requests a file by name; server
  serves from a configured directory.
- **Tracking scope:** Initial implementation, protocol design, build tooling, and
  follow-on hardening (tests, path traversal checks).

## Immediate Work

- [ ] Define source tree and build system
- [ ] ADR for wire protocol
- [ ] Implement server
- [ ] Implement client
- [ ] Document commands in [`../guides/commands.md`](../guides/commands.md)
- [x] Bootstrap agent manifest (`.agents/`)

## Long-Term Backlog

- [ ] Unit tests for path validation under serve root
- [ ] Integration test — client fetches a file from a test server
- [ ] CI workflow (build + test on push)

## Active Context

- **Primary goal:** Get a minimal working binary with server and client modes in C.
- **Design stance:** Loose until requirements are defined; ADR-0001 stays **Proposed**.
- **Related docs:** [`reference/client-server.ref.md`](../reference/client-server.ref.md),
  [`adr/0001-c-client-server-architecture.adr.md`](../adr/0001-c-client-server-architecture.adr.md)
