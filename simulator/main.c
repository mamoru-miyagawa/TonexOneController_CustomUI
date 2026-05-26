/*
 * Desktop simulator entry point.
 *
 * Bridges SDL2 (window + mouse) to LVGL (display + indev), then hands off
 * to ui_init() from the firmware UI module. The same screens.c that runs on
 * the Waveshare 3.5B renders here at 1:1 device pixels (with optional 2x
 * upscale for legibility on a desktop).
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "lvgl.h"
#include "ui.h"

#define SIM_HOR_RES   480
#define SIM_VER_RES   320
#define SIM_ZOOM      2          /* on-screen scale factor */

/* LVGL draws into this RGB565 buffer; we blit it to SDL as a streaming texture. */
static lv_color_t lvgl_buf[SIM_HOR_RES * 40];
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t      disp_drv;
static lv_indev_drv_t     indev_drv;

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *texture;

/* Live mouse state, updated from SDL events and read by LVGL each tick. */
static int  mouse_x = 0;
static int  mouse_y = 0;
static bool mouse_pressed = false;
static bool quit = false;

/* ─── LVGL display flush ────────────────────────────────────────────────
 * LVGL hands us a rectangle of pixels in lv_color_t (RGB565 on this build).
 * SDL_UpdateTexture copies the strip into our streaming texture and we
 * present a full frame after every flush so partial refreshes are visible
 * immediately. */
static void disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *px) {
    SDL_Rect r = {
        .x = area->x1,
        .y = area->y1,
        .w = area->x2 - area->x1 + 1,
        .h = area->y2 - area->y1 + 1,
    };
    SDL_UpdateTexture(texture, &r, px, r.w * sizeof(lv_color_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    lv_disp_flush_ready(drv);
}

/* ─── LVGL touch input ──────────────────────────────────────────────────
 * Mouse becomes a single-point touch indev. We divide by SIM_ZOOM so
 * coordinates land in 480x320 device space regardless of window size. */
static void mouse_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    data->point.x = mouse_x / SIM_ZOOM;
    data->point.y = mouse_y / SIM_ZOOM;
    data->state   = mouse_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

/* ─── SDL event pump ──────────────────────────────────────────────────── */
static void pump_sdl(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT) mouse_pressed = true;
                mouse_x = e.button.x;
                mouse_y = e.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                if (e.button.button == SDL_BUTTON_LEFT) mouse_pressed = false;
                mouse_x = e.button.x;
                mouse_y = e.button.y;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
                break;
        }
    }
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Tonex Controller — 480x320 sim",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SIM_HOR_RES * SIM_ZOOM, SIM_VER_RES * SIM_ZOOM,
        SDL_WINDOW_SHOWN
    );
    if (!window) { fprintf(stderr, "CreateWindow: %s\n", SDL_GetError()); return 1; }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) { fprintf(stderr, "CreateRenderer: %s\n", SDL_GetError()); return 1; }

    /* Render texture at native device res, upscaled by SDL on present. */
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                SDL_TEXTUREACCESS_STREAMING,
                                SIM_HOR_RES, SIM_VER_RES);
    if (!texture) { fprintf(stderr, "CreateTexture: %s\n", SDL_GetError()); return 1; }

    /* ─── LVGL init ─── */
    lv_init();

    lv_disp_draw_buf_init(&draw_buf, lvgl_buf, NULL,
                          sizeof(lvgl_buf) / sizeof(lv_color_t));

    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.hor_res  = SIM_HOR_RES;
    disp_drv.ver_res  = SIM_VER_RES;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    lv_indev_drv_register(&indev_drv);

    /* ─── Build the UI ─── */
    ui_init();
    /* EEZ-generated layout doesn't create the ui_chip_X / ui_skin_image etc.
       fields that display.c and stub_actions.c reference. screens_compat_init
       fills them with hidden 0x0 placeholders so the pointers are valid. */
    extern void screens_compat_init(void);
    screens_compat_init();
    extern void sim_seed_initial_state(void);
    sim_seed_initial_state();

    fprintf(stderr, "[sim] running. PREV/NEXT cycle 8 preset stubs. Click chips for log. ESC to quit.\n");

    while (!quit) {
        pump_sdl();
        /* ui_tick() runs the per-screen tick handler EEZ generates (e.g.
           tick_screen_settings), which pumps flow-variable bindings such as
           bpm_gauge -> meter. lv_timer_handler() alone doesn't trigger it. */
        ui_tick();
        lv_timer_handler();
        SDL_Delay(5);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
