# LVGL desktop simulator for the new 480x320 main screen.
#
# Build (MSYS2 MinGW64 shell):
#   pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake \
#                       mingw-w64-x86_64-ninja mingw-w64-x86_64-SDL2
#   cd simulator
#   cmake -S . -B build -G Ninja
#   cmake --build build
#   ./build/tonex_sim.exe

cmake_minimum_required(VERSION 3.15)
project(tonex_sim C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Warnings on, but don't treat as errors — the imported screens.c has
# a few cast/signedness niggles we don't want to fight.
add_compile_options(-Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable)

# ─── SDL2 ──────────────────────────────────────────────────────────────────
# MSYS2's mingw-w64-x86_64-SDL2 package installs an sdl2.pc.
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

# ─── LVGL 8.3.11 ───────────────────────────────────────────────────────────
# Pinned to match what the firmware uses (idf_component.yml: lvgl/lvgl ~8.3.0).
# LVGL's CMake build looks for lv_conf.h via LV_CONF_INCLUDE_SIMPLE in the
# include path — we add the simulator dir below so it finds ours.
include(FetchContent)
FetchContent_Declare(
    lvgl
    GIT_REPOSITORY https://github.com/lvgl/lvgl.git
    GIT_TAG        v8.3.11
    GIT_SHALLOW    TRUE
)
set(LV_CONF_BUILD_DISABLE_THORVG_INTERNAL ON CACHE BOOL "" FORCE)
set(LV_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h" CACHE STRING "" FORCE)
FetchContent_MakeAvailable(lvgl)

target_include_directories(lvgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE)

# ─── UI sources from the firmware tree ────────────────────────────────────
# Pointed at the EEZ-generated folder so the sim renders whatever EEZ Studio
# produces. Remember to hit Build in EEZ Studio after editing the .eez-project
# so this folder is up-to-date before rebuilding the sim.
set(UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../source/main/ui_generated_480x320land")
file(GLOB UI_IMAGE_SOURCES "${UI_DIR}/ui_image_*.c")
file(GLOB UI_FONT_SOURCES  "${UI_DIR}/ui_font_*.c")

add_executable(tonex_sim
    main.c
    stub_actions.c
    "${UI_DIR}/screens.c"
    "${UI_DIR}/screens_compat.c"
    "${UI_DIR}/ui.c"
    "${UI_DIR}/styles.c"
    "${UI_DIR}/images.c"
    ${UI_IMAGE_SOURCES}
    ${UI_FONT_SOURCES}
)

target_include_directories(tonex_sim PRIVATE
    "${UI_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(tonex_sim PRIVATE
    lvgl
    PkgConfig::SDL2
)

# On Windows MinGW, SDL2 needs the main wrapper; pkg-config's `sdl2` already
# pulls in mingw32 + SDL2main + SDL2 in the right order, but we also need
# console output so don't pass -mwindows.
if(WIN32)
    target_link_libraries(tonex_sim PRIVATE -lmingw32)
endif()

# Stage SDL2.dll next to the exe so `./tonex_sim.exe` runs without PATH hacks.
add_custom_command(TARGET tonex_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
            "If SDL2.dll is missing next to the exe, copy it from /mingw64/bin manually."
    VERBATIM
)
