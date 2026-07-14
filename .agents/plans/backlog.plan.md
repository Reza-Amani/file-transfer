---
name: Project Backlog
overview: Canonical backlog for file-transfer — Windows C client/server file transfer per file-transfer.spec.md.
todos:
  - id: implement-per-spec
    content: Implement server/client/common per plans/file-transfer.spec.md and update build.ps1
    status: pending
  - id: docs-in-sync
    content: Keep README, design.ref.md, commands.md, setup.md aligned as CLI lands
    status: pending
  - id: smoke-script
    content: Optional PowerShell smoke test — transfer a known file and compare bytes
    status: pending
  - id: source-layout
    content: Define source tree (single binary, server/client modes, common/) and add first Makefile or CMake build
    status: completed
  - id: wire-protocol-adr
    content: Write ADR for on-the-wire protocol (framing, errors, connection model)
    status: completed
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

- **Role:** Simple network file transfer — client requests a bare filename; server
  serves from `--dir` over TCP with concurrent connections.
- **Tracking scope:** Implementation against
  [`file-transfer.spec.md`](file-transfer.spec.md), docs, and optional smoke test.

## Immediate Work

- [ ] Implement per [`file-transfer.spec.md`](file-transfer.spec.md) (layout,
      protocol, server threads, client `--out`, `build.ps1`)
- [ ] Keep README / guides / `design.ref.md` accurate as the CLI lands
- [ ] Optional: PowerShell smoke script (known file, byte compare)
- [x] Source layout and build scaffolding (hello world)
- [x] Wire protocol ADR ([`0002`](../adr/0002-line-oriented-transfer-protocol.adr.md))
- [x] Bootstrap agent manifest (`.agents/`)

## Long-Term Backlog

- [ ] Unit tests for bare-filename / serve-root validation
- [ ] Integration test beyond the optional smoke script
- [ ] CI workflow (build + test on push)
- [ ] Efficient re-transfer when client has a similar old copy (separate ADR)

## Active Context

- **Primary goal:** Working Windows binary with server and client modes.
- **Design stance:** Spec + Accepted ADRs are binding.
- **Related docs:** [`file-transfer.spec.md`](file-transfer.spec.md),
  [`reference/design.ref.md`](../reference/design.ref.md),
  [`reference/client-server.ref.md`](../reference/client-server.ref.md),
  [`adr/0001-c-client-server-architecture.adr.md`](../adr/0001-c-client-server-architecture.adr.md)
