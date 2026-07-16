---
name: grill-to-spec
description: >-
  Relentlessly interviews the user about a plan or design (grilling), sharpening
  domain terminology as it goes, and — once shared understanding is reached —
  writes a spec file to `.agents/plans/<topic>.spec.md` as the starting point
  for spec-driven development. Afterwards, lists any manifest files (terms.rule.md,
  adr/, reference/, guides/) made stale by the decisions and asks the user's
  permission before updating them. Use when the user asks to "grill me into a
  spec", "grill this plan", "stress-test this design and write a spec", or wants
  a spec-driven starting point for a task.
---

Adapted from [mattpocock/skills](https://github.com/mattpocock/skills) — see
[`THIRD_PARTY_NOTICES.md`](../../../THIRD_PARTY_NOTICES.md).

# Grill to Spec

Interview the user relentlessly about a plan or design until shared
understanding is reached, then capture that understanding as a **spec** the
user can plan and implement from. This is the grilling loop plus active domain
modeling, with two deliverables at the end: a spec file, and (with permission)
manifest updates.

## Inputs you must resolve first

- **Target repo:** this repository. The spec and any manifest updates go here.
- If `manifest-template` is also open in the workspace, treat it as read-only
  and never write there.

If the workspace is ambiguous (multiple non-template repos), ask the user
which folder is the target before writing anything.

> **Safety:** All writes (spec, manifest edits) go to the **target** repo only.

## Phase 1: Grilling

Interview the user relentlessly about every aspect of the plan until you reach
a shared understanding. Walk down each branch of the design tree, resolving
dependencies between decisions one-by-one. For each question, provide your
recommended answer.

Ask the questions **one at a time**, waiting for feedback on each question
before continuing.

If a *fact* can be found by exploring the target repo, look it up rather than
asking. The *decisions*, though, are the user's — put each one to them and wait
for their answer.

Do not write the spec, and do not enact the plan, until the user confirms you
have reached a shared understanding. Do not implement any of the decisions until the spec is written. Even then, recommend the user to plan and implement the spec in a separate chat.

## Phase 2: Domain modeling during the session

While grilling, actively sharpen the target repo's domain model. The canonical
glossary in a manifest repo is `.agents/rules/terms.rule.md`; decisions live in
`.agents/adr/*.adr.md`.

- **Challenge against the terms.** If the target has a `terms.rule.md` and
the user uses a term that conflicts with it, call it out: example: "Your
terms file defines 'adoption' as X, but you seem to mean Y — which is your intent?"
- **Sharpen fuzzy language.** When the user uses vague or overloaded terms,
propose a precise canonical term. "You're saying 'target' — do you mean the
product or the build option?"
- **Discuss concrete scenarios.** Stress-test domain relationships with
specific edge-case scenarios that force precision about boundaries between
concepts.
- **Cross-reference with code.** When the user states how something works,
check whether the target repo's code agrees. Surface contradictions.

Do **not** edit manifest files during this phase. Instead, keep a running list
of manifest files that the session's decisions make stale (see Phase 4).

## Phase 3: Write the spec

When the user confirms shared understanding is reached — or asks to wrap up —
write the spec to the **target repo** at:

```
.agents/plans/<topic>.spec.md
```

Use a short, descriptive dash-case `<topic>` (e.g. `partial-refunds.spec.md`).
Create `.agents/plans/` if it does not exist. Never place the spec anywhere
else.

The spec is the starting point of the user's task: complete enough to plan and
implement from without re-deriving the conversation. Structure:

```markdown
# <Title> Spec

> Produced by a grilling session on YYYY-MM-DD. Status: ready for planning.

## Goal
What is being built or changed, and why. One or two paragraphs.

## Decisions
The resolved design decisions from the session, each with its rationale.
This is the heart of the spec — one entry per branch of the design tree.

## Scope
What is in scope, and — just as important — what was explicitly ruled out.

## Details
Details of the task intended spec.

## Constraints
Mandates, invariants, and hard limits the implementation must respect; only those that are relevant to the spec, but absent in the ADRs and rules.

## Open questions
Anything deliberately deferred, with who/what resolves it.

```

Omit sections that would be empty; add sections the topic demands. Link to the
target's manifest documents (ADRs, references, rules) instead of duplicating
their content.

If the session ends **without** reaching shared understanding, do not write a
spec. Tell the user which branches remain unresolved.

## Phase 4: Offer manifest updates (permission-gated)

After the spec is written, check whether the session's decisions made any manifest files in the target repo stale. Follow the placement rules in target repo's manifest structure (.agents/rules/manifest-structure.rule.md if present)


Candidates to check (only if the file/directory already exists in the target):

- `.agents/rules/terms.rule.md` — terms coined or sharpened during the session.
- `.agents/adr/` — a decision from the session that is hard to reverse,
surprising without context, and the result of a real trade-off. If any of the
three is missing, skip the ADR.
- `.agents/reference/` — mechanics described in the session that contradict or
extend an existing `*.ref.md`.
- `.agents/guides/` — procedures or commands the session changed.

Then:

1. **List** the files that need an update and what would change in each, in one
  short summary.
2. **Ask the user's permission** before editing any of them.
3. Update **only** the files the user approves, following the target's naming
  suffixes (`*.adr.md`, `*.ref.md`, `<category>.rule.md`) and cross-linking
  conventions.

## Decision tree

```
Workspace folders?
├─ This repo only .................. target = this repo
├─ This repo + manifest-template ... target = this repo (template read-only)
└─ Ambiguous ....................... ASK which folder is the target

Shared understanding confirmed?
├─ No .............................. keep grilling; no spec, no edits
└─ Yes ............................. write .agents/plans/<topic>.spec.md

Stale manifest files found in target?
├─ No .............................. report "manifest untouched" and stop
└─ Yes ............................. list them → ask permission → update
                                     only the approved ones
```

## Completion checks

- Questions were asked one at a time; facts came from the repo, decisions from
the user.
- The spec exists at `<target>/.agents/plans/<topic>.spec.md` and is complete
enough to plan from — or the user was told why no spec was written.
- No manifest file was edited without the user's explicit approval.
- Nothing was written into `manifest-template` when it is open alongside this repo.
