# Useful Commands

Quick reference for build, test, run, and CLI commands.

> When commands change, update this file in the same change. Run
> [`actions/verify-docs-in-sync.md`](../actions/verify-docs-in-sync.md) after
> changing flags or binaries.

Platform: **Windows** (Winsock). Canonical build is [`build.ps1`](../../build.ps1).

## Build

From the repo root:

```powershell
.\build.ps1
```

In Cursor / VS Code, use the **Build** button in the status bar or
`Terminal → Run Build Task` (`Ctrl+Shift+B`). Both run `build.ps1` via
[`.vscode/tasks.json`](../../.vscode/tasks.json).

Output: `build/file-transfer.exe`

If PowerShell blocks the script:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

## Run — same machine

Serve a folder, then fetch a file (two terminals):

```powershell
# Terminal 1 — server
.\build\file-transfer.exe server --port 9000 --dir C:\share

# Terminal 2 — client
.\build\file-transfer.exe client --host 127.0.0.1 --port 9000 --file report.pdf --out .\report.pdf
```

Replace `C:\share` and `report.pdf` with a real directory and bare filename that
exists under that directory.

### CLI reference

**Server**

```text
file-transfer server --port <port> --dir <serve-root>
```

| Flag | Required | Meaning |
|------|----------|---------|
| `--port` | no (default 9000) | Listen port |
| `--dir` | yes | Serve root directory |

**Client**

```text
file-transfer client --host <addr> --port <port> --file <name> --out <path>
```

| Flag | Required | Meaning |
|------|----------|---------|
| `--host` | yes | Server address |
| `--port` | no (default 9000) | Server port |
| `--file` | yes | Bare filename to request |
| `--out` | yes | Local path to write |

## Run — two machines

1. On the **server** machine, allow inbound TCP on the chosen port (default 9000)
   and note that machine's LAN IP (e.g. `192.168.1.10`).
2. Start the server with a shared folder:

   ```powershell
   .\build\file-transfer.exe server --port 9000 --dir C:\share
   ```

3. On the **client** machine (outbound TCP is enough):

   ```powershell
   .\build\file-transfer.exe client --host 192.168.1.10 --port 9000 --file report.pdf --out .\report.pdf
   ```

The client opens the connection; the server only needs to accept inbound
connections.

## Test

Manual demo: use the same-machine or two-machine examples above and confirm the
output file matches the source (size and content).

Optional automated smoke test (starts a temporary server, transfers a known
binary file and an empty file, rejects an illegal name):

```powershell
powershell -ExecutionPolicy Bypass -File .\smoke-test.ps1
```

## Clean

```powershell
Remove-Item -Recurse -Force build
```

## Design and setup

- Design and assumptions → [`../reference/design.ref.md`](../reference/design.ref.md)
- First-time toolchain install → [`setup.md`](setup.md)
