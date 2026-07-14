# Smoke test: transfer a known file and compare bytes.
# Usage (from repo root): powershell -ExecutionPolicy Bypass -File .\smoke-test.ps1
$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $Root

$Exe = Join-Path $Root "build\file-transfer.exe"
if (-not (Test-Path $Exe)) {
    Write-Host "Building first..."
    & powershell -ExecutionPolicy Bypass -File .\build.ps1
}

$Work = Join-Path $Root "build\smoke"
$Share = Join-Path $Work "share"
$Out = Join-Path $Work "out"
New-Item -ItemType Directory -Force -Path $Share, $Out | Out-Null

$Payload = Join-Path $Share "hello.bin"
# Mix of text and binary (including a zero byte) to exercise opaque transfer.
[byte[]]$Bytes = [System.Text.Encoding]::UTF8.GetBytes("hello smoke`n") + [byte[]](0, 1, 2, 255)
[System.IO.File]::WriteAllBytes($Payload, $Bytes)

# Empty file case
$Empty = Join-Path $Share "empty.dat"
[System.IO.File]::WriteAllBytes($Empty, @())

$Port = 19000
$Server = $null

try {
    $Server = Start-Process -FilePath $Exe `
        -ArgumentList @("server", "--port", "$Port", "--dir", $Share) `
        -PassThru -NoNewWindow `
        -RedirectStandardOutput (Join-Path $Work "server.out.log") `
        -RedirectStandardError (Join-Path $Work "server.err.log")

    Start-Sleep -Seconds 1
    if ($Server.HasExited) {
        throw "Server exited early (code $($Server.ExitCode)). See build\smoke\server.*.log"
    }

    $OutFile = Join-Path $Out "hello.bin"
    & $Exe client --host 127.0.0.1 --port $Port --file hello.bin --out $OutFile
    if ($LASTEXITCODE -ne 0) {
        throw "Client failed for hello.bin (exit $LASTEXITCODE)"
    }

    $Got = [System.IO.File]::ReadAllBytes($OutFile)
    if ($Got.Length -ne $Bytes.Length) {
        throw "Size mismatch: expected $($Bytes.Length), got $($Got.Length)"
    }
    for ($i = 0; $i -lt $Bytes.Length; $i++) {
        if ($Got[$i] -ne $Bytes[$i]) {
            throw "Byte mismatch at offset $i"
        }
    }

    $EmptyOut = Join-Path $Out "empty.dat"
    & $Exe client --host 127.0.0.1 --port $Port --file empty.dat --out $EmptyOut
    if ($LASTEXITCODE -ne 0) {
        throw "Client failed for empty.dat (exit $LASTEXITCODE)"
    }
    $EmptyGot = [System.IO.File]::ReadAllBytes($EmptyOut)
    if ($EmptyGot.Length -ne 0) {
        throw "Empty file should be 0 bytes, got $($EmptyGot.Length)"
    }

    # Illegal path should fail (client rejects locally before connect)
    $prevEap = $ErrorActionPreference
    $ErrorActionPreference = "Continue"
    & $Exe client --host 127.0.0.1 --port $Port --file "..\secret" --out (Join-Path $Out "bad.bin") 2>$null
    $illegalExit = $LASTEXITCODE
    $ErrorActionPreference = $prevEap
    if ($illegalExit -eq 0) {
        throw "Expected non-zero exit for illegal filename"
    }

    Write-Host "Smoke test passed."
}
finally {
    if ($null -ne $Server -and -not $Server.HasExited) {
        Stop-Process -Id $Server.Id -Force -ErrorAction SilentlyContinue
    }
}
