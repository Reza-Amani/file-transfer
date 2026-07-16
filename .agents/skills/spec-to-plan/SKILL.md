---
name: spec-to-plan
description: >-
  Turns a spec (or the user's stated intent) into an implementation plan in
  Cursor Plan Mode format and saves it under `.agents/plans/*.plan.md`. Does not
  edit the spec. Use when the user asks to "plan from this spec", "make a plan
  for the spec", "spec to plan", or wants a Cursor-style plan before building.
---

# Spec to Plan

Read a spec (or gather the intended work if there is no spec file), research the
repo, and write an implementation plan the way **Cursor Plan Mode** does. Save
the plan under `.agents/plans/`. Do not change the spec.

This skill pairs with [`grill-to-spec`](../grill-to-spec/SKILL.md): that skill
writes `*.spec.md`; this one turns a ready spec into `*.plan.md`.

## Inputs you must resolve first

- **Target repo:** this repository. The plan goes here under `.agents/plans/`.
- If `manifest-template` is also open in the workspace, treat it as read-only
  and never write there.
- **Spec:** a path to a `*.spec.md` (or other spec file the user names), **or**
  enough intent and context from the user to plan without a file.

If the workspace is ambiguous (multiple non-template repos), ask the user
which folder is the target before writing anything.

> **Safety:** Write the plan only in the **target** repo. Never edit the spec.

## Phase 1: Resolve the spec

1. If the user named a spec file, open it and treat it as the source of truth.
2. If there is no spec file, ask for the intended work and any context (goal,
constraints, out of scope, links to ADRs or prior notes). Do not invent a
full spec file unless the user asks for one.
3. If the user points at a vague goal with no detail, ask for enough context to
plan. Prefer short, focused questions.

### Spec problems (do not edit)

If the spec is incomplete, contradictory, or unclear:

- **Do not** change the spec.
- Tell the user what is wrong and what is blocked.
- Ask whether to pause, proceed with stated assumptions (list them), or wait
for a spec fix.

Only continue once the user accepts assumptions or fixes the gaps.

## Phase 2: Research before writing

Research the **target** repo the same way Plan Mode does:

1. Using the manifest files and routing rule, find the files, modules, and tests the change will touch.
2. Read relevant ADRs, references, and rules from the target's `.agents/` when
the task touches their domain (use the target's `routing.rule.md` if present).
3. Prefer facts from the repo over guesses.

Do not implement code in this skill. Planning only.

## Phase 3: Choose detail level

Match **Cursor Plan Mode** by default: concrete enough to build from (goals,
approach, file-level changes, todos, risks), not a vague outline.

Use a **high-detail** plan when any of these is true:

- The user asks for more detail.
- A smaller or cheaper model will implement the plan later.
- The change is easy to get wrong (many files, subtle contracts, migrations).

High-detail plans add: exact signatures or field lists, short code sketches,
line or symbol anchors where known, per-todo verification, edge cases, and a
clear "do not change" list. Goal: a weaker implementer should not invent the
wrong API or skip a constraint.

Ask once if detail level is unclear and the user did not say which model will
build. Default to Plan Mode depth if they do not answer.

## Phase 4: Write the plan file

Save to the **target** repo only:

```
.agents/plans/<topic>.plan.md
```

Use a short dash-case `<topic>` (for example `partial-refunds.plan.md`). You may
use a subdirectory under `plans/` when the user asks or when the target already
groups plans that way (for example `plans/billing/partial-refunds.plan.md`).
Create `.agents/plans/` if needed.

**Never** put the plan in `~/.cursor/plans/`, `.cursor/plans/`, or
`manifest-template` when it is open alongside this repo.

### Format (Cursor Plan Mode)

Use the same shape Cursor Plan Mode uses. Full template and section guidance:
[`reference.md`](reference.md).

Required shape:

1. **YAML frontmatter** with `name`, `overview`, `todos` (each with `id`,
`content`, `status: pending`), and `isProject: false`.
2. **Markdown body** with:
  - Title
  - Goals / approach (and key decisions if the spec leaves choices)
  - File-level changes with real paths and what to do in each
  - Optional mermaid diagram when architecture or data flow matters
  - Out of scope / what does not change
  - Risks or watch-outs
  - **Acceptance:** either a short checklist derived from the spec, **or** a
clear link to the original spec as the source of truth / acceptance
criteria when that is better than duplicating it

Todos in frontmatter must match the work in the body. Keep `id` values short
and dash-case. Write `content` as a clear imperative the implementer can check
off.

Link to the spec from the plan (relative path). Do not copy the whole spec into
the plan.

## Phase 5: Hand off

After writing the plan:

1. Tell the user the plan path.
2. Summarize open assumptions (if any) and any spec issues still outstanding.
3. Suggest implementing in a **separate** chat or Agent/Build pass from the
plan. Do not start coding unless the user asks.

## Decision tree

```
Spec file given?
├─ Yes → read it; do not edit
└─ No  → ask for intent + context

Spec has blocking gaps?
├─ Yes → notify user; pause or proceed only with their OK + listed assumptions
└─ No  → research target repo

Detail level?
├─ User asked for more detail OR cheap/small implementer OR fragile change
│    → high-detail Cursor-style plan
└─ Else → standard Cursor Plan Mode depth

Write → <target>/.agents/plans/<topic>.plan.md
```

## Completion checks

- Plan exists at `<target>/.agents/plans/**/*.plan.md` (or a `plans/`
subdirectory) with Cursor-style frontmatter and body.
- Spec was not modified.
- Spec problems were reported to the user when found.
- Acceptance is either in the plan or clearly deferred to the linked spec.
- Detail level matches Phase 3.
- Nothing was written into `manifest-template` when it is open alongside this repo.
