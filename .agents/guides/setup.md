# Setup Guide

How to prepare a **Windows** machine to build and run **file-transfer**, and how
to wire the agent manifest into your tools.

For day-to-day build, run, and two-machine examples after setup, see
[`commands.md`](commands.md).

## What you need

| Tool | Purpose |
|------|---------|
| **Git** | Clone the repository |
| **C compiler** (`gcc` via MSYS2 MinGW) | Compile the program |
| **PowerShell** | Run [`build.ps1`](../../build.ps1) |
| **Cursor / VS Code** (optional) | Status-bar Build and Run tasks |

The project is **C11** and targets **Windows with Winsock**. The canonical build
path is [`build.ps1`](../../build.ps1). A POSIX/Linux port is not in scope yet
(see [ADR-0001](../adr/0001-c-client-server-architecture.adr.md)).

---

## 1. Install Git

Download and run the installer from [git-scm.com](https://git-scm.com/download/win).
Accept the defaults unless you have a preference for the default editor or PATH
handling.

Verify in a new PowerShell window:

```powershell
git --version
```

---

## 2. Clone the repository

```powershell
git clone <repository-url> file-transfer
cd file-transfer
```

Replace `<repository-url>` with the remote you use (HTTPS or SSH).

---

## 3. Install a C toolchain (MSYS2 + MinGW GCC)

1. **Install MSYS2** from [msys2.org](https://www.msys2.org/). Use the default
   install location (`C:\msys64`).

2. **Open the MSYS2 terminal** (Start menu → “MSYS2 MSYS”) and update:

   ```bash
   pacman -Syu
   ```

   If the installer asks you to close the terminal, do so, reopen **MSYS2 MSYS**,
   and run `pacman -Syu` again until it reports nothing left to update.

3. **Install the MinGW GCC toolchain**:

   ```bash
   pacman -S --needed mingw-w64-x86_64-gcc
   ```

4. **Add GCC to your PATH**:

   - Open **Settings → System → About → Advanced system settings → Environment
     Variables**.
   - Under **User variables**, edit **Path** and add:

     ```
     C:\msys64\mingw64\bin
     ```

   - Close and reopen terminals (and restart Cursor/VS Code if it was running).

5. **Verify** in a new PowerShell window:

   ```powershell
   gcc --version
   where.exe gcc
   ```

   `where.exe` should list `C:\msys64\mingw64\bin\gcc.exe`.

> You do not need `make` on Windows. Use `.\build.ps1` from the repo root.

---

## 4. Build (smoke check)

From the repository root:

```powershell
.\build.ps1
```

If PowerShell blocks the script:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

Cursor and VS Code tasks already pass `-ExecutionPolicy Bypass` (see
[`.vscode/tasks.json`](../../.vscode/tasks.json)).

After a successful build, run examples from [`commands.md`](commands.md). An
optional end-to-end check is `smoke-test.ps1` at the repo root.

---

## 5. IDE integration (optional)

Open the repository folder in **Cursor** or **VS Code**.

- **Build** — status bar **Build** button or `Ctrl+Shift+B`
- **Run** — status bar **Run** button (builds first, then runs the binary)

Both tasks are defined in [`.vscode/tasks.json`](../../.vscode/tasks.json).

---

## 6. Wire the manifest into your AI tools

`.agents/` is the **canonical source**. Mirror into tool folders rather than
maintaining separate copies.

### Option A — Symbolic links (recommended)

On Windows (PowerShell, run as Administrator from the repo root):

```powershell
New-Item -ItemType Directory -Force -Path .\.cursor
New-Item -ItemType SymbolicLink -Path .\.cursor\skills -Target .agents\skills
```

`.cursor/` is listed in [`.gitignore`](../../.gitignore) — it is recreated
locally per machine and not committed. Canonical skills live in `.agents/skills/`.

### Option B — Generated mirror

If symlinks are not viable, copy `.agents/skills` and `.agents/personas` with a
small script. Never hand-edit mirrors.

---

## 7. Verify

**Build environment**

- [ ] `gcc --version` succeeds in a new terminal
- [ ] `.\build.ps1` completes without errors
- [ ] `build\file-transfer.exe` exists

**Agent manifest** (if you use AI tools in this repo)

- [ ] Your agent can read root [`AGENTS.md`](../../AGENTS.md)
- [ ] Ask: “Read the routing rule and tell me which doc covers the server” —
      you should get [`client-server.ref.md`](../reference/client-server.ref.md)
      or [`design.ref.md`](../reference/design.ref.md)

---

## Optional: upgrade the manifest from the template

This repo was bootstrapped from a manifest-template pattern. To pull structural
updates from that template into an existing manifest, use the template's
`sync-manifest-template` skill from a workspace that contains both repos.
