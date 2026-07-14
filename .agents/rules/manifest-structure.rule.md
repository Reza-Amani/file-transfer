# Manifest Structure Rule

This rule defines how the project organizes agent-facing manifest files under
`.agents/`. Use it when adding, moving, renaming, or reviewing manifest files,
or when deciding whether information belongs in a decision record (ADR),
reference, rule, skill, guide, plan, action, persona, or learning record.

> **This file is required.** It is the one document that explains the whole
> structure. Keep it in every copy of the template.

## Manifest Scope

The `.agents/` directory contains domain knowledge, decision records,
operational notes, global rules, and specialized task automation for AI agents
and human contributors. It is the source tree for agent-facing project context.
Tool-specific mirrors such as `.github/` or `.cursor/` may point at or copy from
`.agents/`, but the canonical content belongs here.

The same structure serves **two kinds of repository**:

- **Software / development repos** — code, services, libraries. Examples in this
  template use the coding flavor (layered architecture, modules, build/test
  commands).
- **Operation / data repos** — scripts and instructions for data sources (Google
  Analytics 4, Salesforce), pipelines, dashboards, and data collection
  (data-analyst / data-science work). These use the operation flavor: connection
  inventories, metric glossaries, secret policies, and data runbooks.

The folders, naming, and routing are identical for both. Only the content
differs. Operation-flavored example files are named with an `operation-` prefix
(for example `reference/operation-connections.ref.md`,
`rules/operation-secrets.rule.md`) so they sit clearly beside their coding
counterparts.

## Folder Reference

### `adr/`

**Architecture Decision Records** — strategic decisions that shape the codebase.

- **Format:** Markdown files with optional YAML frontmatter.
- **Naming:** `*.adr.md` suffix.
- **Content:** Status, context, decision, consequences, and optional references.
- **Purpose:** Explain what was decided and why: design patterns, layering,
  ownership rules, constraints, and high-level business logic.
- **Audience:** Developers and agents planning or implementing changes.
- **Rule of thumb:** Put durable decisions here, not command walkthroughs or
  current implementation inventories.

### `reference/`

**Descriptive Documentation** — operational and implementation details, not
architecture decisions.

- **Format:** Markdown files with optional YAML frontmatter.
- **Naming:** `*.ref.md` suffix.
- **Content:** Mechanics: sequences, inventories, wiring, pipeline steps,
  command surfaces, API indices, rollout caveats.
- **Purpose:** Complement ADRs with concrete details needed for implementation
  or debugging.
- **Audience:** Developers and agents building or troubleshooting subsystem work.
- **Pairing:** Reference docs may share a base name with an ADR and cross-link
  it when there is a natural pair.

### `rules/`

**Global Rules and Routing Guides** — project-wide conventions and discovery
maps.

- **Format:** Markdown files.
- **Naming:** `<category>.rule.md`.
- **Content:** Rules that apply across manifest files, project-wide terminology,
  routing maps, naming conventions, and documentation placement rules.
- **Purpose:** Provide the rules agents should consult before choosing context or
  editing manifest documents.
- **Audience:** Agents and developers working with manifest files.

Standard rule files included in this template:

- `manifest-structure.rule.md` (this file): the placement rule for the manifest
  itself. Put manifest organization, file naming, folder purpose, and placement
  guidance here.
- `routing.rule.md`: the discovery map from task/domain to the ADR, reference,
  skill, persona, plan, learning, action, or rule that should be read. Put routing entries
  and context-loading discipline here.
- `terms.rule.md`: project-wide terminology and conversational agreements. Put
  canonical vocabulary definitions here, especially terms that must be used
  consistently across code, schemas, docs, CLI flags, and agent output. In an
  operation / data repo this is the **metric glossary** (one definition per
  metric).

Repos may add their own **domain rules** alongside the standard three — for
example `operation-secrets.rule.md` for credential and data-privacy handling in
an operation / data repo. Name them `<category>.rule.md` and route to them from
`routing.rule.md`.

### `personas/`

**Agent Personas** — role-specific AI workflows and constraints.

- **Format:** Markdown with YAML frontmatter.
- **Naming:** `<role>.agent.md`.
- **Content:** Role description, scope, priorities, tone, and constraints.
- **Purpose:** Focus an agent for specialized work.
- **Activation:** Users invoke with a persona tag, or another manifest document
  routes to the persona.

### `skills/`

**Specialized Task Automations** — runbooks for multi-step domain procedures.

- **Format:** Subdirectories containing at least `SKILL.md`.
- **Naming:** `<skill-name>/SKILL.md`.
- **Content:** Purpose, step-by-step procedure, decision trees, inputs, outputs,
  error handling, and supporting files.
- **Purpose:** Guide repeatable workflows that need more than a short rule.
- **Activation:** Users reference the skill by name, or tooling auto-invokes it
  from matching descriptions.

### `guides/`

**Operational How-To Guides** — tutorials and step-by-step instructions.

- **Format:** Markdown files.
- **Naming:** Descriptive `*.md` names.
- **Content:** Procedural instructions, troubleshooting steps, screenshots, and
  usage examples.
- **Purpose:** Help developers or operators use tools and commands.

### `plans/`

**Project Plans and Tracking** — multi-session plans and backlog items.

- **Format:** Markdown files.
- **Naming:** Descriptive `*.plan.md` names when possible.
- **Content:** Scope, milestones, tasks, status, checkboxes, dependencies, and
  progress notes.
- **Purpose:** Track work that spans multiple sessions and help agents resume
  where prior work stopped.

### `learning/`

**Teach Session Records** — durable records of `teach` skill sessions. Important: these are managed by the "teach"skill, not by the user or other skills. 

- **Format:** Markdown files.
- **Naming:** Dated, descriptive files such as `YYYY-MM-DD-topic.md`; example
  placeholder material may live under `example/`.
- **Content:** Session topic, teaching goal, explanations, examples, exercises,
  follow-up practice, and open questions.
- **Purpose:** Preserve what was taught so future agents and contributors can
  resume learning without re-deriving the session context.
- **Rule of thumb:** Put learning-session records here, not project decisions,
  subsystem mechanics, general scratch notes, or project backlog items.

### `findings/`

**Findings** — durable records of analyses and investigations: a specific
question, how it was answered, and the result. Useful for both repo types (data
analyses in an operation repo; investigation results such as a performance dig or
root-cause analysis in a software repo).

- **Format:** Markdown files.
- **Naming:** Dated, descriptive files such as `YYYY-MM-DD-topic.md`; example
  placeholder material may live under `example/`.
- **Content:** Question, sources and parameters (enough to reproduce), method,
  result, and caveats.
- **Purpose:** Make one-off analysis reproducible and reusable so the next
  person does not repeat the work.
- **Rule of thumb:** Put a question-and-answer-at-a-point-in-time here. Decisions
  go in `adr/`; how a subsystem or pipeline works goes in `reference/`.

### `actions/`

**Actions** — one-step or narrowly scoped procedures.

- **Format:** Markdown files.
- **Naming:** Descriptive `*.md` names.
- **Content:** Short instructions for a specific verification or maintenance
  action.
- **Purpose:** Provide agents with simple repeatable procedures without the
  ceremony of a full skill.

## Placement Rules

1. Put decisions in `adr/`.
2. Put current mechanics, inventories, and command behavior in `reference/`.
3. Put project-wide conventions, terminology, routing, and manifest placement
   rules in `rules/`.
4. Put repeatable multi-step workflows in `skills/`.
5. Put short one-step procedures in `actions/`.
6. Put long-lived progress tracking in `plans/`.
7. Put role behavior and task focus in `personas/`.
8. Put tutorials and operator how-to material in `guides/`.
9. Put records from `teach` skill sessions in `learning/`.
10. Put reproducible analysis and investigation records in `findings/`.

If content seems to belong in more than one place, keep the canonical rule or
decision in the most authoritative place and link to it from the operational
doc. Avoid duplicating canonical definitions across ADRs, references, and
skills.

## Discovery Flow

When a task is about manifest files:

1. Start from root `AGENTS.md`.
2. Read this rule to understand folder purpose and placement.
3. Read `routing.rule.md` to find task-specific ADRs, references, skills,
    personas, plans, learning, findings, actions, or rules.
4. Read `terms.rule.md` only when terminology or wording matters to the task.

For non-manifest coding work, do not bulk-read the manifest. Use routing only
when task context calls for a specific ADR, reference, skill, or rule.

## Cross-Linking Rules

- Prefer relative links between manifest documents.
- Link to the canonical document rather than copying long definitions.
- ADRs may point to references for mechanics.
- References may point to ADRs for authority and rules for terminology.
- Skills may point to ADRs, references, and rules needed by their procedure.
- Learning records may point to skills, guides, ADRs, references, and plans used
  during a teach session.
- Routing entries should point by path or filename, not repeat the target
  document's contents.

## Frontmatter Convention

Manifest files may use YAML frontmatter when tooling needs metadata:

```yaml
---
name: <identifier>
description: "<one-line description>"
applyTo: <glob pattern>
---
```

Use frontmatter only when a tool consumes it or when an established local pattern
requires it. Personas (`*.agent.md`) and skills (`SKILL.md`) usually need a
`name` and `description`; most other docs do not.

## Naming Suffix Cheat Sheet

| Folder        | Naming pattern        | Needs frontmatter?        |
| ------------- | --------------------- | ------------------------- |
| `adr/`        | `*.adr.md`            | optional                  |
| `reference/`  | `*.ref.md`            | optional                  |
| `rules/`      | `<category>.rule.md`  | no                        |
| `personas/`   | `<role>.agent.md`     | yes (`name`, `description`) |
| `skills/`     | `<skill>/SKILL.md`    | yes (`name`, `description`) |
| `guides/`     | descriptive `*.md`    | no                        |
| `plans/`      | `*.plan.md`           | optional (`todos` block)  |
| `learning/`   | `YYYY-MM-DD-topic.md` | optional                  |
| `findings/`   | `YYYY-MM-DD-topic.md` | optional                  |
| `actions/`    | descriptive `*.md`    | optional                  |

## Syncing Across Mirrors

When agent or IDE integrations need copies under `.github/`, `.cursor/`, or
other tool-specific folders, keep `.agents/` as the canonical source. Prefer
directory-level symlinks or generated mirrors over manually editing divergent
copies. See [`../guides/setup.md`](../guides/setup.md) for the linking steps.
