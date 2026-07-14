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

Build instructions will be added once the source layout is in place.

## License

This project is licensed under the [MIT License](LICENSE).
