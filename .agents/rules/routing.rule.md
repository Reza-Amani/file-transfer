# Routing Guide

Single source of truth for discovering manifest documents. Maps a task to the
**specific** ADR, reference, skill, persona, plan, learning record, or action that covers it.

> **How to use this file:** Scan the tables for the row that matches your task,
> then open only the file it names.

**Context discipline:** Do not bulk-read the manifest. Open a specific file
**only when the current task touches its domain.** ADRs are the authority;
reference docs explain mechanics. ADRs > reference notes.

For project-wide mandates, see the root [`AGENTS.md`](../../AGENTS.md) — it is
always in context and need not be re-read here. For project-wide terminology,
see [`terms.rule.md`](terms.rule.md).

## Task → Rules

- Project-wide terminology; applies to all tasks → [`terms.rule.md`](terms.rule.md)
- Manifest structure; applies when editing manifest files → [`manifest-structure.rule.md`](manifest-structure.rule.md)

## Task → ADR (decisions)

- How and why we record decisions → [`adr/0000-record-architecture-decisions.adr.md`](../adr/0000-record-architecture-decisions.adr.md)
- C language, client/server split, and serve-directory model → [`adr/0001-c-client-server-architecture.adr.md`](../adr/0001-c-client-server-architecture.adr.md)

## Task → Reference (mechanics)

- Server and client roles, planned interaction, and source layout → [`reference/client-server.ref.md`](../reference/client-server.ref.md)

## Task → Skill (multi-step procedures)

- Scaffold a new server or client component end-to-end → [`skills/scaffold-component`](../skills/scaffold-component/SKILL.md)

## Task → Learning (teach session records)

- Record or resume a `teach` skill session → the state is kept under [`learning`](../learning) and is managed by the `teach` skill only.

## Task → Findings (analysis & investigation records)

- Record a finding or resume an analysis/investigation → [`findings/README.md`](../findings/README.md)

## Task → Action (one-step checks)

- Verify docs match code before declaring done → [`actions/verify-docs-in-sync.md`](../actions/verify-docs-in-sync.md)

## Task → Plan (multi-step work across sessions)

- Canonical backlog and current focus → [`plans/backlog.plan.md`](../plans/backlog.plan.md)

Important:

- When you are in planning mode, or create a plan per user request, save the plan
  under `plans/` with a descriptive name and a `.plan.md` suffix. No other
  location is allowed for plans.
- When the user asks to continue or edit a plan, look for the plan under
  `plans/`, regardless of the default plan directory for the agent.

## Specialized agents

Defined in `personas/*.agent.md`:

- `reviewer` — pre-commit reviewer focused on safety, correctness, and decision
  compliance → [`personas/reviewer.agent.md`](../personas/reviewer.agent.md)
- `manifester` — manifest keeper who writes and fixes `.agents/` docs in plain
  English → [`personas/manifester.agent.md`](../personas/manifester.agent.md)

## Plans & actions

- Multi-session plans live in `plans/`. The canonical backlog is
  [`plans/backlog.plan.md`](../plans/backlog.plan.md).
- Teach session records live in `learning/`. Start with
  [`learning/README.md`](../learning/README.md).
- Analysis & investigation records live in `findings/`. Start with
  [`findings/README.md`](../findings/README.md).
- One-step procedures live in `actions/` (e.g.
  [`actions/verify-docs-in-sync.md`](../actions/verify-docs-in-sync.md)).

## Guides (setup & commands)

- First-time setup (toolchain, manifest wiring) → [`guides/setup.md`](../guides/setup.md)
- Build, test, run, and CLI commands → [`guides/commands.md`](../guides/commands.md)
- Scratch notes and open questions → [`guides/notes.md`](../guides/notes.md)
