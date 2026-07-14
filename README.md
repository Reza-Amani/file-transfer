# file-transfer

A simple file transfer system written in C for **Windows** (Winsock).

## Overview

One binary, two modes. Clients connect over TCP, request a file by bare name, and save it locally. The server serves files from a single directory and handles
multiple connections at once (one thread per connection).

```text
  +--------+                      +--------+
  | Client | ---- GET file -----> | Server |
  |  mode  | <-- file bytes ----- |  mode  |
  +--------+                      +--------+
```

## Assumptions:
- One binary, two modes.
- Windows only.
- Server and client are on the same network.
- Security: Only check serve root. No encryption, no authentication, no access control. No checking for OS-special names.
- Error handling:
  - If Server is not running or reachable, client prints out and returns an error.
  - If Server drops mid transfer, the client displays and returns an error AND remove the partially received file
- Network bandwidth: No app-level ACK/NACK, no compression. Submission by chuncks rather than whole file. Use TCP ability to handle loss and retransmission

## Documents:
[design](.agents/reference/design.ref.md).
[spec](.agents/plans/file-transfer.spec.md).
[Toolchain Installation](.agents/guides/setup.md)

## Usage
**Build**

```powershell
.\build.ps1
```

**Same machine** (two terminals)
```powershell
.\build\file-transfer.exe server --port 9000 --dir C:\share
.\build\file-transfer.exe client --host 127.0.0.1 --port 9000 --file report.pdf --out .\report.pdf
```

**Two machines:**
Use the actual IP address of the server machine.
[commands and usage](.agents/guides/commands.md).

## More info
For effective use of AI agents, this repo uses an agent manifest bootstrapped from open-sourced
[manifest-template](https://github.com/Reza-Amani/manifest-template).
Routing and placement rules live in [routing](.agents/rules/routing.rule.md) and [manifest](.agents/rules/manifest-structure.rule.md).
Those files are intended for agents.

## License

This project is licensed under the [MIT License](LICENSE).
