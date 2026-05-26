# Re-applies the three patches EEZ Studio's Build wipes from
# source/main/ui_generated_480x320land/. Run from anywhere -- the script
# resolves paths relative to its own location.
#
# Idempotent: safe to run multiple times. Skips patches already in place.
#
# Patches:
#   1. screens.h  -- restores the COMPAT STUBS block (ui_chip_*, ui_skin_image,
#                   etc.) the firmware's display.c / display_tonex.c need
#   2. screens.c  -- adds #include <stdio.h> (needed by patch 3's snprintf)
#   3. screens.c  -- rewrites the broken `const char *new_val = get_var_bpm_gauge();`
#                   block in tick_screen_settings() to convert the int to text.
#                   Works regardless of what the label widget is named in EEZ
#                   (auto-name like `obj10`, or any custom name).
#   4. screens.h  -- dedups screen_settings_state_t when multiple meters share
#                   the same scale/needle names (default `Test`/`Knob`).
#                   Durable fix: rename each meter's scale + needle in EEZ.
#
# Usage:
#   ./post_eez_patch.ps1                # apply patches
#   ./post_eez_patch.ps1 -CheckOnly     # report status, change nothing

[CmdletBinding()]
param(
    [switch]$CheckOnly
)

$ErrorActionPreference = 'Stop'

$uiDir     = Join-Path $PSScriptRoot '..\source\main\ui_generated_480x320land'
$screensH  = Join-Path $uiDir 'screens.h'
$screensC  = Join-Path $uiDir 'screens.c'

if (-not (Test-Path $screensH)) { throw "Not found: $screensH" }
if (-not (Test-Path $screensC)) { throw "Not found: $screensC" }

$summary = @()

# ─── Patch 1: COMPAT STUBS block in screens.h ────────────────────────────
$h = Get-Content $screensH -Raw
if ($h -match 'COMPAT STUBS') {
    $summary += '[1/3] screens.h    COMPAT STUBS block already present -- skipped'
} else {
    $insert = @"
    /* ─── COMPAT STUBS ── (re-add after every EEZ regenerate) */
    lv_obj_t *ui_skin_image;
    lv_obj_t *ui_bank_title_label;
    lv_obj_t *ui_preset_details_text_area;
    lv_obj_t *ui_entry_keyboard;
    lv_obj_t *ui_chip_gate;
    lv_obj_t *ui_chip_comp;
    lv_obj_t *ui_chip_eq;
    lv_obj_t *ui_chip_amp;
    lv_obj_t *ui_chip_cab;
    lv_obj_t *ui_chip_mod;
    lv_obj_t *ui_chip_delay;
    lv_obj_t *ui_chip_reverb;
} objects_t;
"@
    $hNew = $h -replace '(?m)^\} objects_t;\s*$', $insert
    if ($hNew -eq $h) {
        $summary += '[1/3] screens.h    FAILED -- could not find `} objects_t;` to anchor on'
    } elseif (-not $CheckOnly) {
        Set-Content -Path $screensH -Value $hNew -Encoding utf8 -NoNewline
        $summary += '[1/3] screens.h    COMPAT STUBS block inserted'
    } else {
        $summary += '[1/3] screens.h    would insert COMPAT STUBS block'
    }
}

# ─── Patch 2: #include <stdio.h> in screens.c ─────────────────────────────
$c = Get-Content $screensC -Raw
if ($c -match '#include\s*<stdio\.h>') {
    $summary += '[2/3] screens.c    #include <stdio.h> already present -- skipped'
} else {
    $cNew = $c -replace '(#include <string\.h>\r?\n)', "`$1#include <stdio.h>   /* PATCH: snprintf in tick_screen_settings */`r`n"
    if ($cNew -eq $c) {
        $summary += '[2/3] screens.c    FAILED -- could not find anchor `#include <string.h>`'
    } elseif (-not $CheckOnly) {
        Set-Content -Path $screensC -Value $cNew -Encoding utf8 -NoNewline
        $summary += '[2/3] screens.c    #include <stdio.h> inserted'
        $c = $cNew  # reuse for patch 3
    } else {
        $summary += '[2/3] screens.c    would insert #include <stdio.h>'
    }
}

# ─── Patch 3: snprintf rewrite of the broken tick_screen_settings block ─
# The broken pattern EEZ emits when a label.Text is bound to an int variable:
#
#     {
#         const char *new_val = get_var_bpm_gauge();
#         const char *cur_val = lv_label_get_text(objects.SOMENAME);
#         if (strcmp(new_val, cur_val) != 0) {
#             tick_value_change_obj = objects.SOMENAME;
#             lv_label_set_text(objects.SOMENAME, new_val);
#             tick_value_change_obj = NULL;
#         }
#     }
#
# We capture SOMENAME and rewrite the whole block to use snprintf.

$brokenPattern = @'
(?ms)\{\s*
\s+const char \*new_val = get_var_bpm_gauge\(\);\s*
\s+const char \*cur_val = lv_label_get_text\(objects\.(?<widget>\w+)\);\s*
\s+if \(strcmp\(new_val, cur_val\) != 0\) \{\s*
\s+tick_value_change_obj = objects\.\k<widget>;\s*
\s+lv_label_set_text\(objects\.\k<widget>, new_val\);\s*
\s+tick_value_change_obj = NULL;\s*
\s+\}\s*
\s+\}
'@

# Loop until no more broken blocks remain. Each iteration patches the first
# remaining occurrence; patched blocks are replaced with safe snprintf code,
# so they won't match $brokenPattern again. Multiple meters/labels bound to
# the same int variable produce one broken block each.
$patchedWidgets = @()
while ($true) {
    $m = [regex]::Match($c, $brokenPattern)
    if (-not $m.Success) { break }
    $widget = $m.Groups['widget'].Value
    $replacement = @"
{
        /* PATCH (re-apply after every EEZ regenerate): label ``$widget`` is
           bound to int32 ``bpm_gauge``. EEZ emits a ``const char *`` assignment
           from an int return -- type error. Render the int as text manually. */
        char new_val[16];
        snprintf(new_val, sizeof(new_val), "%d", (int)get_var_bpm_gauge());
        const char *cur_val = lv_label_get_text(objects.$widget);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.$widget;
            lv_label_set_text(objects.$widget, new_val);
            tick_value_change_obj = NULL;
        }
    }
"@
    $c = $c.Substring(0, $m.Index) + $replacement + $c.Substring($m.Index + $m.Length)
    $patchedWidgets += $widget
    if (-not $CheckOnly) {
        # write incrementally so a later failure leaves earlier progress on disk
        Set-Content -Path $screensC -Value $c -Encoding utf8 -NoNewline
    }
}

if ($patchedWidgets.Count -eq 0) {
    if ($c -match 'PATCH \(re-apply after every EEZ regenerate\): label ``') {
        $summary += '[3/3] screens.c    all int->string label blocks already patched -- skipped'
    } else {
        $summary += '[3/3] screens.c    no broken int->string label blocks found -- skipped (binding may already be fixed in EEZ)'
    }
} else {
    $verb = if ($CheckOnly) { 'would rewrite' } else { 'rewrote' }
    $summary += "[3/3] screens.c    $verb $($patchedWidgets.Count) int->string label block(s): $($patchedWidgets -join ', ')"
}

# ─── Patch 4: dedup screen_settings_state_t in screens.h ─────────────────
# When the EEZ design has multiple meters whose scales/needles share names,
# EEZ emits one struct field per scale/needle WITHOUT uniquifying — duplicate
# member error. We keep one field per unique type-name pair.
$h = Get-Content $screensH -Raw

if ($h -match 'PATCH \(re-apply after every EEZ regenerate\):[^\}]*Dedup to a single pair') {
    $summary += '[4/4] screens.h    screen_settings_state_t dedup already applied -- skipped'
} else {
    $stateBlockPattern = '(?ms)typedef struct \{\s*(?<body>(?:\s*lv_meter_(?:scale|indicator)_t\s*\*\s*\w+\s*;\s*)+)\s*\} screen_settings_state_t;'
    $m2 = [regex]::Match($h, $stateBlockPattern)
    if (-not $m2.Success) {
        $summary += '[4/4] screens.h    no screen_settings_state_t with duplicate members found -- skipped'
    } else {
        $body = $m2.Groups['body'].Value
        # Pick out unique "type *name" pairs in order of first appearance
        $seen = @{}
        $kept = @()
        foreach ($line in $body -split "`r?`n") {
            if ($line -match '\s*(lv_meter_(?:scale|indicator)_t)\s*\*\s*(\w+)\s*;') {
                $key = "$($Matches[1])::$($Matches[2])"
                if (-not $seen.ContainsKey($key)) {
                    $seen[$key] = $true
                    $kept += "    $($Matches[1]) *$($Matches[2]);"
                }
            }
        }
        $originalLines = ($body -split "`r?`n" | Where-Object { $_ -match '\S' }).Count
        if ($kept.Count -ge $originalLines) {
            $summary += '[4/4] screens.h    screen_settings_state_t has no duplicate members -- skipped'
        } else {
            $replacement = @"
typedef struct {
    /* PATCH (re-apply after every EEZ regenerate): EEZ emits one field per
       meter scale/needle without uniquifying names. Dedup to one field per
       (type, name). Only the last meter assignment wins at runtime.
       Durable fix: rename each meter's scale + needle in EEZ Studio. */
$($kept -join "`r`n")
} screen_settings_state_t;
"@
            $hNew = $h.Substring(0, $m2.Index) + $replacement + $h.Substring($m2.Index + $m2.Length)
            if (-not $CheckOnly) {
                Set-Content -Path $screensH -Value $hNew -Encoding utf8 -NoNewline
                $summary += "[4/4] screens.h    deduped screen_settings_state_t (kept $($kept.Count) of $originalLines fields)"
            } else {
                $summary += "[4/4] screens.h    would dedup screen_settings_state_t (kept $($kept.Count) of $originalLines fields)"
            }
        }
    }
}

# ─── Report ──────────────────────────────────────────────────────────────
Write-Host ''
Write-Host '=== EEZ post-regenerate patch ===' -ForegroundColor Cyan
foreach ($line in $summary) {
    if ($line -match 'FAILED') {
        Write-Host $line -ForegroundColor Red
    } elseif ($line -match 'inserted|rewrote|deduped') {
        Write-Host $line -ForegroundColor Green
    } else {
        Write-Host $line -ForegroundColor DarkGray
    }
}
Write-Host ''
if ($summary -match 'FAILED') { exit 1 }
