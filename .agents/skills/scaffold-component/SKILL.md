---
name: scaffold-component
description: >-
  Scaffolds a new server or client component (source files, build wiring, and
  docs) following ADR-0001. Use when adding the first server/client code,
  a new module under server/ or client/, or shared code in common/.
---

# Scaffold Component

> **Example skill** — shows the runbook format for this repo. Replace or extend
> this procedure as real conventions emerge.

Adds or extends **Server**, **Client**, or **common** code per
[`adr/0001-c-client-server-architecture.adr.md`](../../adr/0001-c-client-server-architecture.adr.md).

## Before coding

1. Read [`reference/client-server.ref.md`](../../reference/client-server.ref.md)
   for the planned layout and interaction.
2. Confirm names against [`rules/terms.rule.md`](../../rules/terms.rule.md).
3. Check [`plans/backlog.plan.md`](../../plans/backlog.plan.md) for the current
   priority (server vs client vs protocol ADR).

## Constraints

- C only for core transfer logic.
- **One binary**, server and client as CLI modes (ADR-0001 is **Proposed**).
- Shared protocol code in `common/`; do not duplicate between modes.
- Server mode must support **concurrent connections**; client mode is **one
  connection per invocation**.
- If the wire protocol is not decided, write a **Proposed** ADR before
  implementing framing on the wire.
- Update [`guides/commands.md`](../../guides/commands.md) when adding CLI modes or
  flags.

## Procedure

```
Task progress:
- [ ] 1. Confirm target component (server / client / common)
- [ ] 2. Add or extend source tree and build rules
- [ ] 3. Implement minimal happy path (no extra features)
- [ ] 4. Add path validation on server if touching file requests
- [ ] 5. Update reference/ and commands.md
- [ ] 6. Check off backlog item in plans/backlog.plan.md
```

1. **Pick the component.** Server (listen/serve), client (connect/request), or
   common (protocol helpers).
2. **Source tree.** Create the directory and entry file if missing. Wire into
   Makefile/CMake when the build system exists.
3. **Minimal behavior.** One happy path only — e.g. server accepts concurrent
   connections; each client run requests one file over one connection.
4. **Safety.** On server, validate paths stay under the serve root.
5. **Docs.** Update [`reference/client-server.ref.md`](../../reference/client-server.ref.md)
   and [`guides/commands.md`](../../guides/commands.md).
6. **Backlog.** Mark the matching todo completed in YAML and checklist.

## Completion checks

- Project builds (once a build system exists).
- No protocol logic duplicated outside `common/`.
- Reference and commands docs match the new flags/paths.
- Backlog YAML and checklist are in sync.
