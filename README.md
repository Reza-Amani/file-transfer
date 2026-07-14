# file-transfer

A simple file transfer system written in C for **Windows** (Winsock).

## Overview

One binary, two modes. Clients connect over TCP, request a file by bare name,
and save it locally. The server serves files from a single directory and handles
multiple connections at once (one thread per connection).

```text
  +--------+                      +--------+
  | Client | ---- GET file -----> | Server |
  |  mode  | <-- file bytes ----- |  mode  |
  +--------+                      +--------+
```

Design and assumptions: [`.agents/reference/design.ref.md`](.agents/reference/design.ref.md).

## Status

Early development. Architecture and protocol are specified; implementation of
server/client modes is forthcoming. See
[`.agents/plans/file-transfer.spec.md`](.agents/plans/file-transfer.spec.md).

## Quick start

**Install toolchain** (Git, MSYS2 MinGW `gcc`):  
[`.agents/guides/setup.md`](.agents/guides/setup.md)

**Build**

```powershell
.\build.ps1
```

**Same machine** (two terminals; after server/client are implemented)

```powershell
.\build\file-transfer.exe server --port 9000 --dir C:\share
.\build\file-transfer.exe client --host 127.0.0.1 --port 9000 --file report.pdf --out .\report.pdf
```

**Two machines:** run the server on the host that allows inbound TCP; point the
client at that host’s LAN IP. Full flag reference and two-laptop walkthrough:
[`.agents/guides/commands.md`](.agents/guides/commands.md).

In Cursor / VS Code, use the **Build** and **Run** buttons in the status bar
(see [`.vscode/tasks.json`](.vscode/tasks.json)).

## Components

| Mode | Role |
|------|------|
| **Server** | Listen on TCP, accept concurrent clients, serve bare filenames from `--dir` |
| **Client** | Connect once, request `--file`, write `--out`, exit |

## Docs map

| Doc | Contents |
|-----|----------|
| [setup.md](.agents/guides/setup.md) | Install Git, GCC, first build |
| [commands.md](.agents/guides/commands.md) | Build, CLI, same-machine and two-machine examples |
| [design.ref.md](.agents/reference/design.ref.md) | Design overview and assumptions |
| [file-transfer.spec.md](.agents/plans/file-transfer.spec.md) | Implementation spec |

For contributors using AI agents in this repo: routing and placement rules live
in [`.agents/rules/routing.rule.md`](.agents/rules/routing.rule.md) and
[`.agents/rules/manifest-structure.rule.md`](.agents/rules/manifest-structure.rule.md).
Those files are intended for agents, not required reading to build or run the
program.

## License

This project is licensed under the [MIT License](LICENSE).
