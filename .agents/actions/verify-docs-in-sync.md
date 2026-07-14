---
name: verify-docs-in-sync
description: "Action: After a code change, detect manifest docs that no longer match the code, warn, and recommend the fix."
---

# Action: Verify Docs Are In Sync

Single-step safety check. Run it **after** editing code, **before** reporting a
change as complete. It detects whether a change has made any `.agents/` doc
wrong (a reference, guide, or plan that describes the changed behavior), warns
the user, and recommends the fix.

This is an **action**, not a full skill: it is one focused check with no branches.

## Trigger (run ONLY when relevant)

Run this action when **any** of these is true. If none match, skip silently — do
not mention it.

1. The change adds, removes, or renames a command, flag, config key, public
   function, or file path that a manifest doc names.
2. The change alters a documented sequence, schema, or default value.
3. The change touches a subsystem that has a `reference/*.ref.md`.

Quick gate: if the change does not touch anything named in `.agents/`, skip.

## Checks

For each item, inspect the diff. If a check fails, treat it as **drift**.

1. **Command/flag parity** — commands and flags in
   [`../guides/commands.md`](../guides/commands.md) still exist with the same
   names and defaults.
2. **Reference accuracy** — any `reference/*.ref.md` that describes the changed
   subsystem still matches the code (components, config table, sequence).
3. **Decision compliance** — the change does not contradict an accepted ADR in
   [`../adr/`](../adr/). If it does, do not edit the ADR; flag it.
4. **Backlog sync** — if the change completes a backlog item, it is checked off
   in [`../plans/backlog.plan.md`](../plans/backlog.plan.md) and its YAML
   `todos` status is updated.
5. **No broken links** — links between manifest docs still resolve after any
   file rename or move.

## Output

- **No drift:** state in one line that the docs are in sync. Do not elaborate.
- **Drift found:** emit a `⚠️` warning that names the failing check(s), the exact
  file/symbol, why the doc is now wrong, and the concrete fix. Apply low-risk doc
  fixes directly; for ADR contradictions, recommend and confirm first rather than
  editing the ADR.
