#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: Modern
lv_style_t *get_style_modern_MAIN_DEFAULT();
lv_style_t *get_style_modern_MAIN_CHECKED();
lv_style_t *get_style_modern_KNOB_DEFAULT();
lv_style_t *get_style_modern_KNOB_CHECKED();
lv_style_t *get_style_modern_INDICATOR_CHECKED();
void add_style_modern(lv_obj_t *obj);
void remove_style_modern(lv_obj_t *obj);

// Style: Arc_slider
lv_style_t *get_style_arc_slider_MAIN_DEFAULT();
lv_style_t *get_style_arc_slider_INDICATOR_DEFAULT();
lv_style_t *get_style_arc_slider_KNOB_DEFAULT();
void add_style_arc_slider(lv_obj_t *obj);
void remove_style_arc_slider(lv_obj_t *obj);

// Style: modern_dropdown
lv_style_t *get_style_modern_dropdown_MAIN_DEFAULT();
void add_style_modern_dropdown(lv_obj_t *obj);
void remove_style_modern_dropdown(lv_obj_t *obj);

// Style: modern_list
lv_style_t *get_style_modern_list_MAIN_DEFAULT();
lv_style_t *get_style_modern_list_SELECTED_CHECKED();
lv_style_t *get_style_modern_list_SELECTED_DEFAULT();
void add_style_modern_list(lv_obj_t *obj);
void remove_style_modern_list(lv_obj_t *obj);

// Style: Setting_panel_style
lv_style_t *get_style_setting_panel_style_MAIN_CHECKED();
lv_style_t *get_style_setting_panel_style_MAIN_DEFAULT();
void add_style_setting_panel_style(lv_obj_t *obj);
void remove_style_setting_panel_style(lv_obj_t *obj);

// Style: Settings_label
lv_style_t *get_style_settings_label_MAIN_DEFAULT();
void add_style_settings_label(lv_obj_t *obj);
void remove_style_settings_label(lv_obj_t *obj);

// Style: modern_Slider
lv_style_t *get_style_modern_slider_MAIN_DEFAULT();
lv_style_t *get_style_modern_slider_INDICATOR_DEFAULT();
lv_style_t *get_style_modern_slider_KNOB_DEFAULT();
void add_style_modern_slider(lv_obj_t *obj);
void remove_style_modern_slider(lv_obj_t *obj);

// Style: modern_slider
lv_style_t *get_style_modern_slider1_MAIN_DEFAULT();
lv_style_t *get_style_modern_slider1_INDICATOR_DEFAULT();
lv_style_t *get_style_modern_slider1_KNOB_DEFAULT();
void add_style_modern_slider1(lv_obj_t *obj);
void remove_style_modern_slider1(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/