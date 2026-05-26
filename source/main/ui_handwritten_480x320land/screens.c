#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"
#include "tonex_theme.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

/*
 * Foundation port of the 480x320 redesign (see export/DESIGN_SPEC.md).
 *
 * Layout, top→bottom:
 *   HEADER       y=0..30    bank pill, project text, BT/WIFI/USB dots
 *   HERO         y=30..98   preset name + BPM
 *   CHAIN BAND   y=98..202  IN ▸ 8 effect chips ▸ OUT, settings gear
 *   FOOTER       y=202..320 large PREV / NEXT buttons
 *
 * Chip widgets (objects.ui_icon_*) stay as lv_img so display_tonex.c's
 * lv_img_set_src(...) state-swaps continue to work; the chip frame styling
 * is applied to the lv_img itself and the effect sprite is centered inside.
 *
 * Legacy widgets that the new design has no place for (skin photo, description
 * textarea, skin-edit arrows, OK tick, on-screen keyboard, full Valeton chain)
 * are still allocated but hidden, so display.c writes against them keep working.
 *
 * IMPORTANT: this file lives in ui_handwritten_480x320land/, separate from the
 * EEZ-managed ui_generated_480x320land/. Do not regenerate from EEZ Studio
 * into this folder — your changes will be lost. The ws35b/jc3248w535/polar_max_v2
 * builds source this folder (see source/main/CMakeLists.txt).
 */
/* Build one effect chip: a styled container framing a small icon image and a
   name label, with LV_STATE_CHECKED swapping the background to the effect's
   own color (matches export/screens reference).
   - `chip_out`: gets the container pointer (click target, state target)
   - `icon_out`: gets the inner lv_img pointer (display.c writes src here)
   The container is what's clickable; the inner img is non-clickable so events
   resolve to the container regardless of where inside the chip you tap. */
static void make_chip(lv_obj_t *parent, lv_obj_t **chip_out, lv_obj_t **icon_out,
                     const lv_img_dsc_t *icon, const char *name,
                     uint32_t effect_color, lv_coord_t x) {
    lv_obj_t *chip = lv_obj_create(parent);
    *chip_out = chip;
    lv_obj_set_pos(chip, x, 8);
    lv_obj_set_size(chip, 50, 88);
    lv_obj_clear_flag(chip, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(chip, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(chip, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(chip, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    /* Inactive: dark face, dim border */
    lv_obj_set_style_bg_color(chip, lv_color_hex(0x181210), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(chip,   LV_OPA_COVER,            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(chip, lv_color_hex(0x2A211A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(chip, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    /* Active: full effect color + matching glow */
    lv_obj_set_style_bg_color(chip, lv_color_hex(effect_color), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_color(chip, lv_color_hex(effect_color), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_color(chip, lv_color_hex(effect_color), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(chip, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_opa(chip,   LV_OPA_40, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_add_event_cb(chip, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED,  (void *)0);
    lv_obj_add_event_cb(chip, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
    lv_obj_add_flag(chip, LV_OBJ_FLAG_CLICKABLE);

    /* Inner icon: natural size (LV_SIZE_CONTENT) so LVGL doesn't tile it. */
    lv_obj_t *img = lv_img_create(chip);
    *icon_out = img;
    lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_img_set_src(img, icon);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_CLICKABLE);

    /* Text color is set on the chip (not the label) so LVGL's style inheritance
       carries it down — flips dim→dark when the chip becomes CHECKED. */
    lv_obj_set_style_text_color(chip, lv_color_hex(THEME_TEXT_DIM), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(chip, lv_color_hex(0x1A0F08),       LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(chip,  &THEME_FONT_MONO_XS, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(chip, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *lbl = lv_label_create(chip);
    lv_obj_align(lbl, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_label_set_text(lbl, name);
    lv_obj_clear_flag(lbl, LV_OBJ_FLAG_CLICKABLE);
}

static lv_obj_t *make_status_dot(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, uint32_t color, bool on) {
    lv_obj_t *dot = lv_obj_create(parent);
    lv_obj_set_pos(dot, x, y);
    lv_obj_set_size(dot, 8, 8);
    lv_obj_set_style_pad_all(dot, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(dot, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dot, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dot, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (on) {
        lv_obj_set_style_shadow_color(dot, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(dot, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(dot, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(dot, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    return dot;
}

static lv_obj_t *make_status_label(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, const char *txt) {
    lv_obj_t *l = lv_label_create(parent);
    lv_obj_set_pos(l, x, y);
    lv_obj_set_style_text_color(l, lv_color_hex(THEME_TEXT_DIM), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(l, &THEME_FONT_MONO_XS, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(l, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(l, txt);
    return l;
}

//
// Screens
//

void create_screen_screen1() {
    lv_obj_t *screen = lv_obj_create(0);
    objects.screen1 = screen;
    lv_obj_set_pos(screen, 0, 0);
    lv_obj_set_size(screen, 480, 320);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(THEME_BG), LV_PART_MAIN | LV_STATE_DEFAULT);

    /* ───── GESTURE ROOT (full-screen, transparent) ───── */
    lv_obj_t *gesture = lv_obj_create(screen);
    objects.ui_touch_gesture_container = gesture;
    lv_obj_set_pos(gesture, 0, 0);
    lv_obj_set_size(gesture, 480, 320);
    lv_obj_set_style_pad_all(gesture, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(gesture, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(gesture, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(gesture, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(gesture, action_gesture, LV_EVENT_GESTURE, (void *)0);
    lv_obj_clear_flag(gesture, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);

    /* ───── HEADER (y=0..30) ───── */
    lv_obj_t *header = lv_obj_create(gesture);
    objects.ui_top_panel = header;
    lv_obj_set_pos(header, 0, 0);
    lv_obj_set_size(header, 480, 30);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(header, lv_color_hex(THEME_PANEL), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(header, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(header, lv_color_hex(THEME_LINE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(header, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* Bank pill — amber background, dark text */
    {
        lv_obj_t *obj = lv_label_create(header);
        objects.ui_bank_value_label = obj;
        lv_obj_set_pos(obj, 8, 4);
        lv_obj_set_size(obj, 44, 22);
        lv_obj_set_style_pad_top(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(obj, lv_color_hex(THEME_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(obj, lv_color_hex(0x1A0F08), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(obj, &THEME_FONT_TEXT_LG, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(obj, "1");
    }

    /* Project heading — dim mono text in header */
    {
        lv_obj_t *obj = lv_label_create(header);
        objects.ui_project_heading_label = obj;
        lv_obj_set_pos(obj, 62, 9);
        lv_obj_set_size(obj, 240, 16);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_text_color(obj, lv_color_hex(THEME_TEXT_DIM), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(obj, &THEME_FONT_MONO_XS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(obj, "TONEX CONTROLLER");
    }

    /* Status dot pairs (BT / WIFI / USB). display.c toggles HIDDEN flag to swap. */
    objects.ui_bt_status_disconn    = make_status_dot(header, 350, 11, THEME_STATUS_OFF, false);
    objects.ui_bt_status_conn       = make_status_dot(header, 350, 11, THEME_STATUS_BT,  true);
    make_status_label(header, 361, 9, "BT");
    objects.ui_wi_fi_status_disconn = make_status_dot(header, 384, 11, THEME_STATUS_OFF, false);
    objects.ui_wi_fi_status_conn    = make_status_dot(header, 384, 11, THEME_STATUS_WIFI,true);
    make_status_label(header, 395, 9, "WIFI");
    objects.ui_usb_status_fail      = make_status_dot(header, 428, 11, THEME_STATUS_OFF, false);
    objects.ui_usb_status_ok        = make_status_dot(header, 428, 11, THEME_STATUS_USB, true);
    make_status_label(header, 439, 9, "USB");

    /* Legacy IK logo — retired per spec §9, kept hidden. */
    {
        lv_obj_t *obj = lv_img_create(screen);
        objects.ui_ik_logo = obj;
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }

    /* ───── HERO BLOCK (y=30..98) ───── */

    /* Preset name — big text, full width of hero left side.
       Skin photo deliberately omitted — chain is the visual focus.
       Long-press still triggers skin-edit (kept for parity with old UI);
       skin-edit overlays the chain band with ◁ ✓ ▷ when active. */
    {
        lv_obj_t *obj = lv_label_create(gesture);
        objects.ui_preset_heading_label = obj;
        lv_obj_set_pos(obj, 14, 50);
        lv_obj_set_size(obj, 340, 30);
        lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(obj, action_enable_skin_edit, LV_EVENT_LONG_PRESSED, (void *)0);
        lv_obj_set_style_text_color(obj, lv_color_hex(THEME_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(obj, &THEME_FONT_HERO_MD, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(obj, "Preset Name");
    }

    /* BPM number — large numeric, right side of hero */
    {
        lv_obj_t *obj = lv_label_create(gesture);
        objects.ui_bpm_value_label = obj;
        lv_obj_set_pos(obj, 378, 38);
        lv_obj_set_size(obj, 62, 32);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_text_color(obj, lv_color_hex(THEME_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(obj, &THEME_FONT_HERO_LG, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(obj, "0");
    }

    /* "BPM" label — accent mono, beside the number */
    {
        lv_obj_t *obj = lv_label_create(gesture);
        objects.ui_bpm_title_label = obj;
        lv_obj_set_pos(obj, 445, 52);
        lv_obj_set_size(obj, 30, 14);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_text_color(obj, lv_color_hex(THEME_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(obj, &THEME_FONT_MONO_XS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(obj, "BPM");
    }

    /* Tap-tempo dot indicator — small amber circle under the BPM number */
    {
        lv_obj_t *obj = lv_obj_create(gesture);
        objects.ui_bpm_indicator = obj;
        lv_obj_set_pos(obj, 414, 78);
        lv_obj_set_size(obj, 10, 10);
        lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(obj, lv_color_hex(THEME_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }
    {
        /* Static "TAP" label next to indicator */
        lv_obj_t *l = lv_label_create(gesture);
        lv_obj_set_pos(l, 428, 80);
        lv_obj_set_style_text_color(l, lv_color_hex(THEME_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(l, &THEME_FONT_MONO_XS, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(l, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(l, "TAP");
    }

    /* Legacy "BANK" title label — retired in the new design. */
    {
        lv_obj_t *obj = lv_label_create(screen);
        objects.ui_bank_title_label = obj;
        lv_label_set_text(obj, "");
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }

    /* Skin container + image — kept hidden on main per the streamlined design
       (no skin focus). Still allocated so display.c writes don't crash. */
    {
        lv_obj_t *obj = lv_obj_create(screen);
        objects.ui_skins = obj;
        lv_obj_set_size(obj, 2, 2);
        lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        {
            lv_obj_t *img = lv_img_create(obj);
            objects.ui_skin_image = img;
            lv_img_set_src(img, &img_skin_jcm);
            lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        }
    }

    /* Preset description textarea — hidden, kept as keyboard target so the
       skin-edit flow's description editing still has somewhere to write. */
    {
        lv_obj_t *obj = lv_textarea_create(screen);
        objects.ui_preset_details_text_area = obj;
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, 2, 2);
        lv_textarea_set_max_length(obj, 31);
        lv_textarea_set_text(obj, "");
        lv_textarea_set_one_line(obj, false);
        lv_obj_add_event_cb(obj, action_preset_description_pressed, LV_EVENT_PRESSED, (void *)0);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }

    /* ───── CHAIN BAND (y=98..202) ───── */
    lv_obj_t *band = lv_obj_create(gesture);
    objects.ui_bottom_panel_tonex = band;
    lv_obj_set_pos(band, 0, 98);
    lv_obj_set_size(band, 480, 104);
    lv_obj_clear_flag(band, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(band, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(band, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(band, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(band, lv_color_hex(THEME_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(band, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* 8 chips at 50w + 4px gaps span x=22..466 (444 wide). IN/OUT labels
       sit vertically centered at the band's edges. */
    /* Signal-flow cable: thin horizontal strip running behind the chips so
       the chain reads as a connected path. Created BEFORE the chips so it
       sits beneath them in z-order. */
    {
        lv_obj_t *cable = lv_obj_create(band);
        lv_obj_set_pos(cable, 22, 51);
        lv_obj_set_size(cable, 442, 2);
        lv_obj_set_style_pad_all(cable, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(cable, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(cable, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(cable, lv_color_hex(THEME_LINE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(cable, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(cable, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    }

    make_status_label(band, 4,   46, "IN");
    make_status_label(band, 468, 46, "OUT");

    make_chip(band, &objects.ui_chip_gate,   &objects.ui_icon_gate,
              &img_effect_icon_gate_off,   "GATE", THEME_FX_GATE,  22);
    make_chip(band, &objects.ui_chip_comp,   &objects.ui_icon_comp,
              &img_effect_icon_comp_off,   "COMP", THEME_FX_COMP,  78);
    make_chip(band, &objects.ui_chip_eq,     &objects.ui_icon_eq,
              &img_effect_icon_eq,         "EQ",   THEME_FX_EQ,    134);
    make_chip(band, &objects.ui_chip_amp,    &objects.ui_icon_amp,
              &img_effect_icon_amp_off,    "AMP",  THEME_FX_AMP,   190);
    make_chip(band, &objects.ui_chip_cab,    &objects.ui_icon_cab,
              &img_effect_icon_cab_off,    "CAB",  THEME_FX_CAB,   246);
    make_chip(band, &objects.ui_chip_mod,    &objects.ui_icon_mod,
              &img_effect_icon_mod_off,    "MOD",  THEME_FX_MOD,   302);
    make_chip(band, &objects.ui_chip_delay,  &objects.ui_icon_delay,
              &img_effect_icon_delay_off,  "DLY",  THEME_FX_DLY,   358);
    make_chip(band, &objects.ui_chip_reverb, &objects.ui_icon_reverb,
              &img_effect_icon_reverb_off, "RVB",  THEME_FX_RVB,   414);

    /* Settings gear — kept in the tree (display.c expects it) but hidden:
       the reference design has no visible gear; settings will be reachable
       per-effect via tapping a chip once effect-detail screens are ported. */
    {
        lv_obj_t *gear = lv_img_create(screen);
        objects.ui_settings_image = gear;
        lv_img_set_src(gear, &img_settings);
        lv_obj_add_event_cb(gear, action_show_settings_page, LV_EVENT_PRESSED, (void *)0);
        lv_obj_add_flag(gear, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);
    }

    /* ───── VALETON CHAIN — parallel hidden subtree (kept for display.c) ───── */
    {
        lv_obj_t *vband = lv_obj_create(gesture);
        objects.ui_bottom_panel_valeton = vband;
        lv_obj_set_pos(vband, 0, 98);
        lv_obj_set_size(vband, 480, 104);
        lv_obj_add_flag(vband, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(vband, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_pad_all(vband, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(vband, lv_color_hex(THEME_BG), LV_PART_MAIN | LV_STATE_DEFAULT);

        struct vchip { lv_obj_t **slot; const lv_img_dsc_t *icon; lv_coord_t x; };
        struct vchip vchips[] = {
            { &objects.ui_icon_val_pre, &img_pre_off,   4 },
            { &objects.ui_icon_val_amp, &img_amp_off,  48 },
            { &objects.ui_icon_val_nr,  &img_nr_off,   92 },
            { &objects.ui_icon_val_rvb, &img_rvb_off, 136 },
            { &objects.ui_icon_val_cab, &img_cab_off, 180 },
            { &objects.ui_icon_val_tc,  &img_tc_off,  224 },
            { &objects.ui_icon_val_eq,  &img_eq_off,  268 },
            { &objects.ui_icon_val_mod, &img_mod_off, 312 },
            { &objects.ui_icon_val_dly, &img_dly_off, 356 },
            { &objects.ui_icon_val_dst, &img_dst_off, 400 },
        };
        for (size_t i = 0; i < sizeof(vchips) / sizeof(vchips[0]); i++) {
            lv_obj_t *chip = lv_img_create(vband);
            *(vchips[i].slot) = chip;
            lv_obj_set_pos(chip, vchips[i].x, 10);
            lv_obj_set_size(chip, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(chip, vchips[i].icon);
            lv_img_set_zoom(chip, 200);
            lv_obj_add_event_cb(chip, action_effect_icon_clicked, LV_EVENT_LONG_PRESSED, (void *)0);
            lv_obj_add_event_cb(chip, action_effect_icon_clicked, LV_EVENT_SHORT_CLICKED, (void *)0);
            lv_obj_add_flag(chip, LV_OBJ_FLAG_CLICKABLE);
        }
        {
            lv_obj_t *gear = lv_img_create(vband);
            objects.ui_val_settings = gear;
            lv_obj_set_pos(gear, 450, 0);
            lv_obj_set_size(gear, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(gear, &img_settings);
            lv_img_set_zoom(gear, 192);
            lv_obj_add_event_cb(gear, action_show_settings_page, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(gear, LV_OBJ_FLAG_CLICKABLE);
        }
    }

    /* ───── FOOTER (y=202..320) ───── */

    /* PREV button — wired to action_previous_clicked (real preset nav).
       The hard shadow below (width 0, ofs_y 3) acts as a "second border
       at the bottom" — gives the button a sense of physical depth. */
    {
        lv_obj_t *btn = lv_obj_create(gesture);
        lv_obj_set_pos(btn, 24, 230);
        lv_obj_set_size(btn, 180, 70);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        /* Vertical gradient: lighter top → darker bottom, like a backlit cap. */
        lv_obj_set_style_bg_color(btn,      lv_color_hex(0x261C16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(btn, lv_color_hex(THEME_PANEL), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(btn,   LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn,      lv_color_hex(THEME_PANEL_HI), LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_bg_opa(btn,        LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(btn,  lv_color_hex(THEME_LINE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn,  1, LV_PART_MAIN | LV_STATE_DEFAULT);
        /* Depth: hard shadow 3px below (no blur). Collapses on press. */
        lv_obj_set_style_shadow_color(btn,  lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_ofs_x(btn,  0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_ofs_y(btn,  3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(btn,  0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_opa(btn,    LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_ofs_y(btn,  0, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_add_event_cb(btn, action_previous_clicked, LV_EVENT_PRESSED, (void *)0);

        lv_obj_t *chev = lv_label_create(btn);
        lv_obj_set_style_text_color(chev, lv_color_hex(THEME_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(chev, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(chev, LV_ALIGN_LEFT_MID, 24, -2);
        lv_label_set_text(chev, LV_SYMBOL_LEFT);

        lv_obj_t *txt = lv_label_create(btn);
        lv_obj_set_style_text_color(txt, lv_color_hex(THEME_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(txt, &THEME_FONT_TEXT_LG, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(txt, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(txt, LV_ALIGN_CENTER, 14, 0);
        lv_label_set_text(txt, "PREV");
    }

    /* NEXT button — same depth treatment as PREV. */
    {
        lv_obj_t *btn = lv_obj_create(gesture);
        lv_obj_set_pos(btn, 276, 230);
        lv_obj_set_size(btn, 180, 70);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn,      lv_color_hex(0x261C16), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_color(btn, lv_color_hex(THEME_PANEL), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_grad_dir(btn,   LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn,      lv_color_hex(THEME_PANEL_HI), LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_set_style_bg_opa(btn,        LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(btn,  lv_color_hex(THEME_LINE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(btn,  1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_color(btn,  lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_ofs_x(btn,  0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_ofs_y(btn,  3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(btn,  0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_opa(btn,    LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_ofs_y(btn,  0, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_add_event_cb(btn, action_next_clicked, LV_EVENT_PRESSED, (void *)0);

        lv_obj_t *txt = lv_label_create(btn);
        lv_obj_set_style_text_color(txt, lv_color_hex(THEME_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(txt, &THEME_FONT_TEXT_LG, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_letter_space(txt, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(txt, LV_ALIGN_CENTER, -14, 0);
        lv_label_set_text(txt, "NEXT");

        lv_obj_t *chev = lv_label_create(btn);
        lv_obj_set_style_text_color(chev, lv_color_hex(THEME_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(chev, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(chev, LV_ALIGN_RIGHT_MID, -24, -2);
        lv_label_set_text(chev, LV_SYMBOL_RIGHT);
    }

    /* Skin-edit overlay: ◁ / ▷ / ✓ are hidden by default. action_enable_skin_edit
       (display.c) unhides them and hides the chain band; action_save_skin_edit
       reverses both. Placed at chain-band Y so they replace the band visually. */
    {
        lv_obj_t *obj = lv_img_create(screen);
        objects.ui_left_arrow = obj;
        lv_obj_set_pos(obj, 24, 128);
        lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_img_set_src(obj, &img_arrow_left);
        lv_img_set_zoom(obj, 192);
        lv_obj_add_event_cb(obj, action_amp_skin_previous, LV_EVENT_PRESSED, (void *)0);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }
    {
        lv_obj_t *obj = lv_img_create(screen);
        objects.ui_right_arrow = obj;
        lv_obj_set_pos(obj, 410, 128);
        lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_img_set_src(obj, &img_arrow_right);
        lv_img_set_zoom(obj, 192);
        lv_obj_add_event_cb(obj, action_amp_skin_next, LV_EVENT_PRESSED, (void *)0);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }
    {
        lv_obj_t *obj = lv_img_create(screen);
        objects.ui_ok_tick = obj;
        lv_obj_set_pos(obj, 220, 128);
        lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_img_set_src(obj, &img_tick);
        lv_img_set_zoom(obj, 256);
        lv_obj_add_event_cb(obj, action_save_skin_edit, LV_EVENT_PRESSED, (void *)0);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }
    {
        lv_obj_t *obj = lv_keyboard_create(screen);
        objects.ui_entry_keyboard = obj;
        lv_obj_set_pos(obj, 0, 137);
        lv_obj_set_size(obj, 480, 183);
        lv_obj_add_event_cb(obj, action_keyboard_ok, LV_EVENT_READY, (void *)0);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }

    lv_keyboard_set_textarea(objects.ui_entry_keyboard, objects.ui_preset_details_text_area);

    tick_screen_screen1();
}

void tick_screen_screen1() {
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ui_SettingsTabView
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 60);
            objects.ui_settings_tab_view = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x563f2a), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_get_tab_btns(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xc0c0c0), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xfb9230), LV_PART_ITEMS | LV_STATE_CHECKED);
                }
                {
                    // ui_GateTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Gate");
                    objects.ui_gate_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_NoiseGateEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_NoiseGateSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_noise_gate_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_NoiseGatePostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_NoiseGatePostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_noise_gate_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_NoiseGateThresholdLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_threshold_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Threshold");
                        }
                        {
                            // ui_NoiseGateThresholdSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_noise_gate_threshold_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_NoiseGateThresholdValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_threshold_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_NoiseGateReleaseLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_release_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Release");
                        }
                        {
                            // ui_NoiseGateReleaseSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_noise_gate_release_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 5, 500);
                            lv_slider_set_value(obj, 20, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_NoiseGateReleaseValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_release_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_NoiseGateDepthLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_depth_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Depth");
                        }
                        {
                            // ui_NoiseGateDepthSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_noise_gate_depth_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -60, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_NoiseGateDepthValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_noise_gate_depth_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_CompresssorTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Comp");
                    objects.ui_compresssor_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_CompressorEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_CompressorEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_compressor_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_CompressorPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_CompressorPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_compressor_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_CompressorThresholdLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_threshold_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Threshold");
                        }
                        {
                            // ui_CompressorThresholdSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_compressor_threshold_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -40, 0);
                            lv_slider_set_value(obj, -14, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_CompressorThresholdValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_threshold_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_CompressorAttackLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_attack_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Attack");
                        }
                        {
                            // ui_CompressorAttackSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_compressor_attack_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 1, 51);
                            lv_slider_set_value(obj, 14, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_CompressorAttackValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_attack_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_CompressorGainLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_gain_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Gain");
                        }
                        {
                            // ui_CompressorGainSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_compressor_gain_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -30, 10);
                            lv_slider_set_value(obj, -12, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_CompressorGainValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_compressor_gain_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_AmpTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Amp");
                    objects.ui_amp_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_AmpEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amp_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_AmpEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_amp_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_AmpCabLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amp_cab_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Cab");
                        }
                        {
                            // ui_CabinetModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_cabinet_model_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 37);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Tone Model\nVIR\nDisabled");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj1 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_AmplifierGainLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_gain_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Gain");
                        }
                        {
                            // ui_AmplifierGainSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_amplifier_gain_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierGainValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_gain_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_AmplifierVolumeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_volume_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Volume");
                        }
                        {
                            // ui_AmplifierVolumeSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_amplifier_volume_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierVolumeValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_volume_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_AmplifierPresenseLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_presense_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Presence");
                        }
                        {
                            // ui_AmplifierPresenseSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_amplifier_presense_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierPresenseValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_presense_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_AmplifierDepthLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_depth_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Depth");
                        }
                        {
                            // ui_AmplifierDepthSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_amplifier_depth_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_AmplifierDepthValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_amplifier_depth_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_EQTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "EQ");
                    objects.ui_eq_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_EQPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_post_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_EQPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_eq_post_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_EQBassLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_bass_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Bass");
                        }
                        {
                            // ui_EQBassSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_eq_bass_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQBassValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_bass_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_EQMidLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Mid");
                        }
                        {
                            // ui_EQMidSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_eq_mid_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQMidValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_EQMidQLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_qlabel = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "MidQ");
                        }
                        {
                            // ui_EQMidQSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_eq_mid_qslider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQMidQValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_mid_qvalue = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_EQTrebleLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_treble_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Treble");
                        }
                        {
                            // ui_EQTrebleSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_eq_treble_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_EQTrebleValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_eq_treble_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ModulationTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Mod");
                    objects.ui_modulation_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ModulationEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ModulationEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_modulation_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ModulationPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_ModulationPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_modulation_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ModulationModeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_mode_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Model");
                        }
                        {
                            // ui_ModulationModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_modulation_model_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 37);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Chorus\nTremolo\nPhaser\nFlanger\nRotary");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj2 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ModulationSyncLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_sync_label = obj;
                            lv_obj_set_pos(obj, 309, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Sync");
                        }
                        {
                            // ui_ModulationSyncSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_modulation_sync_switch = obj;
                            lv_obj_set_pos(obj, 393, -71);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ModulationParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ModulationTSDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_modulation_ts_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 76);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "1/32\n1/32 D\n1/32 T\n1/16\n1/16 D\n1/16 T\n1/8\n1/8 D\n1/8 T\n1/4\n1/4 D\n1/4 T\n1/2\n1/2 D\n1/2 T\n1/1\n1/1 D\n1/1 T");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj3 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ModulationParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ModulationParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ModulationParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 1000);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ModulationParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ModulationParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ModulationParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ModulationParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_modulation_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ModulationParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_modulation_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_DelayTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dly");
                    objects.ui_delay_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_DelayEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_DelayEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_DelayPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_DelayPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_DelayModeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_mode_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Model");
                        }
                        {
                            // ui_DelayModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_delay_model_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 37);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Digital\nTape");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj4 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_DelaySyncLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_sync_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Sync");
                        }
                        {
                            // ui_DelaySyncSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_sync_switch = obj;
                            lv_obj_set_pos(obj, 106, -34);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_DelayPingPongLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_ping_pong_label = obj;
                            lv_obj_set_pos(obj, 309, -71);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "PPng");
                        }
                        {
                            // ui_DelayPingPongSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_delay_ping_pong_switch = obj;
                            lv_obj_set_pos(obj, 393, -71);
                            lv_obj_set_size(obj, 60, 26);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_DelayTSLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_ts_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Time");
                        }
                        {
                            // ui_DelayTSSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_delay_ts_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 1000);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_DelayTSValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_ts_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_DelayTSDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_delay_ts_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 114);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "1/32\n1/32 D\n1/32 T\n1/16\n1/16 D\n1/16 T\n1/8\n1/8 D\n1/8 T\n1/4\n1/4 D\n1/4 T\n1/2\n1/2 D\n1/2 T\n1/1\n1/1 D\n1/1 T");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj5 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_DelayFeedbackLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_feedback_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Feedback");
                        }
                        {
                            // ui_DelayFeedbackSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_delay_feedback_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_DelayFeedbackValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_feedback_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_DelayMixLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_mix_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Mix");
                        }
                        {
                            // ui_DelayMixSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_delay_mix_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_DelayMixValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_delay_mix_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ReverbTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Rev");
                    objects.ui_reverb_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ReverbEnableLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_enable_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ReverbEnableSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_reverb_enable_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ReverbPostLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_post_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Post");
                        }
                        {
                            // ui_ReverbPostSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_reverb_post_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ReverbModeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_mode_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Model");
                        }
                        {
                            // ui_ReverbModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_reverb_model_dropdown = obj;
                            lv_obj_set_pos(obj, 106, 37);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Spring 1\nSpring 2\nSpring 3\nSpring 4\nRoom\nPlate");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj6 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ReverbMixLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_mix_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Mix");
                        }
                        {
                            // ui_ReverbMixSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_reverb_mix_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbMixValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_mix_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ReverbTimeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_time_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Time");
                        }
                        {
                            // ui_ReverbTimeSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_reverb_time_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 10);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbTimeValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_time_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ReverbPredelayLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_predelay_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Pre-Delay");
                        }
                        {
                            // ui_ReverbPredelaySlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_reverb_predelay_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 0, 500);
                            lv_slider_set_value(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbPredelayValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_predelay_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ReverbColorLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_color_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Color");
                        }
                        {
                            // ui_ReverbColorSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_reverb_color_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -10, 10);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ReverbColorValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_reverb_color_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_GlobalTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Glob");
                    objects.ui_global_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_CabBypassLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_cab_bypass_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Cab Byp");
                        }
                        {
                            // ui_CabBypassSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_cab_bypass_switch = obj;
                            lv_obj_set_pos(obj, 106, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_TempoSourcetLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_tempo_sourcet_label = obj;
                            lv_obj_set_pos(obj, 309, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Tempo");
                        }
                        {
                            // ui_TempoSourceSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_tempo_source_switch = obj;
                            lv_obj_set_pos(obj, 393, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x674d35), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_BPMLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bpm_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "BPM");
                        }
                        {
                            // ui_BPMSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_bpm_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 40, 240);
                            lv_slider_set_value(obj, 60, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_BPMValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_bpm_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_InputTrimLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_input_trim_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Input Trim");
                        }
                        {
                            // ui_InputTrimSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_input_trim_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -15, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_InputTrimValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_input_trim_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_TuningReferenceLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_tuning_reference_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Tuning Ref");
                        }
                        {
                            // ui_TuningReferenceSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_tuning_reference_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, 415, 465);
                            lv_slider_set_value(obj, 440, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_TuningReferenceValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_tuning_reference_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_VolumeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_volume_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Volume");
                        }
                        {
                            // ui_VolumeSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_volume_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -40, 3);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb9230), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_VolumeValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_volume_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
            }
        }
        {
            // ui_Closeimage
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.ui_closeimage = obj;
            lv_obj_set_pos(obj, 438, 293);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_tick);
            lv_obj_add_event_cb(obj, action_close_settings_page, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            // ui_settings_dialog
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ui_settings_dialog = obj;
            lv_obj_set_pos(obj, 11, 13);
            lv_obj_set_size(obj, 462, 298);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x81562e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ui_settings_text_entry
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.ui_settings_text_entry = obj;
                    lv_obj_set_pos(obj, 53, -14);
                    lv_obj_set_size(obj, 326, LV_SIZE_CONTENT);
                    lv_textarea_set_max_length(obj, 8);
                    lv_textarea_set_text(obj, "0");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xd1a60c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // ui_settings_keyboard
                    lv_obj_t *obj = lv_keyboard_create(parent_obj);
                    objects.ui_settings_keyboard = obj;
                    lv_obj_set_pos(obj, -18, 55);
                    lv_obj_set_size(obj, 462, 225);
                    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
                    lv_obj_add_event_cb(obj, action_value_keyboard_ok, LV_EVENT_READY, (void *)0);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.ui_settings_keyboard, objects.ui_settings_text_entry);
    
    tick_screen_settings();
}

void tick_screen_settings() {
}

void create_screen_val_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.val_settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ui_ValSettingsTabView
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 52);
            objects.ui_val_settings_tab_view = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 480, 320);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0x3b3c40), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_get_tab_btns(parent_obj);
                    objects.obj7 = obj;
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT, LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x96969e), LV_PART_ITEMS | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_ITEMS | LV_STATE_CHECKED);
                }
                {
                    // ui_ValNRTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "NR");
                    objects.ui_val_nr_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValNRBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValNRBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_nr_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -109);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValNRModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_nr_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Noise Gate");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj8 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValNRParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValNRParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValNRParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -36);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValNRParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValNRParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNRParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValNRParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_nr_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNRParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_nr_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValPreTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Pre");
                    objects.ui_val_pre_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValNPreBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_npre_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValPreBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_pre_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValPreModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_pre_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Comp\nComp4\nBoost\nMicro Boost\nB-Boost\nToucher\nCrier\nOcta\nPitch\nDetune");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj9 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValPreParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValPreParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValPreParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValPreParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValPreParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPreParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValPreParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_pre_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPreParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_pre_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValDstTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dst");
                    objects.ui_val_dst_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValDstBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValDstBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_dst_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValDstModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_dst_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Green OD\nYellow OD\nSuper OD\nSM Dist\nPlustortion\nLa Charger\nDarktale\nSora Fuzz\nRed Haze\nBass OD");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj10 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValDstParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValDstParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValDstParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValDstParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValDstParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDstParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValDstParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dst_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDstParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dst_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValAmpTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Amp");
                    objects.ui_val_amp_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValAmpBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValAmpBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_amp_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValAmpModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_amp_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Tweedy\nBellman 59N\nDark Twin\nFoxy 30N\nJ-120 CL\nMatch CL\nL-Star CL\nUK 45\nUK 50JP\nUK 800\nBellman 59B\nFoxy 30TB\nSup Dual OD\nSolo100 OD\nZ38 OD\nBad KT OD\nJuiceW R100\nDizz VH\nDizz VH+\nEagle 120\nEV51\nSolo100 LD\nMess Dual V\nMess Dual M\nPower LD\nFlagman+\nBog Red V\nClassic Bass\nFoxy Bass\nMess Bass\nAC Pre 1\nAC Pre 2\n");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj11 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValAmpParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValAmpParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValAmpParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValAmpParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValAmpParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValAmpParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValAmpParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_amp_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValAmpParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_amp_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValCabTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Cab");
                    objects.ui_val_cab_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValCabBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValCabBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_cab_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValCabModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_cab_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Twd CP\nDark VIT\nFoxy 1x12\nL-Star 1x12\nDark CS 2x12\nDark Twin 2x12\nSup Star 2x12\nJ-120 2x12\nFoxy 2x12\nUK Grn 2x12\nUK Grn 4x12\nBog 4x12\nDizz 4x12\nEV 4x12\nSolo 4x12\nMess 4x12\nEagle 4x12\nJuice 4x12\nBellman 2x12\nAmpg 4x10");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj12 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValCabParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValCabParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValCabParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValCabParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValCabParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValCabParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValCabParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_cab_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValCabParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_cab_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValEQTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "EQ");
                    objects.ui_val_eq_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValEQBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValEQBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_eq_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValEQModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_eq_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Guitar 1\nGuitar 2\nBass 1\nBass 2\nMess");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj13 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValEQParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValEQParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValEQParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValEQParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValEQParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValEQParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValEQParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_eq_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValEQParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_eq_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValModTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Mod");
                    objects.ui_val_mod_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValModBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValModBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_mod_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValModModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_mod_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "A-Chorus\nB-Chorus\nJet\nN-Jet\nO-Phase\nM-Vibe\nV-Roto\nVibrato\nO-Trem\nSine Trem\nBias Trem");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj14 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValModParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValModParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValModParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValModParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValModParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValModParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValModParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_mod_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValModParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_mod_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValDlyTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Dly");
                    objects.ui_val_dly_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValDlyBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValDlyBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_dly_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValDlyModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_dly_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Pure\nAnalog\nSlapback\nSweet Echo\nTape\nTube\nRev Echo\nRing Echo\nSweep Echo\nPing Pong");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj15 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValDlyParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValDlyParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValDlyParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValDlyParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValDlyParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValDlyParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValDlyParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_dly_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValDlyParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_dly_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValRvbTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Rvb");
                    objects.ui_val_rvb_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValRvbBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValRvbBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_rvb_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValRvbModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_rvb_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "Air\nRoom\nHall\nChurch\nPlate L\nPlate\nSpring\nN-Star\nDeepsea\nSweet Space");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj16 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValRvbParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValRvbParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValRvbParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValRvbParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValRvbParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValRvbParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValRvbParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_rvb_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValRvbParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_rvb_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValNSTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "NS");
                    objects.ui_val_ns_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValNSBlockLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_block_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Enable");
                        }
                        {
                            // ui_ValNSBlockSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_ns_block_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValNSModelDropdown
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.ui_val_ns_model_dropdown = obj;
                            lv_obj_set_pos(obj, 298, 1);
                            lv_obj_set_size(obj, 165, 38);
                            lv_dropdown_set_options_static(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n71\n72\n73\n74\n75\n76\n77\n78\n79");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_SELECTED | LV_STATE_CHECKED | LV_STATE_PRESSED);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_dropdown_get_list(parent_obj);
                                    objects.obj17 = obj;
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x1f1f1f), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_SELECTED | LV_STATE_CHECKED);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_SELECTED | LV_STATE_CHECKED);
                                }
                            }
                        }
                        {
                            // ui_ValNSParam0Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param0_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param0");
                        }
                        {
                            // ui_ValNSParam0Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param0_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam0Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param0_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam1Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param1_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param1");
                        }
                        {
                            // ui_ValNSParam1Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param1_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam1Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param1_value = obj;
                            lv_obj_set_pos(obj, 417, -34);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam2Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param2_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param2");
                        }
                        {
                            // ui_ValNSParam2Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param2_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam2Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param2_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam3Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param3_label = obj;
                            lv_obj_set_pos(obj, 4, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param3");
                        }
                        {
                            // ui_ValNSParam3Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param3_slider = obj;
                            lv_obj_set_pos(obj, 113, 42);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam3Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param3_value = obj;
                            lv_obj_set_pos(obj, 417, 42);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValNSParam4Label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param4_label = obj;
                            lv_obj_set_pos(obj, 4, 80);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Param4");
                        }
                        {
                            // ui_ValNSParam4Slider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_ns_param4_slider = obj;
                            lv_obj_set_pos(obj, 113, 80);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -100, -20);
                            lv_slider_set_value(obj, -64, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValNSParam4Value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_ns_param4_value = obj;
                            lv_obj_set_pos(obj, 417, 80);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
                {
                    // ui_ValGlobTab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Glb");
                    objects.ui_val_glob_tab = obj;
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfb9230), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x513d2b), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ui_ValGlobNoCabLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_no_cab_label = obj;
                            lv_obj_set_pos(obj, 4, -110);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "No Cab");
                        }
                        {
                            // ui_ValGlobNoCabSwitch
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ui_val_glob_no_cab_switch = obj;
                            lv_obj_set_pos(obj, 113, -110);
                            lv_obj_set_size(obj, 60, 24);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_CHECKED);
                        }
                        {
                            // ui_ValGlobInputLevelLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_input_level_label = obj;
                            lv_obj_set_pos(obj, 4, -72);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Input Lev");
                        }
                        {
                            // ui_ValGlobInputLevelSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_glob_input_level_slider = obj;
                            lv_obj_set_pos(obj, 113, -72);
                            lv_obj_set_size(obj, 290, 15);
                            lv_slider_set_range(obj, -20, 20);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValGlobInputLevelValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_input_level_value = obj;
                            lv_obj_set_pos(obj, 417, -72);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValGlobMasterVolLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_master_vol_label = obj;
                            lv_obj_set_pos(obj, 4, -34);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Master Vol");
                        }
                        {
                            // ui_ValGlobMasterVolSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_glob_master_vol_slider = obj;
                            lv_obj_set_pos(obj, 113, -34);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValGlobMasterVolValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_glob_master_vol_value = obj;
                            lv_obj_set_pos(obj, 417, -31);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                        {
                            // ui_ValPatchVolLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_patch_vol_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Patch Vol");
                        }
                        {
                            // ui_ValPatchVolSlider
                            lv_obj_t *obj = lv_slider_create(parent_obj);
                            objects.ui_val_patch_vol_slider = obj;
                            lv_obj_set_pos(obj, 113, 4);
                            lv_obj_set_size(obj, 290, 15);
                            lv_obj_add_event_cb(obj, action_parameter_changed, LV_EVENT_RELEASED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xaeb1b4), LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3890d5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // ui_ValPatchVolValue
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ui_val_patch_vol_value = obj;
                            lv_obj_set_pos(obj, 417, 4);
                            lv_obj_set_size(obj, 60, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_value_clicked, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "0");
                        }
                    }
                }
            }
        }
        {
            // ui_ValCloseimage
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.ui_val_closeimage = obj;
            lv_obj_set_pos(obj, 438, 293);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_tick);
            lv_obj_add_event_cb(obj, action_close_settings_page, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            // ui_val_settings_dialog
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ui_val_settings_dialog = obj;
            lv_obj_set_pos(obj, 11, 13);
            lv_obj_set_size(obj, 462, 298);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x386a91), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ui_val_settings_text_entry
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.ui_val_settings_text_entry = obj;
                    lv_obj_set_pos(obj, 53, -14);
                    lv_obj_set_size(obj, 326, LV_SIZE_CONTENT);
                    lv_textarea_set_max_length(obj, 8);
                    lv_textarea_set_text(obj, "0");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xd1a60c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // ui_val_settings_keyboard
                    lv_obj_t *obj = lv_keyboard_create(parent_obj);
                    objects.ui_val_settings_keyboard = obj;
                    lv_obj_set_pos(obj, -18, 55);
                    lv_obj_set_size(obj, 462, 225);
                    lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
                    lv_obj_add_event_cb(obj, action_value_keyboard_ok, LV_EVENT_READY, (void *)0);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    lv_keyboard_set_textarea(objects.ui_val_settings_keyboard, objects.ui_val_settings_text_entry);
    
    tick_screen_val_settings();
}

void tick_screen_val_settings() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_screen1,
    tick_screen_settings,
    tick_screen_val_settings,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 3) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_screen1();
    create_screen_settings();
    create_screen_val_settings();
}