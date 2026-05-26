#ifndef TONEX_THEME_H
#define TONEX_THEME_H

/*
 * Theme tokens for the 480x320 main screen redesign.
 * Mirrors DESIGN_SPEC §3 (export/DESIGN_SPEC.md).
 *
 * Hex values are sRGB 0xRRGGBB; lv_color_hex expects 0xRRGGBB on this build
 * (CONFIG_LV_COLOR_16_SWAP=y handles BGR conversion at draw time).
 */

/* surface */
#define THEME_BG            0x0E0B08   /* warm near-black canvas       */
#define THEME_PANEL         0x1A1410   /* header strip, button face    */
#define THEME_PANEL_HI      0x241C16   /* inactive chip, dropdown face */
#define THEME_LINE          0x2E251D   /* hairline dividers            */
#define THEME_TEXT          0xF5EBD8   /* primary text                 */
#define THEME_TEXT_DIM      0x8A7E6E   /* secondary text, mono labels  */

/* accent */
#define THEME_ACCENT        0xE8B547   /* amber  - bank pill, BPM, cable when live */
#define THEME_ACCENT_HOT    0xFFC857   /* pressed/hot states            */

/* effect hues (DO NOT reassign per spec §3) */
#define THEME_FX_GATE       0xF26B5E   /* coral red */
#define THEME_FX_COMP       0x4FB3D9   /* sky blue  */
#define THEME_FX_EQ         0xB388F2   /* lavender  */
#define THEME_FX_AMP        0xE8B547   /* amber     */
#define THEME_FX_CAB        0xD98452   /* burnt orange */
#define THEME_FX_MOD        0x3DD9B0   /* mint      */
#define THEME_FX_DLY        0xF079C0   /* pink      */
#define THEME_FX_RVB        0xA186F2   /* violet    */

/* status dots */
#define THEME_STATUS_BT     0x4FB3D9
#define THEME_STATUS_WIFI   0x3DD9B0
#define THEME_STATUS_USB    0xE8B547
#define THEME_STATUS_OFF    0x333333
#define THEME_STATUS_LABEL_OFF  0x555555

/* fonts: requires CONFIG_LV_FONT_MONTSERRAT_{10,12,14,18,22,28,32}=y */
#define THEME_FONT_MONO_XS  lv_font_montserrat_10   /* mono labels (PRESET, BPM)      */
#define THEME_FONT_MONO_SM  lv_font_montserrat_12   /* mono labels slightly larger    */
#define THEME_FONT_TEXT_MD  lv_font_montserrat_14   /* body, button text              */
#define THEME_FONT_TEXT_LG  lv_font_montserrat_18   /* bank pill, button labels       */
#define THEME_FONT_HERO_MD  lv_font_montserrat_22   /* preset name (compact)          */
#define THEME_FONT_HERO_LG  lv_font_montserrat_28   /* BPM number                     */
#define THEME_FONT_BIG      lv_font_montserrat_32   /* drag overlay live value (TBD)  */

#endif /* TONEX_THEME_H */
