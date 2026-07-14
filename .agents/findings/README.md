# Findings

Durable records of **discoveries, analyses and investigations** — what question was asked (if any),
how it was answered, and what the result was. Findings make one-off analysis
work reproducible and reusable so the next person (human or agent) does not
repeat the query from scratch.

> Findings are **not** decisions and **not** mechanics. A decision goes in
> [`../adr/`](../adr/); how a subsystem works goes in
> [`../reference/`](../reference/). A finding records a specific question and its
> answer at a point in time.

## Naming

Use dated, descriptive Markdown files:

- `YYYY-MM-DD-topic.md`

## What belongs in a finding

- **Question:** what was asked and why.
- **Sources & parameters:** enough context to **reproduce** the result.
- **Method:** the steps or experiment used.
- **Result:** the outcome and what it means.
- **Caveats:** assumptions and open questions.

## What does not belong here

- Decisions → [`../adr/`](../adr/).
- Subsystem mechanics → [`../reference/`](../reference/).
- Multi-session project work → [`../plans/`](../plans/).
- `teach` session records → [`../learning/`](../learning/).

## Promote durable insights

If a finding leads to a lasting decision, write an ADR and link back. If it
documents how the protocol works, move that into a reference doc.
