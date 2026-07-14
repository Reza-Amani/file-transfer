# Canonical build on Windows. See .agents/guides/commands.md
$ErrorActionPreference = "Stop"

$BuildDir = "build"
$Target = Join-Path $BuildDir "file-transfer.exe"
$Source = "src/main.c"

if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

& gcc -std=c11 -Wall -Wextra -pedantic -o $Target $Source
Write-Host "Built $Target"
