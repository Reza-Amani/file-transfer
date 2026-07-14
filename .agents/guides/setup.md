# Setup Guide

How to prepare a machine for **file-transfer** development and wire the agent
manifest into your tools.

## 1. Install a C toolchain

You need a C compiler. On Windows the project builds with [`build.ps1`](../../build.ps1)
(no `make` required).

### Windows

Install **MSYS2** and the MinGW GCC package:

```powershell
pacman -S mingw-w64-x86_64-gcc
```

Ensure `C:\msys64\mingw64\bin` is on PATH (or open a MinGW64 shell).

Verify:

```powershell
gcc --version
```

### macOS

```bash
xcode-select --install
```

### Linux

```bash
# Debian/Ubuntu
sudo apt install build-essential

# Fedora
sudo dnf install gcc make
```

## 2. Clone and build

From the repo root:

```powershell
.\build.ps1
.\build\file-transfer.exe
```

In Cursor / VS Code, use the **Build** status bar button or `Ctrl+Shift+B`
(see [`.vscode/tasks.json`](../../.vscode/tasks.json)).

Full commands: [`commands.md`](commands.md).

## 3. Wire the manifest into your AI tools

`.agents/` is the **canonical source**. Mirror into tool folders rather than
maintaining separate copies.

### Option A — Symbolic links (recommended)

On Windows (PowerShell, run as Administrator from the repo root):

```powershell
# Create .cursor/ if it does not exist yet (mkdir -p equivalent).
# -Force: no error if the folder already exists.
# The next command needs this parent folder before it can place a symlink inside it.
New-Item -ItemType Directory -Force -Path .\.cursor

# Point Cursor at the canonical skills folder (requires Admin on Windows).
New-Item -ItemType SymbolicLink -Path .\.cursor\skills -Target .agents\skills
```

If `.cursor` already exists, the first line is a no-op. You can skip it and run
only the symlink line.

`.cursor/` is listed in [`.gitignore`](../../.gitignore) — it is recreated
locally per machine and not committed. Canonical skills live in `.agents/skills/`.

On macOS/Linux:

```bash
mkdir -p .cursor
ln -s ../.agents/skills .cursor/skills
```

Add `.github/agents` → `.agents/personas` the same way if you use GitHub
Copilot agents.

### Option B — Generated mirror

If symlinks are not viable in your environment, copy `.agents/skills` and
`.agents/personas` with a small script or CI step. Never hand-edit mirrors.

## 4. Verify

- Confirm your agent can read root [`AGENTS.md`](../../AGENTS.md).
- Ask: "Read the routing rule and tell me which doc covers the server" — you
  should get [`client-server.ref.md`](../reference/client-server.ref.md).

## Optional: upgrade the manifest from the template

This repo was bootstrapped from the [manifest-template](https://github.com/)
pattern. To pull structural updates from that template into an existing manifest,
use the template's `sync-manifest-template` skill from a workspace that contains
both repos.
