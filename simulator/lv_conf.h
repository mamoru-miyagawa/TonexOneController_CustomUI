/*
 * LVGL config for the desktop simulator.
 * Mirrors the relevant bits of the ESP-IDF Kconfig (source/sdkconfig.ws35b)
 * so that the same screens.c renders identically on PC and on the 3.5B.
 *
 * Only options that differ from LVGL's defaults are set explicitly.
 * Everything else falls back to lv_conf_internal.h defaults.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* ─── Color depth & format ─────────────────────────────────────────────── */
/* Device is RGB565 (16-bit) with COLOR_16_SWAP=1 for the BGR LCD panel.
 * On the PC sim we keep RGB565 but disable the swap (SDL renders RGB565 native). */
#define LV_COLOR_DEPTH       16
#define LV_COLOR_16_SWAP     0
#define LV_COLOR_SCREEN_TRANSP 0

/* ─── Memory ───────────────────────────────────────────────────────────── */
#define LV_MEM_CUSTOM        0
#define LV_MEM_SIZE          (256U * 1024U)
#define LV_MEM_ADR           0

/* ─── Tick / refresh ───────────────────────────────────────────────────── */
#define LV_DISP_DEF_REFR_PERIOD   16   /* ~60 Hz on PC */
#define LV_INDEV_DEF_READ_PERIOD  16

/* SDL_GetTicks() returns ms since SDL_Init — wire that in as the LVGL tick. */
#define LV_TICK_CUSTOM              1
#define LV_TICK_CUSTOM_INCLUDE      "SDL2/SDL.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (SDL_GetTicks())

#define LV_DPI_DEF 130

/* ─── Drawing ──────────────────────────────────────────────────────────── */
#define LV_DRAW_COMPLEX            1
#define LV_SHADOW_CACHE_SIZE       0
#define LV_CIRCLE_CACHE_SIZE       4
#define LV_LAYER_SIMPLE_BUF_SIZE   (24 * 1024)
#define LV_IMG_CACHE_DEF_SIZE      0
#define LV_GRADIENT_MAX_STOPS      2
#define LV_GRAD_CACHE_DEF_SIZE     0
#define LV_DISP_ROT_MAX_BUF        (10 * 1024)

/* ─── HAL / OS ─────────────────────────────────────────────────────────── */
#define LV_USE_OS  LV_OS_NONE
#define LV_USE_GPU_NXP_PXP         0
#define LV_USE_GPU_NXP_VG_LITE     0
#define LV_USE_GPU_SDL             0

/* ─── Logging ──────────────────────────────────────────────────────────── */
#define LV_USE_LOG    1
#define LV_LOG_LEVEL  LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 1

/* ─── Asserts ──────────────────────────────────────────────────────────── */
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0
#define LV_ASSERT_HANDLER_INCLUDE   <stdint.h>
#define LV_ASSERT_HANDLER           while(1);

/* ─── Misc ─────────────────────────────────────────────────────────────── */
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR  0
#define LV_USE_REFR_DEBUG   0
#define LV_SPRINTF_CUSTOM   0
#define LV_SPRINTF_USE_FLOAT 0
#define LV_USE_USER_DATA    1
#define LV_ENABLE_GLOBAL_CUSTOM 0

/* ─── Theme ────────────────────────────────────────────────────────────── */
#define LV_USE_THEME_DEFAULT       1
#define LV_THEME_DEFAULT_DARK      1
#define LV_THEME_DEFAULT_GROW      0
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#define LV_USE_THEME_BASIC         1
#define LV_USE_THEME_MONO          0

/* ─── Layouts (need flex for some defaults) ────────────────────────────── */
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/* ─── Fonts (match sdkconfig.ws35b enabled list) ───────────────────────── */
#define LV_FONT_MONTSERRAT_8   0
#define LV_FONT_MONTSERRAT_10  1
#define LV_FONT_MONTSERRAT_12  1
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_MONTSERRAT_16  0
#define LV_FONT_MONTSERRAT_18  1
#define LV_FONT_MONTSERRAT_20  1   /* used by create_screen_settings  */
#define LV_FONT_MONTSERRAT_22  1
#define LV_FONT_MONTSERRAT_24  1   /* used by create_screen_settings  */
#define LV_FONT_MONTSERRAT_26  0
#define LV_FONT_MONTSERRAT_28  1
#define LV_FONT_MONTSERRAT_30  0
#define LV_FONT_MONTSERRAT_32  1
#define LV_FONT_MONTSERRAT_34  1   /* used by ui_generated_480x320land/screens.c hero label */
#define LV_FONT_MONTSERRAT_36  1   /* used by create_screen_val_settings */
#define LV_FONT_MONTSERRAT_38  0
#define LV_FONT_MONTSERRAT_40  1   /* used by create_screen_val_settings */
#define LV_FONT_MONTSERRAT_42  0
#define LV_FONT_MONTSERRAT_44  0
#define LV_FONT_MONTSERRAT_46  0
#define LV_FONT_MONTSERRAT_48  0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_16_CJK            0
#define LV_FONT_UNSCII_8                 0
#define LV_FONT_UNSCII_16                0
#define LV_FONT_CUSTOM                   0
#define LV_FONT_FMT_TXT_LARGE            1   /* ui_font_anton_80 needs wider bitmap_index — its 1 MB+ glyph table overflows the 20-bit default */
#define LV_USE_FONT_COMPRESSED           0
#define LV_USE_FONT_SUBPX                0

#define LV_FONT_DEFAULT &lv_font_montserrat_14

/* ─── Text engine ──────────────────────────────────────────────────────── */
#define LV_TXT_ENC                  LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS          " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN  0
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3
#define LV_TXT_COLOR_CMD            "#"
#define LV_USE_BIDI                 0
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/* ─── Widgets ──────────────────────────────────────────────────────────── */
#define LV_USE_ARC          1
#define LV_USE_BAR          1
#define LV_USE_BTN          1
#define LV_USE_BTNMATRIX    1
#define LV_USE_CANVAS       0
#define LV_USE_CHECKBOX     1
#define LV_USE_DROPDOWN     1
#define LV_USE_IMG          1
#define LV_USE_LABEL        1
#define LV_LABEL_TEXT_SELECTION 0
#define LV_LABEL_LONG_TXT_HINT  1
#define LV_USE_LINE         1
#define LV_USE_ROLLER       1
#define LV_ROLLER_INF_PAGES 7
#define LV_USE_SLIDER       1
#define LV_USE_SWITCH       1
#define LV_USE_TEXTAREA     1
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500
#define LV_USE_TABLE        1

/* ─── Extras ───────────────────────────────────────────────────────────── */
#define LV_USE_ANIMIMG      0
#define LV_USE_CALENDAR     0
#define LV_USE_CHART        0
#define LV_USE_COLORWHEEL   0
#define LV_USE_IMGBTN       0
#define LV_USE_KEYBOARD     1
#define LV_USE_LED          0
#define LV_USE_LIST         0
#define LV_USE_MENU         0
#define LV_USE_METER        1   /* used by EEZ-generated settings screen knob (screens.c create_screen_settings) */
#define LV_USE_MSGBOX       0
#define LV_USE_SPAN         0
#define LV_USE_SPINBOX      0
#define LV_USE_SPINNER      0
#define LV_USE_TABVIEW      1
#define LV_USE_TILEVIEW     0
#define LV_USE_WIN          0

/* ─── Snapshot / file system ──────────────────────────────────────────── */
#define LV_USE_SNAPSHOT     0
#define LV_USE_MONKEY       0
#define LV_USE_GRIDNAV      0
#define LV_USE_FRAGMENT     0
#define LV_USE_IMGFONT      0
#define LV_USE_MSG          0
#define LV_USE_IME_PINYIN   0
#define LV_USE_FILE_EXPLORER 0
#define LV_USE_FS_STDIO     0
#define LV_USE_FS_POSIX     0
#define LV_USE_FS_WIN32     0
#define LV_USE_FS_FATFS     0
#define LV_USE_PNG          0
#define LV_USE_BMP          0
#define LV_USE_SJPG         0
#define LV_USE_GIF          0
#define LV_USE_QRCODE       0
#define LV_USE_FREETYPE     0
#define LV_USE_TINY_TTF     0
#define LV_USE_RLOTTIE      0
#define LV_USE_FFMPEG       0

/* ─── Misc legacy ──────────────────────────────────────────────────────── */
#define LV_BUILD_EXAMPLES   0
#define LV_USE_DEMO_WIDGETS 0
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0
#define LV_USE_DEMO_BENCHMARK          0
#define LV_USE_DEMO_STRESS             0
#define LV_USE_DEMO_MUSIC              0

#endif /* LV_CONF_H */
