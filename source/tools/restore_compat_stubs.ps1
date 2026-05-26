# Re-inserts the COMPAT STUBS block into the EEZ-regenerated screens.h.
#
# EEZ Studio rewrites screens.h every time you regenerate, which wipes
# the hand-maintained block of widget declarations that display.c /
# display_tonex.c reference but the current EEZ design doesn't supply.
# screens_compat.c creates hidden 0x0 placeholders for these at runtime,
# but their fields must exist in objects_t for the C code to compile.
#
# Usage (from any directory): powershell -File tools\restore_compat_stubs.ps1
# Or from the source root:    .\tools\restore_compat_stubs.ps1
#
# Idempotent — running multiple times is safe; only inserts if missing.
# Edit $StubLines below if the list of widgets needing stubs changes.

$ErrorActionPreference = 'Stop'

$ScriptDir  = Split-Path -Parent $MyInvocation.MyCommand.Path
$SourceRoot = Split-Path -Parent $ScriptDir
$Target     = Join-Path $SourceRoot 'main\ui_generated_480x320land\screens.h'

if (-not (Test-Path $Target)) {
    Write-Error "screens.h not found at: $Target"
    exit 1
}

$Marker = '/* ===== COMPAT STUBS ====='
$Closer = '} objects_t;'

$StubLines = @(
    '',
    '    /* ===== COMPAT STUBS =====',
    '       Widgets used by display.c / display_tonex.c but not produced by the',
    '       current EEZ design. screens_compat_init() (screens_compat.c) creates',
    '       hidden 0x0 placeholders for these at startup so the runtime code can',
    '       safely call LVGL APIs on real objects instead of NULL.',
    '       Hand-maintained — EEZ regen wipes this block; re-add after regenerating.',
    '       Run tools/restore_compat_stubs.ps1 to re-insert automatically. */',
    '    lv_obj_t *ui_skin_image;',
    '    lv_obj_t *ui_preset_details_text_area;',
    '    lv_obj_t *ui_entry_keyboard;',
    '    lv_obj_t *ui_left_arrow;',
    '    lv_obj_t *ui_right_arrow;',
    '    lv_obj_t *ui_ok_tick;',
    '    lv_obj_t *ui_chip_gate;',
    '    lv_obj_t *ui_chip_comp;',
    '    lv_obj_t *ui_chip_eq;',
    '    lv_obj_t *ui_chip_amp;',
    '    lv_obj_t *ui_chip_cab;',
    '    lv_obj_t *ui_chip_mod;',
    '    lv_obj_t *ui_chip_delay;',
    '    lv_obj_t *ui_chip_reverb;',
    '    /* ===== END COMPAT STUBS ===== */'
)

$content = Get-Content $Target -Raw

if ($content -match [regex]::Escape($Marker)) {
    Write-Host "COMPAT STUBS block already present in screens.h - nothing to do."
    exit 0
}

$lines = Get-Content $Target
$closerIdx = -1
for ($i = 0; $i -lt $lines.Length; $i++) {
    if ($lines[$i].Trim() -eq $Closer) {
        $closerIdx = $i
        break
    }
}

if ($closerIdx -lt 0) {
    Write-Error "Could not find '$Closer' line in screens.h - did EEZ change the file format?"
    exit 1
}

$before = $lines[0..($closerIdx - 1)]
$after  = $lines[$closerIdx..($lines.Length - 1)]
$new    = $before + $StubLines + $after

Set-Content -Path $Target -Value $new -Encoding UTF8
Write-Host "Inserted COMPAT STUBS block before line $($closerIdx + 1) in screens.h."
