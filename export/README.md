# ToneX Controller — Export Package

Ready-to-port redesign of the ToneX external display controller (480×320).

## What's inside

| File | Purpose |
|---|---|
| **DESIGN_SPEC.md** | Authoritative spec — colors, sizes, components, behaviors. Read this first. |
| **tonex-controller-standalone.html** | Single-file viewer showing every screen state at 1:1 device pixels. Works offline. **Open in any browser.** |
| **tonex-controller-interactive-standalone.html** | Interactive prototype — drag knobs, drill into effects, open dropdowns. Works offline. |
| **screens/** | Every screen state as a 480×320 PNG. Pin these next to your LVGL editor while you build. |
| **source/** | The HTML/JSX source that produced everything above. |

## Quick orientation

- **Screens.html / standalone.html** — your visual reference. Layout, colors, type, all 1:1.
- **interactive-standalone.html** — your UX reference. Try the drag-overlay, list-picker, modifier toggles to feel the intent.
- **DESIGN_SPEC.md §3** — color tokens to copy into LVGL styles.
- **DESIGN_SPEC.md §5** — pixel coordinates for the layouts.
- **screens/** PNGs — drop them in your LVGL editor as background-image references.

## Open questions before shipping

See DESIGN_SPEC.md §10 — Global Settings screen, Sync-mode subdivision UI, tap-tempo gesture, status bar tap targets. Worth deciding before fully porting.

Built by Claude for thegang.io, May 2026.
