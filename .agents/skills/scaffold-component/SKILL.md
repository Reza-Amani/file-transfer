---
name: scaffold-component
description: >-
  Scaffolds a new server or client component (source files, build wiring, and
  docs) following ADR-0001. Use when adding the first server/client code,
  a new module under server/ or client/, or shared code in common/.
---

# Scaffold Component

Adds or extends **Server**, **Client**, or **common** code per
[`adr/0001-c-client-server-architecture.adr.md`](../../adr/0001-c-client-server-architecture.adr.md)
and [`plans/file-transfer.spec.md`](../../plans/file-transfer.spec.md).

## Before coding

1. Read [`plans/file-transfer.spec.md`](../../plans/file-transfer.spec.md) and
   [`reference/client-server.ref.md`](../../reference/client-server.ref.md).
2. Confirm names against [`rules/terms.rule.md`](../../rules/terms.rule.md).
3. Follow the wire protocol in
   [`adr/0002-line-oriented-transfer-protocol.adr.md`](../../adr/0002-line-oriented-transfer-protocol.adr.md).
4. Check [`plans/backlog.plan.md`](../../plans/backlog.plan.md) for current priority.

## Constraints

- C only for core transfer logic; **Windows / Winsock**.
- **One binary**, `server` / `client` subcommands (ADR-0001 **Accepted**).
- Shared protocol code in `src/common/`; do not duplicate between modes.
- Server: **thread per connection**; client: **one connection per invocation**.
- Bare filenames only under `--dir`; client `--out` required.
- Update [`guides/commands.md`](../../guides/commands.md) when changing CLI flags.

## Procedure

```
Task progress:
- [ ] 1. Confirm target component (server / client / common)
- [ ] 2. Add or extend source tree and build.ps1
- [ ] 3. Implement minimal happy path (no extra features)
- [ ] 4. Add bare-filename validation on server if touching file requests
- [ ] 5. Update reference/ and commands.md
- [ ] 6. Check off backlog item in plans/backlog.plan.md
```

1. **Pick the component.** Server (listen/serve), client (connect/request), or
   common (protocol / net / path helpers).
2. **Source tree.** Create the directory and entry file if missing. Wire into
   [`build.ps1`](../../../build.ps1).
3. **Minimal behavior.** One happy path only — protocol GET/OK/ERR, chunked I/O.
4. **Safety.** On server, accept bare filenames only under the serve root.
5. **Docs.** Update [`reference/client-server.ref.md`](../../reference/client-server.ref.md)
   and [`guides/commands.md`](../../guides/commands.md) if behavior or flags change.
6. **Backlog.** Mark the matching todo completed in YAML and checklist.

## Completion checks

- Project builds with `.\build.ps1`.
- No protocol logic duplicated outside `common/`.
- Reference and commands docs match the new flags/paths.
- Backlog YAML and checklist are in sync.
