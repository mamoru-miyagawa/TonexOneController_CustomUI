#ifndef EEZ_LVGL_UI_SCREENS_COMPAT_H
#define EEZ_LVGL_UI_SCREENS_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Creates hidden 0x0 placeholder widgets for fields that exist in objects_t
   (see the COMPAT STUBS block in screens.h) but are not produced by the
   current EEZ design. Call once, after ui_init(). */
void screens_compat_init(void);

#ifdef __cplusplus
}
#endif

#endif /* EEZ_LVGL_UI_SCREENS_COMPAT_H */
