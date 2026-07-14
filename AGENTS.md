# AGENTS.md

Entry point for all AI agents and contributors working in this repository
(GitHub Copilot, Cursor, Claude, openCode, and future tools).

**Before acting, read [`.agents/rules/routing.rule.md`](.agents/rules/routing.rule.md)**
and follow its routing table to find the specific decision record, reference,
guide, skill, or persona for your task. Read only what the current task needs.
Do not bulk-read every file under `.agents/` at once.

## Project direction

Design is still loose — treat [`adr/`](.agents/adr/) entries marked **Proposed**
as direction, not hard law, until requirements are settled and ADRs are
**Accepted**.

- **C only.** Core transfer logic in C. Other languages need a new ADR.
- **One binary, two modes.** A single program runs as **server** or **client**,
  selected by CLI (subcommand or flag — exact shape TBD).
- **One connection per client invocation.** Each client run handles one file
  request over one connection, then exits.
- **Concurrent server.** In server mode, the process accepts and serves **multiple
  incoming connections at the same time** at runtime.
- **Serve from a configured directory.** Server mode reads files only from an
  explicitly configured root. Validate paths; do not serve outside that root.
- **Early project.** Source layout and wire protocol are still open. Check
  [`plans/backlog.plan.md`](.agents/plans/backlog.plan.md) before large features.

## Generic rules

- **Read the decisions first.** **Accepted** ADRs are binding. **Proposed** ADRs
  capture intent until requirements are defined — do not treat them as frozen.
- **Keep docs in sync.** If your change makes a reference or guide wrong, fix it
  in the same change. See [`.agents/rules/manifest-structure.rule.md`](.agents/rules/manifest-structure.rule.md).
- **Check the backlog before proposing work.** The canonical task list lives in
  [`.agents/plans/backlog.plan.md`](.agents/plans/backlog.plan.md).

## What is `.agents/`?

`.agents/` is the source tree for agent-facing project context: decisions,
mechanics, conventions, runbooks, and role definitions. It is tool-agnostic.
Tool-specific folders such as `.github/` or `.cursor/` may mirror or link to it,
but the canonical content lives here. See the
[root README](README.md) for the full tour.
