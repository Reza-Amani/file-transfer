# Notes

Scratch space for durable notes that do not yet belong in an ADR or reference.
Promote decisions to [`../adr/`](../adr/) and mechanics to
[`../reference/`](../reference/).

## Open questions

- **2026-07-14** — Wire protocol: length-prefixed messages vs simple
  request-line + raw bytes? Needs an ADR before implementation.
- **2026-07-14** — Build system: plain `Makefile` vs CMake? Decide when adding
  first source files.
- **2026-07-14** — Server concurrency model: threads vs process-per-connection vs
  event loop?
- **2026-07-14** — CLI shape: `file-transfer server …` / `file-transfer client …`
  vs flags?

## Settled direction (until ADRs Accepted)

- One binary, server and client as CLI modes.
- One connection per client invocation; server handles many concurrent connections.

## Operational notes

_(none yet)_
