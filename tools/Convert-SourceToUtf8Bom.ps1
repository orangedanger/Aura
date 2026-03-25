param(
    [string]$Root = "Source",
    [switch]$Apply
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$utf8Strict = New-Object System.Text.UTF8Encoding($false, $true)
$utf8Bom = New-Object System.Text.UTF8Encoding($true)
$gbk = [System.Text.Encoding]::GetEncoding(936)

function Test-Utf8NoBom {
    param([byte[]]$Bytes)
    try {
        [void]$utf8Strict.GetString($Bytes)
        return $true
    } catch {
        return $false
    }
}

function Get-TextWithFallback {
    param([byte[]]$Bytes)

    if ($Bytes.Length -ge 3 -and $Bytes[0] -eq 0xEF -and $Bytes[1] -eq 0xBB -and $Bytes[2] -eq 0xBF) {
        return [System.Text.Encoding]::UTF8.GetString($Bytes, 3, $Bytes.Length - 3)
    }

    if (Test-Utf8NoBom -Bytes $Bytes) {
        return [System.Text.Encoding]::UTF8.GetString($Bytes)
    }

    return $gbk.GetString($Bytes)
}

if (-not (Test-Path -LiteralPath $Root)) {
    throw "Path not found: $Root"
}

$targets = Get-ChildItem -LiteralPath $Root -Recurse -File -Include *.h, *.hpp, *.cpp, *.c, *.inl, *.cs
$changed = 0

foreach ($file in $targets) {
    $originalBytes = [System.IO.File]::ReadAllBytes($file.FullName)
    $text = Get-TextWithFallback -Bytes $originalBytes
    $newBytes = $utf8Bom.GetBytes($text)

    $isDifferent = $originalBytes.Length -ne $newBytes.Length
    if (-not $isDifferent) {
        for ($i = 0; $i -lt $originalBytes.Length; $i++) {
            if ($originalBytes[$i] -ne $newBytes[$i]) {
                $isDifferent = $true
                break
            }
        }
    }

    if ($isDifferent) {
        $changed++
        if ($Apply) {
            [System.IO.File]::WriteAllBytes($file.FullName, $newBytes)
            Write-Host "[UPDATED] $($file.FullName)"
        } else {
            Write-Host "[PLAN]    $($file.FullName)"
        }
    }
}

if ($Apply) {
    Write-Host "Done. Updated files: $changed"
} else {
    Write-Host "Dry run done. Files to update: $changed"
    Write-Host "Run with -Apply to write changes."
}
