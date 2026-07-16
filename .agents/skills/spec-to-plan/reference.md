# Cursor Plan Mode format (for spec-to-plan)

Use this as the shape and depth bar when writing `.agents/plans/*.plan.md`.
Match Cursor Plan Mode: concrete file paths, actionable todos, and enough
detail that an implementer can build without re-deriving the design.

## Frontmatter

```yaml
---
name: short-plan-name
overview: One or two sentences on what will be built and why.
todos:
  - id: kebab-case-id
    content: Imperative task the implementer can complete and check off
    status: pending
  - id: another-step
    content: Next discrete step
    status: pending
isProject: false
---
```

Rules:

- `name`: short label (often matches the file topic).
- `overview`: the whole plan in one breath.
- `todos`: ordered build steps. Keep them small enough to finish in one pass
  each. Status starts as `pending` for a new plan.
- `isProject`: use `false` unless the target repo already uses another
  convention for workspace-saved plans.

## Body skeleton

Omit empty sections. Add sections the work needs (for example a behavior
matrix or version-bump rules).

```markdown
# <Title>

## Spec

Source of truth: [`../plans/<topic>.spec.md`](../plans/<topic>.spec.md)
(adjust the relative path). Prefer linking over copying acceptance text.

## Goals

- Bullet goals the implementation must hit.

## Approach

Short design narrative. Key decisions if the spec left room. Optional mermaid
when structure or flow matters:

\`\`\`mermaid
flowchart LR
  a[Caller] --> b[Service]
  b --> c[Store]
\`\`\`

## File-level changes

### 1) [`path/to/file`](path/to/file) — change

What to add, change, or remove. Name types, functions, flags, and contracts.
Include a short code sketch when the API must be exact.

### 2) [`path/to/new_file`](path/to/new_file) — NEW

Purpose and outline of contents.

## Acceptance criteria

Either:

- A short checklist derived from the spec, or
- "Meet the linked spec; it is the acceptance source of truth."

## Out of scope / what this does not change

Explicit non-goals and untouched call sites when that prevents scope creep.

## Risks / things to watch

Easy mistakes, ordering constraints, backward-compat traps.
```

## Detail levels

### Standard (default Plan Mode depth)

- Real paths and what changes in each.
- Named APIs, flags, and data shapes.
- Todos that map 1:1 to the file-level work.
- Risks and out-of-scope.

### High detail (cheap implementer or user request)

Everything in standard, plus:

- Exact signatures, field lists, and constants when known.
- Short copy-paste sketches for new helpers or types.
- Line or symbol anchors ("near `Migrate`, after `LiveTradeResult`").
- Per-todo "done when" checks (command to run, behavior to see).
- Edge cases and failure paths the implementer must not invent.
- Explicit "do not change" list for nearby code that looks related.

## Anti-patterns

- Vague steps like "update the backend" with no paths.
- Todos that do not appear in the body (or body work with no todo).
- Pasting the entire spec into the plan.
- Editing the spec from this skill.
- Saving under `~/.cursor/plans/` or `.cursor/plans/` instead of
  `.agents/plans/`.
