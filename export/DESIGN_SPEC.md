# ToneX Controller — Port Spec

A handoff document for porting the new design back into the LVGL/EEZ-Studio firmware on the 480×320 external display.

## Files in this folder

- **`ToneX Controller.html`** — interactive prototype on a design canvas (pan/zoom, drill-in works, knobs draggable). Use for navigation/UX reference.
- **`Screens.html`** — every screen state laid out at 1:1 device pixels. Use for visual reference while building.
- **`screens/*.png`** — every screen state exported as a 480×320 PNG (1:1 with the device). Open these next to your LVGL editor.
- **`tonex-controller-standalone.html`** — single self-contained file (no internet needed). Same as Screens.html but inlined.

---

## 1 — Device & technology

| | |
|---|---|
| Display | 480 × 320 px |
| Orientation | Landscape |
| Color format | BGR |
| LVGL version | 8.3 (per existing eez-project) |
| Dark theme | Yes |
| Border radius | 0 |

The display is finger-touch only. Hit targets are sized for adult fingers in a gigging context (low light, fast, no precision).

---

## 2 — Type

Two families, both free, both available on most embedded LVGL builds via Montserrat/equivalent if you can't ship custom fonts:

| Role | Family | Fallback |
|---|---|---|
| Display (titles, big numbers) | **Space Grotesk** — 400 / 500 / 600 / 700 | Inter, Montserrat, system-sans |
| Mono (labels, BPM, codes) | **JetBrains Mono** — 400 / 500 / 600 / 700 | SF Mono, ui-monospace |

If you only have one font available in the build, use it for everything — but keep the mono labels uppercase + tracked (`letter-spacing: 0.1em–0.25em`) to feel mono.

### Sizes used (px, design units = device px)

| Token | Size | Used for |
|---|---|---|
| `text-xs`  | 8–9  | mono labels (`PRESET`, `BPM`, `OUT`, modifier pill copy) |
| `text-sm`  | 10–11 | mono unit labels (`POST`, `SYNC`), browser sub-line |
| `text-md`  | 13–14 | row text, preset name in browser |
| `text-lg`  | 16–18 | effect screen title (`Reverb`), slot pill on main |
| `text-xl`  | 22–28 | preset name, BPM (heroes) |
| `text-2xl` | 110   | drag-overlay live value (giant) |

---

## 3 — Color tokens

All in sRGB. Convert to BGR for LVGL `lv_color_hex`.

### Surface (the "stage")
| Token | Hex | Purpose |
|---|---|---|
| `bg`      | `#0E0B08` | base canvas, warm near-black |
| `panel`   | `#1A1410` | header strip, prev/next button face |
| `panelHi` | `#241C16` | inactive pedal chip, switch off, dropdown face |
| `line`    | `#2E251D` | hairline dividers and inactive strokes |
| `text`    | `#F5EBD8` | primary text |
| `textDim` | `#8A7E6E` | secondary text and mono labels |

### Accent (UI hero)
| Token | Hex | Purpose |
|---|---|---|
| `accent`    | `#E8B547` | bank pill, prev/next chevrons, slot label, BPM, cable when live |
| `accentHot` | `#FFC857` | for any pressed/hot states (currently used minimally) |

### Effect hues
Each effect carries its own hue. **DO NOT reassign** — the user learns the color → effect mapping.

| Effect | Hex | Notes |
|---|---|---|
| **GATE** Noise Gate | `#F26B5E` | coral red |
| **COMP** Compressor | `#4FB3D9` | sky blue |
| **EQ** | `#B388F2` | lavender |
| **AMP** | `#E8B547` | amber (same as global accent — Amp is the hero) |
| **CAB** Cabinet | `#D98452` | burnt orange |
| **MOD** Modulation | `#3DD9B0` | mint |
| **DLY** Delay | `#F079C0` | pink |
| **RVB** Reverb | `#A186F2` | violet |

### Status dot colors (top-right of header)
| Service | Hex |
|---|---|
| BT | `#4FB3D9` |
| WIFI | `#3DD9B0` |
| USB | `#E8B547` |
| Off | `#333333` (dot), `#555555` (label) |

---

## 4 — Screens & navigation

```
Main
 ├── tap bank pill (top-left) ──▶ Preset Browser
 ├── tap pedal in chain        ──▶ Effect Detail
 └── PREV / NEXT buttons       (cycle presets)

Effect Detail
 ├── ◁ back arrow              ──▶ Main
 ├── master Enable toggle      (header right, only on effects that have it)
 ├── modifier pills            (POST / SYNC / PING / BYP — only the ones the effect supports)
 ├── Model dropdown            ──▶ List Picker overlay
 ├── Cab IR dropdown           ──▶ List Picker overlay
 └── drag any knob horizontally → Drag Overlay (live value modal)

Preset Browser
 ├── ◁ back arrow              ──▶ Main
 └── tap a row                 → activate that preset, return to Main
```

---

## 5 — Layout dimensions (px)

### Main screen (480×320)
```
┌────────────────────────────────────────────────────┐
│  HEADER · h=30                                     │   y=0
│  [A·01]  PRESET 01/8     ●BT ●WIFI ●USB           │
├────────────────────────────────────────────────────┤   y=30
│                                                    │
│   HERO BLOCK · 60px tall                          │
│   Smythbuilt OD             124 BPM                │
│   SMYTHBUILT 50             ◉ TAP                  │
│                                                    │
├────────────────────────────────────────────────────┤   y≈98
│  CHAIN BAND · h=104                                │
│  IN ▸  [G][C][E][A][C][M][D][R]  ▸ OUT             │   chips 46×84
│                                                    │
├────────────────────────────────────────────────────┤   y≈202
│                                                    │
│   ┌──────────┐   SLOT   ┌──────────┐               │
│   │ ◁ PREV   │  A·01    │  NEXT ▷  │   buttons 44px
│   └──────────┘          └──────────┘               │
│                                                    │
└────────────────────────────────────────────────────┘   y=320
```

### Effect detail (480×320)
```
┌────────────────────────────────────────────────────┐
│  HEADER · h=36  (tinted by effect color)           │   y=0
│  ◁  [glyph] Effect Name   A·01           [toggle]  │
├────────────────────────────────────────────────────┤   y=36
│  modifier pills row (8px gap, 6px below header)    │   y=44
│  [POST] [SYNC] [PING]   (only the ones that apply) │
│                                                    │
│  dropdowns row                                     │   y≈76
│  ┌─ MODEL ────────────┐  ┌─ CAB IR ──────────────┐ │
│  │ [glyph] Spring 1 ▾ │  │ [▥] 4x12 V30        ▾ │ │
│  └────────────────────┘  └────────────────────────┘ │
│                                                    │
│  KNOBS ROW · centered                              │   y≈140 - 280
│   ◯       ◯       ◯       ◯                       │
│  MIX    TIME   PRE-DEL  COLOR                      │
│  32%    2.4    35ms    5.5                         │
└────────────────────────────────────────────────────┘
```

Knob diameter: **66px** for 3 knobs, **58px** for 4 knobs (so they fit on one row).

### Preset browser row (480 × 44 each)
```
┌────────────────────────────────────────────────────┐
│ A·01  [skin 78×32]  Preset Name      ┃┃ ┃┃┃┃┃┃     │
│                     AMP NAME · 124 BPM             │
└────────────────────────────────────────────────────┘
```
Right-edge strip = 8 vertical bars × 4px wide, lit with each effect's hue when active in that preset.

---

## 6 — Components

### Pedal chip (in chain)
- **46 × 84 px**, radius 6px
- Inactive: bg `#181210`, inset stroke `#2A211A`
- Active: bg `{effect.color}`, inset shadow `0 -3px rgba(0,0,0,0.35)`, outer glow `0 0 14px {color}66`
- Contents (top→bottom): LED dot (8×8, on=#FFF5D6 glowing) · glyph (20px) · short name (mono, 8.5px) · model abbrev (mono, 7px, when applicable)
- "P" badge: top-right corner, 7px mono, appears when `post` toggle is on AND chip is active

### Cable between chips
- 10×84 px SVG, sine curve
- Off: `#2E251D`, plain stroke 2px
- Live (both ends active): `#E8B547`, with `drop-shadow(0 0 3px #E8B547)`

### Knob
- 270° sweep, range from `-135°` (7 o'clock) to `+135°` (5 o'clock)
- Drag axis: **horizontal** (right = +). 220px of travel = full range. Shift halves the rate (fine adjust).
- Tick ring: 11 ticks, every 5th is longer. Filled ticks (≤ current value) are the effect color with `drop-shadow(0 0 2px)`. Unfilled are `#3A2F26`.
- Body: 2-stop radial gradient, top-left lighter
- Indicator: 2.5px stroke, effect color, glowing; ends in a small `#FFF5D6` dot

### Toggle — pill style (modifier toggles)
- Padding `4px 9px`, radius 11px, mono 9.5px text, letter-spacing `.12em`
- Off: transparent bg, dim text, inset 1px line stroke
- On: bg = effect color, text `#1A0F08`, outer glow `0 0 8px {color}55`

### Toggle — switch style (master Enable, header right)
- 46 × 24 px track, 20px round knob, 2px padding
- Off: bg `#241C16`, knob `#5C4F40`
- On: bg = effect color, knob `#1A0F08`

### Dropdown button
- Field shape, radius 4, padding `8px 12px`, bg `#1F1814`, inset 1px stroke `{color}55`
- Contents: model glyph chip (24×24, bg = effect color, glyph in `#1A0F08`) + 2-line label (mono uppercase tag, big value) + ▾ chevron in effect color

### List picker overlay
- Full-screen dimmer: `rgba(8, 5, 3, 0.78)` with `backdrop-filter: blur(3px)`
- Card: 380 × auto (max 280), bg `#1A1410`, radius 6, inset 1px stroke `{color}55`
- Header strip: tinted gradient `{color}22 → transparent`, mono title color = effect color
- Rows: 10×14px padding, glyph chip (22×22) + label + "● ACTIVE" tag in effect color when selected

### Drag overlay (the headline feature)
- Full-screen, `rgba(8, 5, 3, 0.92)` + `blur(2px)` backdrop
- **`pointer-events: none`** so the drag keeps tracking
- Layout (vertically centered):
  - Tiny param label (mono 11px, effect color, tracking .25em)
  - Huge live value (display 110px, color `#FFF5D6`, glow `0 0 30px {color}99`)
  - Unit suffix to the right of the value (mono 22px, effect color)
  - Thin 280×4px progress bar showing where in the range
  - Min/Max labels under the bar
- Slides in `100ms ease-out`, disappears instantly on pointer-up.

---

## 7 — Real device data

Use these — they mirror the actual ToneX device. No invented effects.

### Models per effect

```
MOD:      Chorus · Flanger · Phaser · Rotary · Tremolo
DLY:      Digital · Tape
RVB:      Spring 1 · Spring 2 · Spring 3 · Spring 4 · Room · Plate
AMP/CAB:  Cab IR slot (Default, 4x12 V30, 2x12 Greenback, 1x12 Tweed,
                       Open-back 1x12, User IR 01/02)
```

### Toggle set per effect

| Effect | Enable | Post | Sync | Ping | Cab Byp |
|---|---|---|---|---|---|
| Gate | ● | ● | | | |
| Comp | ● | ● | | | |
| EQ   |   | ● | | | |
| Amp  | ● |   | | | |
| Cab  |   |   | | | ● |
| Mod  | ● | ● | ● | | |
| Delay | ● | ● | ● | ● | |
| Reverb | ● | ● | | | |

### Knob params per effect (preserve ranges, units)

See `shared.jsx` source for exact ranges. Summary:

| Effect | Knobs |
|---|---|
| Gate | Threshold (–90…0 dB) · Release (1…500 ms) · Depth (0…60 dB) |
| Comp | Threshold (–60…0 dB) · Attack (0…100 ms) · Gain (0…24 dB) |
| EQ | Bass · Mid · Mid Q · Treble (all –12…+12, Q 0…10) |
| Amp | Gain · Volume · Presence · Depth (all 0…10) |
| Cab | (no knobs — IR slot only) |
| Mod | Rate (0.1…10 Hz) · Depth (0…100%) · Mix (0…100%) |
| Delay | Time (20…2000 ms) · Feedback (0…100%) · Mix (0…100%) |
| Reverb | Mix (0…100%) · Time (0.1…12 s) · Pre-Delay (0…250 ms) · Color (0…10) |

---

## 8 — Behaviors & micro-rules

1. **Knob drag** — pointer-down anchors the value, dx maps linearly: `Δv = (dx / 220) × (max - min)`. Hold shift for ×0.25 rate (PC keyboards only; not relevant on touch).
2. **Drag overlay** — appears on `pointerdown`, disappears on `pointerup`. Live updates every move. Tracks the knob currently being dragged, not the screen layout.
3. **Sync mode** — when `SYNC` toggle is ON for Mod or Delay, the Rate/Time knob should be **replaced** by a tempo-subdivision selector (1/4, 1/8, 1/8T, 1/16, etc.) — *not implemented in prototype but recommended.*
4. **Post-routing badge** — when an active chip has `post=true`, show a small "P" badge in the top-right corner. (A future revision could reorder the chain visually past the Amp.)
5. **Master Enable** — when the master Enable toggle is OFF, the entire params region is rendered at 0.4 opacity (still tappable, but visually gated).
6. **Cab screen** — has no knobs. Just the Cab IR dropdown and the BYP pill toggle.
7. **Preset browser sort** — currently shown in physical bank/slot order (A·01, A·02, ..., B·04). No alphabetical sort.

---

## 9 — Asset usage

The skin PNGs from the original project (`smythbuilt.png`, `skin_jcm.png`, etc.) are reused **only in the preset browser** as 78×32 thumbnails (object-fit: contain). The big amp skin is gone from the main screen — per the brief, the main screen leads with the preset name and chain, not the photo.

Effect glyphs are kept as **unicode characters** in this prototype (⎍ ◈ ≡ ▤ ▥ ∿ ⋯ ◊). For shipping, replace these with the existing `effect_icon_*.png` assets from the original project — they're crisper at small sizes on the LVGL pipeline.

---

## 10 — Open questions for porting

- **Global settings screen** (BPM, Master Vol, Input Trim, Tuning Ref) — not designed yet. The original eez-project has these; recommend reusing the drill-in pattern (one screen per tab).
- **Sync-mode subdivision UI** — see #8.3 above. Worth designing before porting if Sync is a frequently-used feature.
- **Tap-tempo gesture** — the `◉ TAP` indicator hints at tap-to-set-BPM but no implementation in the prototype.
- **Status bar interactions** — BT/WiFi/USB dots are decorative now. Tap to open status / pairing screens?
