# Useful Commands

Quick reference for build, test, run, and CLI commands.

> When commands change, update this file in the same change. Run
> [`actions/verify-docs-in-sync.md`](../actions/verify-docs-in-sync.md) after
> changing flags or binaries.

## Build

With `make` (MSYS2/MinGW, macOS, Linux):

```bash
make
```

Without `make` on Windows (uses `gcc` from PATH):

```powershell
.\build.ps1
```

Output: `build/file-transfer` (or `build/file-transfer.exe` on Windows).

## Test

```bash
# TBD — e.g. make test
```

## Run locally

Hello-world smoke test (current `main`):

```bash
make run
```

```powershell
.\build.ps1
.\build\file-transfer.exe
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

```bash
make clean
```

Removes the `build/` directory.
