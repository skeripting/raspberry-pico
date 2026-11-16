param([Parameter(Mandatory = $true)][string]$Firmware)
$ErrorActionPreference = 'Stop'
if (-not (Test-Path -LiteralPath $Firmware -PathType Leaf)) {
    throw "Firmware not found: $Firmware. Compile Project first."
}
$boards = @(Get-PSDrive -PSProvider FileSystem | Where-Object {
    $info = Join-Path $_.Root 'INFO_UF2.TXT'
    (Test-Path -LiteralPath $info -ErrorAction SilentlyContinue) -and
    ((Get-Content -LiteralPath $info -Raw) -match 'Board-ID: RPI-RP2')
})
if ($boards.Count -eq 0) {
    throw 'No RP2040 BOOTSEL drive found. Hold BOOTSEL while reconnecting the Pico, release it, then run again.'
}
if ($boards.Count -ne 1) {
    throw 'More than one RP2040 BOOTSEL drive found. Connect only the Pico you want to program.'
}
$destination = Join-Path $boards[0].Root (Split-Path $Firmware -Leaf)
Copy-Item -LiteralPath $Firmware -Destination $destination -ErrorAction Stop
Write-Host 'UF2 copied successfully. The Pico will reboot and run the program.'
