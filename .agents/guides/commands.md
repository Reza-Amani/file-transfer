# Useful Commands

Quick reference for build, test, run, and CLI commands.

> When commands change, update this file in the same change. Run
> [`actions/verify-docs-in-sync.md`](../actions/verify-docs-in-sync.md) after
> changing flags or binaries.

## Build

**Windows (canonical):** run [`build.ps1`](../../build.ps1) from the repo root.

```powershell
.\build.ps1
```

In Cursor / VS Code, use the **Build** button in the status bar or
`Terminal → Run Build Task` (`Ctrl+Shift+B`). Both run `build.ps1` via
[`.vscode/tasks.json`](../../.vscode/tasks.json).

Output: `build/file-transfer.exe`

**macOS / Linux:** use the Makefile when `make` is available:

```bash
make
```

Output: `build/file-transfer`

## Test

```bash
# TBD — e.g. make test
```

## Run locally

Hello-world smoke test (current `main`):

```powershell
.\build.ps1
.\build\file-transfer.exe
```

Or use the **Run** status bar task (builds first, then runs the binary).

```bash
make run
```

### Server mode (planned)

```bash
# TBD — e.g. ./build/file-transfer server --root /path/to/files --listen :8080
```

### Client mode (planned)

```bash
# TBD — e.g. ./build/file-transfer client --host localhost:8080 --file report.pdf --output ./report.pdf
# TBD — e.g. ./build/file-transfer client --host localhost:8080 --file readme.txt   # stdout
```

## Clean

Delete the `build/` directory:

```powershell
Remove-Item -Recurse -Force build
```

```bash
make clean
```
