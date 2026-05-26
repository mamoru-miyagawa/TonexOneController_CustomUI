#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: Modern
//

void init_style_modern_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_LEFT_MID);
    lv_style_set_bg_color(style, lv_color_hex(0x000000));
    lv_style_set_border_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_border_width(style, 2);
    lv_style_set_radius(style, 0);
};

lv_style_t *get_style_modern_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_modern_MAIN_CHECKED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x000000));
    lv_style_set_border_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_border_width(style, 2);
};

lv_style_t *get_style_modern_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_MAIN_CHECKED(style);
    }
    return style;
};

void init_style_modern_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x363f4f));
    lv_style_set_radius(style, 0);
    lv_style_set_pad_right(style, 5);
};

lv_style_t *get_style_modern_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_KNOB_DEFAULT(style);
    }
    return style;
};

void init_style_modern_KNOB_CHECKED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x6ed8bf));
    lv_style_set_pad_right(style, -4);
    lv_style_set_pad_left(style, 5);
};

lv_style_t *get_style_modern_KNOB_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_KNOB_CHECKED(style);
    }
    return style;
};

void init_style_modern_INDICATOR_CHECKED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x001913));
    lv_style_set_radius(style, 0);
    lv_style_set_border_color(style, lv_color_hex(0x6ed8bf));
    lv_style_set_border_width(style, 2);
};

lv_style_t *get_style_modern_INDICATOR_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_INDICATOR_CHECKED(style);
    }
    return style;
};

void add_style_modern(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_modern_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_modern_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_KNOB_CHECKED(), LV_PART_KNOB | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_modern_INDICATOR_CHECKED(), LV_PART_INDICATOR | LV_STATE_CHECKED);
};

void remove_style_modern(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_modern_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_modern_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_KNOB_CHECKED(), LV_PART_KNOB | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_modern_INDICATOR_CHECKED(), LV_PART_INDICATOR | LV_STATE_CHECKED);
};

//
// Style: Arc_slider
//

void init_style_arc_slider_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_arc_width(style, 2);
    lv_style_set_arc_color(style, lv_color_hex(0x001913));
    lv_style_set_arc_rounded(style, false);
    lv_style_set_base_dir(style, LV_BASE_DIR_AUTO);
};

lv_style_t *get_style_arc_slider_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_arc_slider_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_arc_slider_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_arc_rounded(style, false);
    lv_style_set_arc_width(style, 2);
    lv_style_set_arc_color(style, lv_color_hex(0x6ed8bf));
};

lv_style_t *get_style_arc_slider_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_arc_slider_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_arc_slider_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x7dd8bf));
};

lv_style_t *get_style_arc_slider_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_arc_slider_KNOB_DEFAULT(style);
    }
    return style;
};

void add_style_arc_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_arc_slider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_arc_slider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_arc_slider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

void remove_style_arc_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_arc_slider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_arc_slider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_arc_slider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

//
// Style: modern_dropdown
//

void init_style_modern_dropdown_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x373f50));
    lv_style_set_height(style, 28);
    lv_style_set_text_font(style, &lv_font_montserrat_14);
    lv_style_set_pad_top(style, 6);
    lv_style_set_outline_width(style, 2);
    lv_style_set_outline_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_outline_pad(style, 2);
    lv_style_set_border_width(style, 0);
    lv_style_set_clip_corner(style, false);
    lv_style_set_radius(style, 0);
};

lv_style_t *get_style_modern_dropdown_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_dropdown_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_modern_dropdown(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_modern_dropdown_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_modern_dropdown(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_modern_dropdown_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: modern_list
//

void init_style_modern_list_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x282b30));
    lv_style_set_border_width(style, 0);
};

lv_style_t *get_style_modern_list_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_list_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_modern_list_SELECTED_CHECKED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x6fd9c0));
};

lv_style_t *get_style_modern_list_SELECTED_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_list_SELECTED_CHECKED(style);
    }
    return style;
};

void init_style_modern_list_SELECTED_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x282b30));
};

lv_style_t *get_style_modern_list_SELECTED_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_list_SELECTED_DEFAULT(style);
    }
    return style;
};

void add_style_modern_list(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_modern_list_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_list_SELECTED_CHECKED(), LV_PART_SELECTED | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_modern_list_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
};

void remove_style_modern_list(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_modern_list_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_list_SELECTED_CHECKED(), LV_PART_SELECTED | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_modern_list_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
};

//
// Style: Setting_panel_style
//

void init_style_setting_panel_style_MAIN_CHECKED(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xfb9230));
    lv_style_set_bg_color(style, lv_color_hex(0x513d2b));
    lv_style_set_text_font(style, &lv_font_montserrat_20);
};

lv_style_t *get_style_setting_panel_style_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_setting_panel_style_MAIN_CHECKED(style);
    }
    return style;
};

void init_style_setting_panel_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_20);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
    lv_style_set_border_color(style, lv_color_hex(0x6ed8bf));
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_TOP);
};

lv_style_t *get_style_setting_panel_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_setting_panel_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_setting_panel_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_setting_panel_style_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_setting_panel_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_setting_panel_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_setting_panel_style_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_setting_panel_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: Settings_label
//

void init_style_settings_label_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_m_plus_u_bold_14);
    lv_style_set_align(style, LV_ALIGN_CENTER);
    lv_style_set_text_color(style, lv_color_hex(0xeee9db));
};

lv_style_t *get_style_settings_label_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_settings_label_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_settings_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_settings_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_settings_label(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_settings_label_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: modern_Slider
//

void init_style_modern_slider_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_LEFT_MID);
    lv_style_set_bg_color(style, lv_color_hex(0x001913));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_radius(style, 0);
    lv_style_set_outline_width(style, 2);
    lv_style_set_outline_pad(style, 2);
    lv_style_set_outline_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_width(style, 300);
    lv_style_set_height(style, 10);
};

lv_style_t *get_style_modern_slider_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_slider_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_modern_slider_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_radius(style, 0);
    lv_style_set_outline_width(style, 2);
    lv_style_set_outline_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_outline_pad(style, 2);
};

lv_style_t *get_style_modern_slider_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_slider_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_modern_slider_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x6fd9c0));
    lv_style_set_radius(style, 0);
};

lv_style_t *get_style_modern_slider_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_slider_KNOB_DEFAULT(style);
    }
    return style;
};

void add_style_modern_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_modern_slider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_slider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_slider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

void remove_style_modern_slider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_modern_slider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_slider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_slider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

//
// Style: modern_slider
//

void init_style_modern_slider1_MAIN_DEFAULT(lv_style_t *style) {
    init_style_modern_slider_MAIN_DEFAULT(style);
    
    lv_style_set_width(style, 300);
    lv_style_set_height(style, 10);
};

lv_style_t *get_style_modern_slider1_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_slider1_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_modern_slider1_INDICATOR_DEFAULT(lv_style_t *style) {
    init_style_modern_slider_INDICATOR_DEFAULT(style);
    
};

lv_style_t *get_style_modern_slider1_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_slider1_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_modern_slider1_KNOB_DEFAULT(lv_style_t *style) {
    init_style_modern_slider_KNOB_DEFAULT(style);
    
};

lv_style_t *get_style_modern_slider1_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_modern_slider1_KNOB_DEFAULT(style);
    }
    return style;
};

void add_style_modern_slider1(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_modern_slider1_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_slider1_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_modern_slider1_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

void remove_style_modern_slider1(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_modern_slider1_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_slider1_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_modern_slider1_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_modern,
        add_style_arc_slider,
        add_style_modern_dropdown,
        add_style_modern_list,
        add_style_setting_panel_style,
        add_style_settings_label,
        add_style_modern_slider,
        add_style_modern_slider1,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_modern,
        remove_style_arc_slider,
        remove_style_modern_dropdown,
        remove_style_modern_list,
        remove_style_setting_panel_style,
        remove_style_settings_label,
        remove_style_modern_slider,
        remove_style_modern_slider1,
    };
    remove_style_funcs[styleIndex](obj);
}