# Learning

Records of `teach` skill sessions. Use this folder to preserve what was taught,
what examples were used, and what follow-up practice or open questions remain.

## The `teach` skill

These records pair with Matt Pocock's `teach` skill, a stateful, multi-session
teaching workflow. See it on [skills.sh](https://www.skills.sh/mattpocock/skills/teach).

### Download / install

Install it from the `mattpocock/skills` repository with the `skills` CLI:

```sh
npx skills add https://github.com/mattpocock/skills --skill teach
```

To install every skill in the repository instead, drop the `--skill` flag:

```sh
npx skills add mattpocock/skills
```

### Invoke it

The skill is stateful — it treats the current directory as a teaching workspace
and learns across sessions. To start, open this folder (or a topic subfolder)
and ask your agent to teach you the topic, for example:

> Use the `teach` skill to teach me Berkeley sockets in C.

The skill then maintains its own files in the workspace (such as `MISSION.md`,
`RESOURCES.md`, `learning-records/`, and `lessons/`). Keep those alongside the
summary records described below.

## Naming

Use dated, descriptive Markdown files:

- `YYYY-MM-DD-topic.md`

Do not put real teach session records under a generic `example/` folder unless
you are intentionally keeping template-only material.

## What Belongs Here

- The topic and goal of a teach session.
- Key explanations, examples, and exercises used in the session.
- Follow-up prompts, practice tasks, or questions to revisit.

## What Does Not Belong Here

- Project decisions; put those in [`../adr/`](../adr/).
- Current subsystem mechanics; put those in [`../reference/`](../reference/).
- Multi-session project work; put that in [`../plans/`](../plans/).
- Temporary discoveries, analysis, or investigation results; put those in [`../findings/`](../findings/).
- General scratch notes; put those in [`../guides/notes.md`](../guides/notes.md).
