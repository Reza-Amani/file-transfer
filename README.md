# file-transfer

A simple file transfer system written in C.

## Overview

This project provides a **server** and a **client**. Clients connect to the server over the network and request files by name. The server reads the requested file from disk and sends it back to the client.

```
  +--------+                      +--------+
  | Client | ---- request file --> | Server |
  |   (C)  | <-- file contents --- |   (C)  |
  +--------+                      +--------+
```

## Components

| Component | Role |
|-----------|------|
| **Server** | Listens for connections, receives file requests, and serves files from a configured directory. |
| **Client** | Connects to the server, sends a file request, and writes the received data to disk (or stdout). |

## Status

Early development. Server and client implementations are forthcoming.

## Building

Requires a C compiler (`gcc` or compatible). On Windows, install [MSYS2](https://www.msys2.org/)
and `mingw-w64-x86_64-gcc` so `gcc` is on PATH.

**Build** from the repo root:

```powershell
.\build.ps1
```

**Run** the hello-world binary:

```powershell
.\build\file-transfer.exe
```

In Cursor / VS Code, use the **Build** and **Run** buttons in the status bar
(see [`.vscode/tasks.json`](.vscode/tasks.json)).

On macOS / Linux, use `make` instead of `build.ps1`.

See [`.agents/guides/commands.md`](.agents/guides/commands.md) for the full command reference.

## License

This project is licensed under the [MIT License](LICENSE).
