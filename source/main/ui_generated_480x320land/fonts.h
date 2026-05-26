#ifndef EEZ_LVGL_UI_FONTS_H
#define EEZ_LVGL_UI_FONTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_font_t ui_font_anton_14;
extern const lv_font_t ui_font_anton_28;
extern const lv_font_t ui_font_anton_36;
extern const lv_font_t ui_font_anton_58;
extern const lv_font_t ui_font_m_plus_u_bold_16;
extern const lv_font_t ui_font_m_plus_u_bold_10;
extern const lv_font_t ui_font_anton_80;
extern const lv_font_t ui_font_m_plus_u_bold_14;
extern const lv_font_t ui_font_anton_40;

#ifndef EXT_FONT_DESC_T
#define EXT_FONT_DESC_T
typedef struct _ext_font_desc_t {
    const char *name;
    const void *font_ptr;
} ext_font_desc_t;
#endif

extern ext_font_desc_t fonts[];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_FONTS_H*/