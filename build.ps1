# Canonical build on Windows. See .agents/guides/commands.md
$ErrorActionPreference = "Stop"

$BuildDir = "build"
$Target = Join-Path $BuildDir "file-transfer.exe"

$Sources = @(
    "src/main.c",
    "src/server/server.c",
    "src/client/client.c",
    "src/common/protocol.c",
    "src/common/net.c",
    "src/common/path.c"
)

if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

& gcc -std=c11 -Wall -Wextra -pedantic -O2 `
    -o $Target `
    @Sources `
    -lws2_32

if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE"
}

Write-Host "Built $Target"
