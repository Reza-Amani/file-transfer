# ADR-0000: Record Architecture Decisions

## Status

Accepted

## Context

We make architecturally significant decisions as the project evolves: choices
about structure, technologies, boundaries, and constraints. When the reasoning
behind a decision lives only in someone's head, a chat thread, or a commit
message, it gets lost. New contributors (human or AI agent) then re-litigate
settled questions or unknowingly violate constraints.

We need a lightweight, durable, version-controlled way to capture the *why*
behind decisions, not just the *what*.

You need an ADR only when all these 3 conditions are met:
1. **Hard to reverse** — the cost of changing your mind later is meaningful
2. **Surprising without context** — a future reader will wonder "why did they do it this way?"
3. **The result of a real trade-off** — there were genuine alternatives and you picked one for specific reasons

## Decision

We record each architecturally significant decision as an **Architecture
Decision Record (ADR)** — a short Markdown file in [`.agents/adr/`](./).

- Each ADR is immutable once **Accepted**. To change a decision, write a new ADR
  that supersedes the old one and update the old one's status.
- ADRs are numbered sequentially and named `NNNN-short-title.adr.md`.
- Each ADR uses the sections below: **Status, Context, Decision, Consequences**
  (and optional **References**).
- ADRs are the **authority**. Reference docs (`*.ref.md`) describe how things
  currently work; when a reference and an ADR disagree, the ADR wins and the
  reference must be corrected.

### Status values

- **Proposed** — under discussion, not yet binding.
- **Accepted** — in force; agents and contributors must comply.
- **Superseded by ADR-XXXX** — replaced by a newer decision.
- **Deprecated** — no longer relevant, kept for history.

## Consequences

- Decisions become discoverable and reviewable in version control.
- Agents can read [`.agents/rules/routing.rule.md`](../rules/routing.rule.md) to
  find the ADR relevant to a task instead of guessing intent.
- There is a small, worthwhile cost: each significant decision needs a short
  write-up before it is considered done.
- Reviewers (see [`personas/reviewer.agent.md`](../personas/reviewer.agent.md))
  can check changes against accepted ADRs.

## References

- Michael Nygard, "Documenting Architecture Decisions" (the original ADR essay).
- Folder conventions: [`.agents/rules/manifest-structure.rule.md`](../rules/manifest-structure.rule.md).
