/*
 * Stubs for every action_* symbol declared in actions.h.
 *
 * On the device these live in display.c / display_tonex.c and call into the
 * real control / USB / params layers. None of that builds on PC, so the
 * simulator stubs just log clicks and — for PREV/NEXT — cycle a small fixed
 * preset table so you can see how the hero/header/chain react to state.
 *
 * Mirror DESIGN_SPEC §7 (export/DESIGN_SPEC.md) for the seed data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lvgl.h"
#include "screens.h"
#include "images.h"
#include "actions.h"
#include "ui.h"

/* ─── Seed data ─────────────────────────────────────────────────────────── */
/* Matches the device: preset number is a single integer (1..N). display.c
   writes it via sprintf("%d", value+1) into ui_bank_value_label. No bank
   letters — that was DESIGN_SPEC fiction. */
typedef struct {
    const char *slot;          /* shown in the amber header pill — just "1", "2"… */
    const char *name;          /* big hero label */
    const char *bpm;           /* hero BPM number */
    /* effect on/off mask — order: gate, comp, eq, amp, cab, mod, dly, rvb */
    unsigned char on[8];
} sim_preset_t;

static const sim_preset_t presets[] = {
    /* slot  name              bpm    gate comp eq amp cab mod dly rvb */
    { "1",  "Smythbuilt OD",  "124", {1,0,1,1,1,0,0,1} },
    { "2",  "JCM 800 Lead",   "132", {1,0,1,1,1,0,1,1} },
    { "3",  "Twin Sparkle",   "96",  {0,1,1,1,1,1,0,1} },
    { "4",  "Klon Gold Push", "110", {1,0,1,1,1,0,1,1} },
    { "5",  "Big Muff Doom",  "72",  {1,0,1,1,1,0,0,1} },
    { "6",  "5150 Modern",    "140", {1,0,1,1,1,0,1,1} },
    { "7",  "Plexi Brown",    "118", {1,0,1,1,1,1,0,1} },
    { "8",  "Studio Black",   "100", {0,0,0,1,1,0,0,0} },
};
static const int N_PRESETS = (int)(sizeof(presets) / sizeof(presets[0]));
static int current = 0;
/* mutable mirror of presets[current].on so chip clicks can toggle without
   modifying the const seed table */
static unsigned char live_on[8] = {0};

static const lv_img_dsc_t *chip_img(int fx, int on) {
    /* fx index matches the sim_preset_t.on order */
    switch (fx) {
        case 0: return on ? &img_effect_icon_gate_on   : &img_effect_icon_gate_off;
        case 1: return on ? &img_effect_icon_comp_on   : &img_effect_icon_comp_off;
        case 2: return &img_effect_icon_eq;
        case 3: return on ? &img_effect_icon_amp_on    : &img_effect_icon_amp_off;
        case 4: return on ? &img_effect_icon_cab_on    : &img_effect_icon_cab_off;
        case 5: return on ? &img_effect_icon_mod_on    : &img_effect_icon_mod_off;
        case 6: return on ? &img_effect_icon_delay_on  : &img_effect_icon_delay_off;
        case 7: return on ? &img_effect_icon_reverb_on : &img_effect_icon_reverb_off;
        default: return NULL;
    }
}

static lv_obj_t *chip_icon(int fx) {
    switch (fx) {
        case 0: return objects.ui_icon_gate;
        case 1: return objects.ui_icon_comp;
        case 2: return objects.ui_icon_eq;
        case 3: return objects.ui_icon_amp;
        case 4: return objects.ui_icon_cab;
        case 5: return objects.ui_icon_mod;
        case 6: return objects.ui_icon_delay;
        case 7: return objects.ui_icon_reverb;
        default: return NULL;
    }
}

static lv_obj_t *chip_frame(int fx) {
    switch (fx) {
        case 0: return objects.ui_chip_gate;
        case 1: return objects.ui_chip_comp;
        case 2: return objects.ui_chip_eq;
        case 3: return objects.ui_chip_amp;
        case 4: return objects.ui_chip_cab;
        case 5: return objects.ui_chip_mod;
        case 6: return objects.ui_chip_delay;
        case 7: return objects.ui_chip_reverb;
        default: return NULL;
    }
}

/* Apply on/off state to a chip: swap the inner icon AND toggle CHECKED on
   the container so the frame color flips. Mirrors what display_tonex.c does
   on the device. */
static void set_chip_state(int fx, int on) {
    lv_obj_t *icon  = chip_icon(fx);
    lv_obj_t *frame = chip_frame(fx);
    const lv_img_dsc_t *src = chip_img(fx, on);
    if (icon && src) lv_img_set_src(icon, src);
    if (frame) {
        if (on) lv_obj_add_state(frame, LV_STATE_CHECKED);
        else    lv_obj_clear_state(frame, LV_STATE_CHECKED);
    }
}

static void apply_preset(int idx) {
    const sim_preset_t *p = &presets[idx];
    memcpy(live_on, p->on, sizeof(live_on));

    if (objects.ui_preset_heading_label) lv_label_set_text(objects.ui_preset_heading_label, p->name);
    if (objects.ui_bank_value_label)     lv_label_set_text(objects.ui_bank_value_label,     p->slot);
    if (objects.ui_bpm_value_label)      lv_label_set_text(objects.ui_bpm_value_label,      p->bpm);

    for (int i = 0; i < 8; i++) {
        set_chip_state(i, live_on[i]);
    }
    fprintf(stderr, "[sim] preset %s: %s @ %s BPM\n", p->slot, p->name, p->bpm);
}

/* Public — called from main.c after ui_init() to populate seed data. */
void sim_seed_initial_state(void) {
    apply_preset(0);
}

/* ─── Stubs ─────────────────────────────────────────────────────────────── */
void action_previous_clicked(lv_event_t *e) {
    (void)e;
    current = (current - 1 + N_PRESETS) % N_PRESETS;
    apply_preset(current);
}
void action_next_clicked(lv_event_t *e) {
    (void)e;
    current = (current + 1) % N_PRESETS;
    apply_preset(current);
}

/* Tab index per chip — matches enum ConfigTabs43BTonex in control.h. */
static int tab_for_chip(int fx) {
    switch (fx) {
        case 0: return 0; /* gate   -> CONFIG_TAB_GATE        */
        case 1: return 1; /* comp   -> CONFIG_TAB_COMPRESSOR  */
        case 2: return 3; /* eq     -> CONFIG_TAB_EQ          */
        case 3: return 2; /* amp    -> CONFIG_TAB_AMPLIFIER   */
        case 4: return 2; /* cab    -> CONFIG_TAB_AMPLIFIER (cab lives on the amp tab) */
        case 5: return 4; /* mod    -> CONFIG_TAB_MODULATION  */
        case 6: return 5; /* delay  -> CONFIG_TAB_DELAY       */
        case 7: return 6; /* reverb -> CONFIG_TAB_REVERB      */
        default: return 0;
    }
}

void action_effect_icon_clicked(lv_event_t *e) {
    lv_obj_t *t = lv_event_get_target(e);
    /* Click target is the chip container (ui_chip_X) under the handwritten
       layout; fall back to the inner icon for EEZ layouts. */
    int fx = -1;
    if      (t == objects.ui_chip_gate   || t == objects.ui_icon_gate)   fx = 0;
    else if (t == objects.ui_chip_comp   || t == objects.ui_icon_comp)   fx = 1;
    else if (t == objects.ui_chip_eq     || t == objects.ui_icon_eq)     fx = 2;
    else if (t == objects.ui_chip_amp    || t == objects.ui_icon_amp)    fx = 3;
    else if (t == objects.ui_chip_cab    || t == objects.ui_icon_cab)    fx = 4;
    else if (t == objects.ui_chip_mod    || t == objects.ui_icon_mod)    fx = 5;
    else if (t == objects.ui_chip_delay  || t == objects.ui_icon_delay)  fx = 6;
    else if (t == objects.ui_chip_reverb || t == objects.ui_icon_reverb) fx = 7;
    if (fx < 0) return;

    static const char *names[] = {"gate","comp","eq","amp","cab","mod","delay","reverb"};
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_LONG_PRESSED) {
        if (objects.ui_settings_tab_view) {
            lv_tabview_set_act(objects.ui_settings_tab_view, tab_for_chip(fx), LV_ANIM_OFF);
        }
        /* Use loadScreen() (not lv_scr_load_anim directly) so EEZ's currentScreen
           tracker advances — otherwise ui_tick() keeps ticking screen1 and the
           settings-screen flow bindings (arc -> bpm_gauge -> meter) never run. */
        loadScreen(SCREEN_ID_SETTINGS);
        fprintf(stderr, "[sim] long-press %s -> open detail tab %d\n", names[fx], tab_for_chip(fx));
        return;
    }

    if (code == LV_EVENT_SHORT_CLICKED) {
        /* Toggle the chip's on/off image AND CHECKED state — matches device
           behavior after a usb_modify_parameter flip. EQ has no on/off. */
        live_on[fx] = !live_on[fx];
        set_chip_state(fx, live_on[fx]);
        fprintf(stderr, "[sim] chip %s -> %s\n", names[fx], live_on[fx] ? "ON" : "OFF");
    }
}

void action_show_settings_page(lv_event_t *e) {
    (void)e;
    loadScreen(SCREEN_ID_SETTINGS);
}

void action_close_settings_page(lv_event_t *e) {
    (void)e;
    loadScreen(SCREEN_ID_SCREEN1);
}

void action_gesture(lv_event_t *e) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    fprintf(stderr, "[sim] gesture dir=%d\n", (int)dir);
}

/* EEZ flow-variable accessors. The firmware implements these against real
   BPM state; the sim just holds a static value so the BPM gauge widget on
   the settings screen doesn't crash. */
static int32_t sim_bpm_gauge = 120;
int32_t get_var_bpm_gauge(void)            { return sim_bpm_gauge; }
void    set_var_bpm_gauge(int32_t value)   { sim_bpm_gauge = value; }

/* The rest exist only so the linker is happy with screens.c's event_cb wiring. */
void action_amp_skin_next(lv_event_t *e)          { (void)e; }
void action_amp_skin_previous(lv_event_t *e)      { (void)e; }
void action_parameter_changed(lv_event_t *e)      { (void)e; }
void action_enable_skin_edit(lv_event_t *e)       { (void)e; fprintf(stderr, "[sim] skin-edit long-press (no-op in sim)\n"); }
void action_save_skin_edit(lv_event_t *e)         { (void)e; }
void action_keyboard_ok(lv_event_t *e)            { (void)e; }
void action_preset_description_pressed(lv_event_t *e) { (void)e; }
void action_value_keyboard_ok(lv_event_t *e)      { (void)e; }
void action_value_clicked(lv_event_t *e)          { (void)e; }
