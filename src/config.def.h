/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2016 - Daniel De Matteis
 *  Copyright (C) 2016-2019 - Brad Parker
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CONFIG_DEF_H
#define __CONFIG_DEF_H

#include <boolean.h>
#include <audio/audio_resampler.h>
#include "configuration.h"
#include "gfx/video_defines.h"
#include "input/input_driver.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_NETWORKING
#include "network/netplay/netplay.h"
#endif

/* Required for 3DS display mode setting */
#if defined(_3DS)
#include "gfx/common/ctr_common.h"
#endif

/* Required for OpenDingux IPU filter + refresh
 * rate settings */
#if defined(DINGUX)
#include "dingux/dingux_utils.h"
#endif

/* Required for menu screensaver animation */
#if defined(HAVE_MATERIALUI) || defined(HAVE_XMB) || defined(HAVE_OZONE)
#include "menu/menu_screensaver.h"
#endif

/* Required for 'show inputs on overlay' setting */
#if defined(HAVE_OVERLAY)
#include "../input/input_overlay.h"
#endif

#if defined(HW_RVL)
#define MAX_GAMMA_SETTING 30
#elif defined(GEKKO)
#define MAX_GAMMA_SETTING 2
#else
#define MAX_GAMMA_SETTING 1
#endif

#if defined(XENON) || defined(_XBOX360) || defined(__PSL1GHT__) || defined(__PS3__)
#define DEFAULT_ASPECT_RATIO 1.7778f
#elif defined(_XBOX1) || defined(GEKKO) || defined(ANDROID)
#define DEFAULT_ASPECT_RATIO 1.3333f
#else
#define DEFAULT_ASPECT_RATIO 1.3333f
#endif

#if defined(GEKKO)
#define DEFAULT_MOUSE_SCALE 1
#endif

#define DEFAULT_TOUCH_SCALE 1

#if defined(RARCH_MOBILE) || defined(HAVE_LIBNX) || defined(__WINRT__) || defined(EMSCRIPTEN)
#define DEFAULT_POINTER_ENABLE true
#else
#define DEFAULT_POINTER_ENABLE false
#endif

/* Certain platforms might have assets stored in the bundle that
 * we need to extract to a user-writable directory on first boot.
 *
 * Examples include: Android, iOS/OSX) */
#if defined(ANDROID) || defined(__APPLE__)
#define DEFAULT_BUNDLE_ASSETS_EXTRACT_ENABLE true
#else
#define DEFAULT_BUNDLE_ASSETS_EXTRACT_ENABLE false
#endif

#ifdef HAVE_MATERIALUI
/* Show icons to the left of each menu entry */
#define DEFAULT_MATERIALUI_ICONS_ENABLE true
/* Show system-specific icons in the playlists tab */
#define DEFAULT_MATERIALUI_PLAYLIST_ICONS_ENABLE true
#endif

/* Material UI colour theme */
#define DEFAULT_MATERIALUI_THEME MATERIALUI_THEME_OZONE_DARK

/* Type of animation to use when performing menu transitions
 * > 'Auto' follows Material UI standards:
 *   - Slide when switching between parent menus (tabs)
 *   - Fade when changing levels in a menu
 * Note: Not wrapping this with a HAVE_MATERIALUI ifdef
 * because there's too much baggage involved... */
#define DEFAULT_MATERIALUI_TRANSITION_ANIM MATERIALUI_TRANSITION_ANIM_AUTO

/* Adjust menu padding etc. to better fit the
 * screen when using landscape layouts */
#if defined(RARCH_MOBILE)
#define DEFAULT_MATERIALUI_LANDSCAPE_LAYOUT_OPTIMIZATION MATERIALUI_LANDSCAPE_LAYOUT_OPTIMIZATION_DISABLED
#else
#define DEFAULT_MATERIALUI_LANDSCAPE_LAYOUT_OPTIMIZATION MATERIALUI_LANDSCAPE_LAYOUT_OPTIMIZATION_ALWAYS
#endif

/* Show/hide navigation bar
 * > When hidden, MaterialUI menu navigation
 *   behaves like RGUI */
#define DEFAULT_MATERIALUI_SHOW_NAV_BAR true

/* Reposition navigation bar to make better use
 * of screen space when using landscape layouts */
#define DEFAULT_MATERIALUI_AUTO_ROTATE_NAV_BAR true

/* Default portrait/landscape playlist view modes
 * (when thumbnails are enabled) */
#define DEFAULT_MATERIALUI_THUMBNAIL_VIEW_PORTRAIT MATERIALUI_THUMBNAIL_VIEW_PORTRAIT_DISABLED
#define DEFAULT_MATERIALUI_THUMBNAIL_VIEW_LANDSCAPE MATERIALUI_THUMBNAIL_VIEW_LANDSCAPE_DISABLED

/* Enable second thumbnail when using 'list view'
 * thumbnail views
 * Note: Second thumbnail will only be drawn if
 * display has sufficient horizontal real estate */
#if defined(RARCH_MOBILE)
#define DEFAULT_MATERIALUI_DUAL_THUMBNAIL_LIST_VIEW_ENABLE false
#else
#define DEFAULT_MATERIALUI_DUAL_THUMBNAIL_LIST_VIEW_ENABLE true
#endif

/* Draw solid colour 4:3 background when rendering
 * thumbnails
 * > Helps to unify menu appearance when viewing
 *   thumbnails of different sizes */
#define DEFAULT_MATERIALUI_THUMBNAIL_BACKGROUND_ENABLE true

#define DEFAULT_SCREEN_BRIGHTNESS 100

#define DEFAULT_CRT_SWITCH_RESOLUTION CRT_SWITCH_NONE

#define DEFAULT_CRT_SWITCH_RESOLUTION_SUPER 2560

#define DEFAULT_CRT_SWITCH_CENTER_ADJUST 0

#define DEFAULT_CRT_SWITCH_PORCH_ADJUST 0

#define DEFAULT_CRT_SWITCH_HIRES_MENU true

#define DEFAULT_HISTORY_LIST_ENABLE true

#define DEFAULT_PLAYLIST_ENTRY_RENAME true

#define DEFAULT_WIFI_ENABLE true

#define DEFAULT_ACCESSIBILITY_ENABLE false

#define DEFAULT_ACCESSIBILITY_NARRATOR_SPEECH_SPEED 5

#define DEFAULT_DRIVER_SWITCH_ENABLE true

#define DEFAULT_USER_LANGUAGE 0

#define DEFAULT_GAMEMODE_ENABLE true

#if (defined(_WIN32) && !defined(_XBOX)) || (defined(__linux) && !defined(ANDROID) && !defined(HAVE_LAKKA)) || (defined(__MACH__) && !defined(IOS)) || defined(EMSCRIPTEN)
#define DEFAULT_MOUSE_ENABLE true
#else
#define DEFAULT_MOUSE_ENABLE false
#endif

#ifdef HAVE_CHEEVOS
#define DEFAULT_CHEEVOS_ENABLE false
#endif

/* VIDEO */

#if defined(_XBOX360)
#define DEFAULT_GAMMA 1
#else
#define DEFAULT_GAMMA 0
#endif

/* Windowed
 * Real x resolution = aspect * base_size * x scale
 * Real y resolution = base_size * y scale
 */
#define DEFAULT_SCALE (3.0)

/* Fullscreen */

/* To start in Fullscreen, or not. */

#if defined(HAVE_STEAM) || defined(DINGUX) || defined(__WINRT__) || defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
/* Start in fullscreen mode for Steam and Dingux
 * WinRT and Winapi Family builds */
#define DEFAULT_FULLSCREEN true
#else
#define DEFAULT_FULLSCREEN false
#endif

/* To use windowed mode or not when going fullscreen. */
#if defined(__WINRT__) || defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
/* Do not use windowed mode for WinRT and Winapi Family builds on the Xbox UWP with fixed resolution shrinks the image into the left top corner of the screen with some libretro cores */
#define DEFAULT_WINDOWED_FULLSCREEN false
#else
#define DEFAULT_WINDOWED_FULLSCREEN true 
#endif 

/* Which monitor to prefer. 0 is any monitor, 1 and up selects
 * specific monitors, 1 being the first monitor. */
#define DEFAULT_MONITOR_INDEX 0

/* Window */

/* DEFAULT_WINDOW_DECORATIONS:
   Whether to show the usual window decorations like border, titlebar etc. */
#ifdef WEBOS
#define DEFAULT_WINDOW_DECORATIONS false
#else
#define DEFAULT_WINDOW_DECORATIONS true
#endif

/* Amount of transparency to use for the main window.
 * 1 is the most transparent while 100 is opaque. */
#define DEFAULT_WINDOW_OPACITY 100

/* DEFAULT_WINDOW_SAVE_POSITIONS:
 * Whether to remember window positions
 * NOTE: Only enabled for desktop Windows
 * at present. */
#define DEFAULT_WINDOW_SAVE_POSITIONS false

/* Whether to use custom (fixed size)
 * window dimensions in windowed mode. */
#ifdef WEBOS
#define DEFAULT_WINDOW_CUSTOM_SIZE_ENABLE true
#else
#define DEFAULT_WINDOW_CUSTOM_SIZE_ENABLE false
#endif

/* Window dimensions when using a fixed size
 * window. A value of 0 disables fixed size
 * windows, using nominal dimensions of
 * window scale multiplied by the core
 * framebuffer size. */
#if defined(WEBOS)
#define DEFAULT_WINDOW_WIDTH  1920
#define DEFAULT_WINDOW_HEIGHT 1080
#else
#define DEFAULT_WINDOW_WIDTH  1280
#define DEFAULT_WINDOW_HEIGHT 720
#endif

/* Maximum auto-set window dimensions
 * when not using a fixed size window */
#define DEFAULT_WINDOW_AUTO_WIDTH_MAX  1920
#define DEFAULT_WINDOW_AUTO_HEIGHT_MAX 1080

/* Fullscreen resolution. A value of 0 uses the desktop
 * resolution. */
#if defined(DINGUX)
#define DEFAULT_FULLSCREEN_X 320
#define DEFAULT_FULLSCREEN_Y 240
#elif defined(__WINRT__) || defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#define DEFAULT_FULLSCREEN_X 1920
#define DEFAULT_FULLSCREEN_Y 1080
#else
#define DEFAULT_FULLSCREEN_X 0
#define DEFAULT_FULLSCREEN_Y 0
#endif

#if defined(HAVE_WINDOW_OFFSET)
/* Screen offsets to center content in CTRs */
#define DEFAULT_WINDOW_OFFSET_X 0
#define DEFAULT_WINDOW_OFFSET_Y 0
#endif

/* Number of threads to use for video recording */
#define DEFAULT_VIDEO_RECORD_THREADS 2

#if defined(RARCH_CONSOLE) || defined(__APPLE__)
#define DEFAULT_LOAD_DUMMY_ON_CORE_SHUTDOWN false
#else
#define DEFAULT_LOAD_DUMMY_ON_CORE_SHUTDOWN true
#endif
#define DEFAULT_CHECK_FIRMWARE_BEFORE_LOADING false

/* Specifies whether cores are allowed to
 * present core options in category submenus */
#define DEFAULT_CORE_OPTION_CATEGORY_ENABLE true

/* Specifies whether to cache core info
 * into a single (compressed) file for improved
 * load times on platforms with slow IO */
#define DEFAULT_CORE_INFO_CACHE_ENABLE true

/* Specifies whether to 'reload' (fork and quit)
 * RetroArch when launching content with the
 * currently loaded core
 * > Only relevant on platforms without dynamic core
 *   loading support
 * > Setting this to 'false' will decrease loading
 *   times when required core is already running,
 *   but may cause stability issues (if core misbehaves) */
#ifndef HAVE_DYNAMIC
#define DEFAULT_ALWAYS_RELOAD_CORE_ON_RUN_CONTENT true
#endif

/* Forcibly disable composition.
 * Only valid on Windows Vista/7/8 for now. */
#define DEFAULT_DISABLE_COMPOSITION false

/* Video VSYNC (recommended) */
#define DEFAULT_VSYNC true

#define DEFAULT_MAX_SWAPCHAIN_IMAGES 3

#define DEFAULT_ADAPTIVE_VSYNC false

/* Attempts to hard-synchronize CPU and GPU.
 * Can reduce latency at cost of performance. */
#define DEFAULT_HARD_SYNC false

/* Configures how many frames the GPU can run ahead of CPU.
 * 0: Syncs to GPU immediately.
 * 1: Syncs to previous frame.
 * 2: Etc ...
 */
#define DEFAULT_HARD_SYNC_FRAMES 0

/* Sets how many milliseconds to delay after VSync before running the core.
 * Can reduce latency at cost of higher risk of stuttering.
 */
#define DEFAULT_FRAME_DELAY 0
#define MAXIMUM_FRAME_DELAY 19
#define DEFAULT_FRAME_DELAY_AUTO false

/* Inserts black frame(s) inbetween frames.
 * Useful for Higher Hz monitors (set to multiples of 60 Hz) who want to play 60 Hz 
 * material with eliminated  ghosting. video_refresh_rate should still be configured
 * as if it is a 60 Hz monitor (divide refresh rate by multiple of 60 Hz).
 */
#define DEFAULT_BLACK_FRAME_INSERTION 0


/* Uses a custom swap interval for VSync.
 * Set this to effectively halve monitor refresh rate.
 */
#define DEFAULT_SWAP_INTERVAL 1

/* Threaded video. Will possibly increase performance significantly
 * at the cost of worse synchronization and latency.
 */
#if defined(HAVE_LIBNX) || defined(ANDROID)
#define DEFAULT_VIDEO_THREADED true
#else
#define DEFAULT_VIDEO_THREADED false
#endif

#if defined(HAVE_THREADS)
#if defined(GEKKO) || defined(PSP) || defined(PS2)
/* For single-core consoles right now it's best to have this be disabled. */
#define DEFAULT_THREADED_DATA_RUNLOOP_ENABLE false
#else
#define DEFAULT_THREADED_DATA_RUNLOOP_ENABLE true
#endif
#else
#define DEFAULT_THREADED_DATA_RUNLOOP_ENABLE false
#endif

/* Set to true if HW render cores should get their private context. */
#define DEFAULT_VIDEO_SHARED_CONTEXT false

/* Sets GC/Wii screen width. */
#define DEFAULT_VIDEO_VI_WIDTH 640

#ifdef GEKKO
/* Removes 480i flicker, smooths picture a little. */
#define DEFAULT_VIDEO_VFILTER true

/* Allow overscan to be corrected on displays that
 * do not have proper 'pixel perfect' scaling */
#define DEFAULT_VIDEO_OVERSCAN_CORRECTION_TOP 0
#define DEFAULT_VIDEO_OVERSCAN_CORRECTION_BOTTOM 0
#endif

/* Smooths picture. */
#if defined(_3DS) || defined(GEKKO) || defined(HW_RVL) || defined(PSP) || defined(VITA) || defined(SN_TARGET_PSP2) || defined(PS2) || defined(_XBOX) || defined(DINGUX)
#define DEFAULT_VIDEO_SMOOTH true
#else
#define DEFAULT_VIDEO_SMOOTH false
#endif

/* Graphics context specific scaling */
#define DEFAULT_VIDEO_CTX_SCALING false

/* On resize and fullscreen, rendering area will stay 4:3 */
#define DEFAULT_FORCE_ASPECT true

/* Only applies to Android 9.0 (API 28) and up */
/* Choose if the screen will be able to write around the notch or not */
#define DEFAULT_NOTCH_WRITE_OVER_ENABLE false

/* Enable use of shaders. */
#ifdef RARCH_CONSOLE
#define DEFAULT_SHADER_ENABLE true
#else
#define DEFAULT_SHADER_ENABLE false
#endif

/* Should we enable hdr when its supported*/
#define DEFAULT_VIDEO_HDR_ENABLE false

/* The maximum nunmber of nits the actual display can show - needs to be calibrated */
#define DEFAULT_VIDEO_HDR_MAX_NITS 1000.0f

/* The number of nits that paper white is at */
#define DEFAULT_VIDEO_HDR_PAPER_WHITE_NITS 200.0f

/* The contrast setting for hdr used to calculate the display gamma by dividing this value by gamma 2.2  */
#define DEFAULT_VIDEO_HDR_CONTRAST 5.0f

/* Should we expand the colour gamut when using hdr */
#define DEFAULT_VIDEO_HDR_EXPAND_GAMUT true

/* When presets are saved they will be saved using the #reference 
 * directive by default */
#define DEFAULT_VIDEO_SHADER_PRESET_SAVE_REFERENCE_ENABLE true

#define DEFAULT_SHADER_DELAY 0

/* Only scale in integer steps.
 * The base size depends on system-reported geometry and aspect ratio.
 * If video_force_aspect is not set, X/Y will be integer scaled independently.
 * Overscale rounds up instead of down, default is downscale.
 */
#define DEFAULT_SCALE_INTEGER false
#define DEFAULT_SCALE_INTEGER_OVERSCALE false

/* Controls aspect ratio handling. */

/* 1:1 PAR */
#define DEFAULT_ASPECT_RATIO_AUTO false

#if defined(_XBOX360) || defined(__PS3__)
#define DEFAULT_ASPECT_RATIO_IDX ASPECT_RATIO_16_9
#elif defined(PSP) || defined(_3DS) || defined(HAVE_LIBNX) || defined(VITA)
#define DEFAULT_ASPECT_RATIO_IDX ASPECT_RATIO_CORE
#elif defined(RARCH_CONSOLE)
#define DEFAULT_ASPECT_RATIO_IDX ASPECT_RATIO_4_3
#else
#define DEFAULT_ASPECT_RATIO_IDX ASPECT_RATIO_CORE
#endif

#if defined(DINGUX)
/* Enables aspect ratio correction (1:1 PAR) when
 * using the IPU hardware scaler in Dingux devices */
#define DEFAULT_DINGUX_IPU_KEEP_ASPECT true
/* Sets image filtering method when using the
 * IPU hardware scaler in Dingux devices */
#if defined(RETROFW)
#define DEFAULT_DINGUX_IPU_FILTER_TYPE DINGUX_IPU_FILTER_NEAREST
#else
#define DEFAULT_DINGUX_IPU_FILTER_TYPE DINGUX_IPU_FILTER_BICUBIC
#endif

#if defined(DINGUX_BETA)
/* Sets refresh rate of integral LCD panel
 * in Dingux devices */
#define DEFAULT_DINGUX_REFRESH_RATE DINGUX_REFRESH_RATE_60HZ
#endif
#if defined(RS90) || defined(MIYOO)
/* Sets image filtering method on the RS90
 * when integer scaling is disabled */
#define DEFAULT_DINGUX_RS90_SOFTFILTER_TYPE DINGUX_RS90_SOFTFILTER_POINT
#endif
#endif

/* Save configuration file on exit. */
#define DEFAULT_CONFIG_SAVE_ON_EXIT true

#define DEFAULT_SHOW_HIDDEN_FILES false

/* Initialise file browser with the last used start directory */
#define DEFAULT_USE_LAST_START_DIRECTORY false

#define DEFAULT_OVERLAY_BEHIND_MENU false

#define DEFAULT_OVERLAY_HIDE_IN_MENU true

/* Automatically disable overlays when a
 * controller is connected in port 1 */
#define DEFAULT_OVERLAY_HIDE_WHEN_GAMEPAD_CONNECTED false

#define DEFAULT_OVERLAY_SHOW_MOUSE_CURSOR true

#define DEFAULT_DISPLAY_KEYBOARD_OVERLAY false

#ifdef HAKCHI
#define DEFAULT_INPUT_OVERLAY_OPACITY 0.5f
#else
#define DEFAULT_INPUT_OVERLAY_OPACITY 0.7f
#endif

#define DEFAULT_INPUT_OVERLAY_SCALE_LANDSCAPE 1.0f
#define DEFAULT_INPUT_OVERLAY_ASPECT_ADJUST_LANDSCAPE 0.0f
#define DEFAULT_INPUT_OVERLAY_X_SEPARATION_LANDSCAPE 0.0f
#define DEFAULT_INPUT_OVERLAY_Y_SEPARATION_LANDSCAPE 0.0f
#define DEFAULT_INPUT_OVERLAY_X_OFFSET_LANDSCAPE 0.0f
#define DEFAULT_INPUT_OVERLAY_Y_OFFSET_LANDSCAPE 0.0f

#define DEFAULT_INPUT_OVERLAY_SCALE_PORTRAIT 1.0f
#define DEFAULT_INPUT_OVERLAY_ASPECT_ADJUST_PORTRAIT 0.0f
#define DEFAULT_INPUT_OVERLAY_X_SEPARATION_PORTRAIT 0.0f
#define DEFAULT_INPUT_OVERLAY_Y_SEPARATION_PORTRAIT 0.0f
#define DEFAULT_INPUT_OVERLAY_X_OFFSET_PORTRAIT 0.0f
#define DEFAULT_INPUT_OVERLAY_Y_OFFSET_PORTRAIT 0.0f

#if defined(RARCH_MOBILE)
#define DEFAULT_OVERLAY_AUTO_ROTATE true
#else
#define DEFAULT_OVERLAY_AUTO_ROTATE false
#endif

#if defined(RARCH_MOBILE)
#define DEFAULT_INPUT_OVERLAY_AUTO_SCALE true
#else
#define DEFAULT_INPUT_OVERLAY_AUTO_SCALE false
#endif

#include "runtime_file.h"
#ifdef HAVE_MENU
#include "menu/menu_driver.h"

#ifdef HAVE_LIBNX
#define DEFAULT_MENU_USE_PREFERRED_SYSTEM_COLOR_THEME true
#else
#define DEFAULT_MENU_USE_PREFERRED_SYSTEM_COLOR_THEME false
#endif

#ifdef HAVE_OZONE
/* Ozone colour theme: 1 == Basic Black */
#define DEFAULT_OZONE_COLOR_THEME 1
#define DEFAULT_OZONE_COLLAPSE_SIDEBAR false
#define DEFAULT_OZONE_TRUNCATE_PLAYLIST_NAME true
#define DEFAULT_OZONE_SORT_AFTER_TRUNCATE_PLAYLIST_NAME true
#define DEFAULT_OZONE_SCROLL_CONTENT_METADATA false
#endif

#define DEFAULT_SETTINGS_SHOW_DRIVERS true

#define DEFAULT_SETTINGS_SHOW_VIDEO true

#define DEFAULT_SETTINGS_SHOW_AUDIO true

#define DEFAULT_SETTINGS_SHOW_INPUT true

#define DEFAULT_SETTINGS_SHOW_LATENCY true

#define DEFAULT_SETTINGS_SHOW_CORE true

#define DEFAULT_SETTINGS_SHOW_CONFIGURATION true

#define DEFAULT_SETTINGS_SHOW_SAVING true

#define DEFAULT_SETTINGS_SHOW_LOGGING true

#define DEFAULT_SETTINGS_SHOW_FILE_BROWSER true

#define DEFAULT_SETTINGS_SHOW_FRAME_THROTTLE true

#define DEFAULT_SETTINGS_SHOW_RECORDING true

#define DEFAULT_SETTINGS_SHOW_ONSCREEN_DISPLAY true

#define DEFAULT_SETTINGS_SHOW_USER_INTERFACE true

#define DEFAULT_SETTINGS_SHOW_AI_SERVICE true

#define DEFAULT_SETTINGS_SHOW_ACCESSIBILITY true

#define DEFAULT_SETTINGS_SHOW_POWER_MANAGEMENT true

#define DEFAULT_SETTINGS_SHOW_ACHIEVEMENTS true

#define DEFAULT_SETTINGS_SHOW_NETWORK true

#define DEFAULT_SETTINGS_SHOW_PLAYLISTS true

#define DEFAULT_SETTINGS_SHOW_USER true

#define DEFAULT_SETTINGS_SHOW_DIRECTORY true

#define DEFAULT_QUICK_MENU_SHOW_RESUME_CONTENT true

#define DEFAULT_QUICK_MENU_SHOW_RESTART_CONTENT true

#define DEFAULT_QUICK_MENU_SHOW_CLOSE_CONTENT true

#define DEFAULT_QUICK_MENU_SHOW_TAKE_SCREENSHOT true

#define DEFAULT_QUICK_MENU_SHOW_SAVE_LOAD_STATE true

#define DEFAULT_QUICK_MENU_SHOW_UNDO_SAVE_LOAD_STATE true

static const bool quick_menu_show_add_to_favorites            = true;
static const bool quick_menu_show_start_recording             = true;
static const bool quick_menu_show_start_streaming             = true;
static const bool quick_menu_show_set_core_association        = true;
static const bool quick_menu_show_reset_core_association      = true;
static const bool quick_menu_show_options                     = true;

#define DEFAULT_QUICK_MENU_SHOW_CORE_OPTIONS_FLUSH false

static const bool quick_menu_show_controls                    = true;
static const bool quick_menu_show_cheats                      = true;
static const bool quick_menu_show_shaders                     = true;
static const bool quick_menu_show_information                 = true;
static const bool quick_menu_show_recording                   = true;
static const bool quick_menu_show_streaming                   = true;

static const bool quick_menu_show_save_core_overrides         = true;
static const bool quick_menu_show_save_game_overrides         = true;
static const bool quick_menu_show_save_content_dir_overrides  = true;

#ifdef HAVE_NETWORKING
static const bool quick_menu_show_download_thumbnails         = true;
#endif

#define DEFAULT_KIOSK_MODE_ENABLE false

#define DEFAULT_MENU_HORIZONTAL_ANIMATION true

static const bool menu_show_online_updater     = true;
static const bool menu_show_load_core          = true;
static const bool menu_show_load_content       = true;
#ifdef HAVE_CDROM
static const bool menu_show_load_disc          = true;
static const bool menu_show_dump_disc          = true;
#ifdef HAVE_LAKKA
static const bool menu_show_eject_disc         = true;
#endif /* HAVE_LAKKA */
#endif
static const bool menu_show_information        = true;
static const bool menu_show_configurations     = true;
static const bool menu_show_help               = true;
static const bool menu_show_quit_retroarch     = true;
static const bool menu_show_restart_retroarch  = true;
static const bool menu_show_reboot             = true;
static const bool menu_show_shutdown           = true;
#if defined(HAVE_LAKKA) || defined(VITA)
static const bool menu_show_core_updater       = false;
#else
static const bool menu_show_core_updater       = true;
#endif
static const bool menu_show_legacy_thumbnail_updater = false;
static const bool menu_show_sublabels                = true;
static const bool menu_dynamic_wallpaper_enable      = true;
static const bool menu_scroll_fast                   = false;

#define DEFAULT_MENU_SCROLL_DELAY 256

#define DEFAULT_MENU_TICKER_TYPE (TICKER_TYPE_LOOP)
static const float menu_ticker_speed        = 2.0f;

#define DEFAULT_MENU_TICKER_SMOOTH true

#if defined(HAVE_THREADS)
static const bool menu_savestate_resume     = true;
#else
static const bool menu_savestate_resume     = false;
#endif

#define DEFAULT_MENU_INSERT_DISK_RESUME true

#define DEFAULT_QUIT_ON_CLOSE_CONTENT QUIT_ON_CLOSE_CONTENT_DISABLED

/* While the menu is active, supported drivers
 * will display a screensaver after SCREENSAVER_TIMEOUT
 * seconds of inactivity. A timeout of zero disables
 * the screensaver */
#define DEFAULT_MENU_SCREENSAVER_TIMEOUT 0

#if defined(HAVE_MATERIALUI) || defined(HAVE_XMB) || defined(HAVE_OZONE)
/* When menu screensaver is enabled, specifies
 * animation effect and animation speed */
#define DEFAULT_MENU_SCREENSAVER_ANIMATION MENU_SCREENSAVER_BLANK
#define DEFAULT_MENU_SCREENSAVER_ANIMATION_SPEED 1.0f
#endif

static const bool content_show_settings     = true;
static const bool content_show_favorites    = true;
#ifdef HAVE_IMAGEVIEWER
static const bool content_show_images       = true;
#endif
static const bool content_show_music        = true;
#if defined(HAVE_FFMPEG) || defined(HAVE_MPV)
static const bool content_show_video        = true;
#endif
#if defined(HAVE_NETWORKING)
#if defined(_3DS)
static const bool content_show_netplay      = false;
#else
static const bool content_show_netplay      = true;
#endif
#endif
static const bool content_show_history      = true;

/* Specifies 'add content' visibility when using
 * menus WITH a dedicated 'Import Content' tab */
#define DEFAULT_MENU_CONTENT_SHOW_ADD true
/* Specifies 'add content' visibility when using
 * menus WITHOUT a dedicated 'Import Content' tab */
#define DEFAULT_MENU_CONTENT_SHOW_ADD_ENTRY MENU_ADD_CONTENT_ENTRY_DISPLAY_PLAYLISTS_TAB
#define DEFAULT_MENU_CONTENT_SHOW_CONTENTLESS_CORES MENU_CONTENTLESS_CORES_DISPLAY_SINGLE_PURPOSE

static const bool content_show_playlists    = true;
#if defined(HAVE_LIBRETRODB)
#define DEFAULT_MENU_CONTENT_SHOW_EXPLORE true
#endif

#ifdef HAVE_XMB
#define DEFAULT_XMB_ANIMATION 0
#define DEFAULT_XMB_VERTICAL_FADE_FACTOR 100

static const unsigned xmb_alpha_factor      = 75;
static const unsigned menu_font_color_red   = 255;
static const unsigned menu_font_color_green = 255;
static const unsigned menu_font_color_blue  = 255;
#ifdef HAVE_ODROIDGO2
static const unsigned xmb_menu_layout       = 2;
#else
static const unsigned xmb_menu_layout       = 0;
#endif
static const unsigned xmb_icon_theme        = XMB_ICON_THEME_MONOCHROME;
static const unsigned xmb_theme             = XMB_THEME_ELECTRIC_BLUE;

#if defined(HAVE_LAKKA) || defined(__arm__) || defined(__PPC64__) || defined(__ppc64__) || defined(__powerpc64__) || defined(__powerpc__) || defined(__ppc__) || defined(__POWERPC__)
#define DEFAULT_XMB_SHADOWS_ENABLE false
#else
#define DEFAULT_XMB_SHADOWS_ENABLE true
#endif
#endif

static const float menu_framebuffer_opacity = 0.900;

static const float menu_wallpaper_opacity = 0.300;

static const float menu_footer_opacity = 1.000;

static const float menu_header_opacity = 1.000;

#if defined(HAVE_OPENGLES2) || (defined(__MACH__) && (defined(__ppc__) || defined(__ppc64__)))
#define DEFAULT_MENU_SHADER_PIPELINE 1
#else
#define DEFAULT_MENU_SHADER_PIPELINE 2
#endif

#define DEFAULT_SHOW_ADVANCED_SETTINGS false

#define DEFAULT_RGUI_COLOR_THEME RGUI_THEME_CLASSIC_GREEN
#define DEFAULT_RGUI_TRANSPARENCY true

static const bool rgui_inline_thumbnails = false;
static const bool rgui_swap_thumbnails = false;
static const unsigned rgui_thumbnail_downscaler = RGUI_THUMB_SCALE_POINT;
static const unsigned rgui_thumbnail_delay = 0;
static const unsigned rgui_internal_upscale_level = RGUI_UPSCALE_NONE;
static const bool rgui_full_width_layout = true;
static const unsigned rgui_aspect = RGUI_ASPECT_RATIO_4_3;
static const unsigned rgui_aspect_lock = RGUI_ASPECT_RATIO_LOCK_NONE;
static const bool rgui_shadows = false;
static const unsigned rgui_particle_effect = RGUI_PARTICLE_EFFECT_NONE;
#define DEFAULT_RGUI_PARTICLE_EFFECT_SPEED 1.0f
#define DEFAULT_RGUI_PARTICLE_EFFECT_SCREENSAVER true
static const bool rgui_extended_ascii = false;
#define DEFAULT_RGUI_SWITCH_ICONS true
#endif

#ifdef HAVE_MENU
#define DEFAULT_BLOCK_CONFIG_READ true
#else
#define DEFAULT_BLOCK_CONFIG_READ false
#endif

/* TODO/FIXME - this setting is thread-unsafe right now and can corrupt the stack - default to off */
#define DEFAULT_AUTOMATICALLY_ADD_CONTENT_TO_PLAYLIST false

static const bool default_game_specific_options = true;
static const bool default_auto_overrides_enable = true;
static const bool default_auto_remaps_enable = true;
static const bool default_global_core_options = false;
static const bool default_auto_shaders_enable = true;

static const bool default_sort_savefiles_enable = false;
static const bool default_sort_savestates_enable = false;
static const bool default_sort_savefiles_by_content_enable = false;
static const bool default_sort_savestates_by_content_enable = false;
static const bool default_sort_screenshots_by_content_enable = false;

static const bool default_savestates_in_content_dir = false;
static const bool default_savefiles_in_content_dir = false;
static const bool default_systemfiles_in_content_dir = false;
static const bool default_screenshots_in_content_dir = false;

#if defined(RS90) || defined(RETROFW) || defined(MIYOO)
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_START_SELECT
#elif defined(_XBOX1) || defined(__PS3__) || defined(_XBOX360) || defined(DINGUX)
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_L3_R3
#elif defined(PS2) || defined(PSP)
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_HOLD_START
#elif defined(VITA)
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_L1_R1_START_SELECT
#elif defined(SWITCH) || defined(ORBIS)
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_START_SELECT
#elif TARGET_OS_TV
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_DOWN_Y_L_R
#else
#define DEFAULT_MENU_TOGGLE_GAMEPAD_COMBO INPUT_COMBO_NONE
#endif

#define DEFAULT_QUIT_GAMEPAD_COMBO INPUT_COMBO_NONE

#if defined(VITA)
static const unsigned input_backtouch_enable       = false;
static const unsigned input_backtouch_toggle       = false;
#endif

#define DEFAULT_OVERLAY_ENABLE_AUTOPREFERRED true

#if defined(HAVE_OVERLAY)
#if defined(RARCH_MOBILE)
#define DEFAULT_OVERLAY_SHOW_INPUTS OVERLAY_SHOW_INPUT_TOUCHED
#else
#define DEFAULT_OVERLAY_SHOW_INPUTS OVERLAY_SHOW_INPUT_PHYSICAL
#endif
#endif

#define DEFAULT_OVERLAY_SHOW_INPUTS_PORT 0

#if defined(ANDROID) || defined(_WIN32)
#define DEFAULT_MENU_SWAP_OK_CANCEL_BUTTONS true
#else
#define DEFAULT_MENU_SWAP_OK_CANCEL_BUTTONS false
#endif

#if defined(WIIU)
#define DEFAULT_ALL_USERS_CONTROL_MENU true
#else
#define DEFAULT_ALL_USERS_CONTROL_MENU false
#endif

#define DEFAULT_QUIT_PRESS_TWICE true

#define DEFAULT_LOG_TO_FILE false

#define DEFAULT_LOG_TO_FILE_TIMESTAMP false

/* Crop overscanned frames. */
#define DEFAULT_CROP_OVERSCAN true

/* Font size for on-screen messages. */
#if defined(DINGUX)
#define DEFAULT_FONT_SIZE 12
#elif defined(PS2)
#define DEFAULT_FONT_SIZE 16
#else
#define DEFAULT_FONT_SIZE 32
#endif


/* Offset for where messages will be placed on-screen.
 * Values are in range [0.0, 1.0]. */
static const float message_pos_offset_x = 0.05;
static const float message_pos_offset_y = 0.05;

/* Color of the message.
 * RGB hex value. */
static const uint32_t message_color = 0xffff00;

static const bool message_bgcolor_enable = false;
static const uint32_t message_bgcolor_red = 0;
static const uint32_t message_bgcolor_green = 0;
static const uint32_t message_bgcolor_blue = 0;
static const float message_bgcolor_opacity = 1.0f;

/* Record post-filtered (CPU filter) video,
 * rather than raw game output. */
#define DEFAULT_POST_FILTER_RECORD false

/* Screenshots post-shaded GPU output if available. */
#define DEFAULT_GPU_SCREENSHOT true

/* Watch shader files for changes and auto-apply as necessary. */
#define DEFAULT_VIDEO_SHADER_WATCH_FILES false

/* Initialise file browser with last used directory
 * when selecting shader presets/passes via the menu */
#define DEFAULT_VIDEO_SHADER_REMEMBER_LAST_DIR false

/* Screenshots named automatically. */
#define DEFAULT_AUTO_SCREENSHOT_FILENAME true

/* Record post-shaded GPU output instead of raw game footage if available. */
#define DEFAULT_GPU_RECORD false

/* OSD-messages. */
#define DEFAULT_FONT_ENABLE true

/* The accurate refresh rate of your monitor (Hz).
 * This is used to calculate audio input rate with the formula:
 * audio_input_rate = game_input_rate * display_refresh_rate /
 * game_refresh_rate.
 *
 * If the implementation does not report any values,
 * NTSC defaults will be assumed for compatibility.
 * This value should stay close to 60Hz to avoid large pitch changes.
 * If your monitor does not run at 60Hz, or something close to it,
 * disable VSync, and leave this at its default. */
#ifdef _3DS
#define DEFAULT_REFRESH_RATE ((32730.0 * 8192.0) / 4481134.0)
#elif defined(RARCH_CONSOLE)
#define DEFAULT_REFRESH_RATE (60/1.001)
#else
#define DEFAULT_REFRESH_RATE (60)
#endif
#define DEFAULT_CRT_REFRESH_RATE (DEFAULT_REFRESH_RATE)

/* Allow games to set rotation. If false, rotation requests are
 * honored, but ignored.
 * Used for setups where one manually rotates the monitor. */
#define DEFAULT_ALLOW_ROTATE true

#if defined(_3DS)
/* Enable bottom LCD screen */
static const bool video_3ds_lcd_bottom = true;
/* Sets video display mode (3D, 2D, etc.) */
static const unsigned video_3ds_display_mode = CTR_VIDEO_MODE_3D;
#endif

/* AUDIO */

/* Will enable audio or not. */
#define DEFAULT_AUDIO_ENABLE true

/* Enable menu audio sounds. */
static const bool audio_enable_menu        = false;
static const bool audio_enable_menu_ok     = false;
static const bool audio_enable_menu_cancel = false;
static const bool audio_enable_menu_notice = false;
static const bool audio_enable_menu_bgm    = false;

#ifdef HAVE_GFX_WIDGETS
#define DEFAULT_MENU_ENABLE_WIDGETS true
#else
#define DEFAULT_MENU_ENABLE_WIDGETS false
#endif

/* Display an animation when loading content
 * > Currently implemented only as a widget */
#define DEFAULT_MENU_SHOW_LOAD_CONTENT_ANIMATION DEFAULT_MENU_ENABLE_WIDGETS

/* Display a notification when successfully
 * connecting/disconnecting an autoconfigured
 * controller
 * > Disabled by default on the Switch */
#if defined(HAVE_LIBNX) && defined(HAVE_GFX_WIDGETS)
#define DEFAULT_NOTIFICATION_SHOW_AUTOCONFIG false
#else
#define DEFAULT_NOTIFICATION_SHOW_AUTOCONFIG true
#endif

/* Display a notification when cheats are being
 * applied */
#define DEFAULT_NOTIFICATION_SHOW_CHEATS_APPLIED true

/* Display a notification when applying an
 * IPS/BPS/UPS patch file */
#define DEFAULT_NOTIFICATION_SHOW_PATCH_APPLIED true

/* Display a notification when loading an
 * input remap file */
#define DEFAULT_NOTIFICATION_SHOW_REMAP_LOAD true

/* Display a notification when loading a
 * configuration override file */
#define DEFAULT_NOTIFICATION_SHOW_CONFIG_OVERRIDE_LOAD true

/* Display a notification when automatically restoring
 * at launch the last used disk of multi-disk content */
#define DEFAULT_NOTIFICATION_SHOW_SET_INITIAL_DISK true

/* Display a notification when fast forwarding
 * content */
#define DEFAULT_NOTIFICATION_SHOW_FAST_FORWARD true

#if defined(HAVE_SCREENSHOTS)
/*Display a notification when taking a screenshot*/
#define DEFAULT_NOTIFICATION_SHOW_SCREENSHOT true

/*Desired duration of the screenshot notification*/
#define DEFAULT_NOTIFICATION_SHOW_SCREENSHOT_DURATION 0

/* Display a white flashing effect with the desired 
 * duration when taking a screenshot*/
#define DEFAULT_NOTIFICATION_SHOW_SCREENSHOT_FLASH 0
#endif

/* Display a notification when setting the refresh rate*/
#if defined(_3DS) || (defined(DINGUX) && defined(DINGUX_BETA))
/* 3DS and OpenDingux Beta devices set refresh rate
 * on gfx driver init - set default notification
 * state to 'false' in order to avoid OSD log spam */
#define DEFAULT_NOTIFICATION_SHOW_REFRESH_RATE false
#else
#define DEFAULT_NOTIFICATION_SHOW_REFRESH_RATE true
#endif

#ifdef HAVE_NETWORKING
#define DEFAULT_NOTIFICATION_SHOW_NETPLAY_EXTRA false
#endif

#ifdef HAVE_MENU
#define DEFAULT_NOTIFICATION_SHOW_WHEN_MENU_IS_ALIVE false
#endif

/* Output samplerate. */
#ifdef GEKKO
#define DEFAULT_OUTPUT_RATE 32000
#elif defined(_3DS) || defined(RETROFW) || defined(MIYOO)
#define DEFAULT_OUTPUT_RATE 32730
#else
#define DEFAULT_OUTPUT_RATE 48000
#endif

/* Audio device (e.g. hw:0,0 or /dev/audio). If NULL, will use defaults. */
#define DEFAULT_AUDIO_DEVICE NULL

/* Desired audio latency in milliseconds. Might not be honored
 * if driver can't provide given latency. */
#if defined(ANDROID) || defined(EMSCRIPTEN) || defined(RETROFW) || defined(MIYOO)
/* For most Android devices, 64ms is way too low. */
#define DEFAULT_OUT_LATENCY 128
#else
#define DEFAULT_OUT_LATENCY 64
#endif

/* Will sync audio. (recommended) */
#define DEFAULT_AUDIO_SYNC true

/* Audio rate control. */
#if !defined(RARCH_CONSOLE)
#define DEFAULT_RATE_CONTROL true
#else
#define DEFAULT_RATE_CONTROL false
#endif

/* Rate control delta. Defines how much rate_control
 * is allowed to adjust input rate. */
#define DEFAULT_RATE_CONTROL_DELTA  0.005

/* Maximum timing skew. Defines how much adjust_system_rates
 * is allowed to adjust input rate. */
#define DEFAULT_MAX_TIMING_SKEW  0.05

/* Default audio volume in dB. (0.0 dB == unity gain). */
#define DEFAULT_AUDIO_VOLUME 0.0

/* Default audio volume of the audio mixer in dB. (0.0 dB == unity gain). */
#define DEFAULT_AUDIO_MIXER_VOLUME 0.0

#ifdef HAVE_WASAPI
/* WASAPI defaults */
#define DEFAULT_WASAPI_EXCLUSIVE_MODE true
#define DEFAULT_WASAPI_FLOAT_FORMAT false
/* auto */
#define DEFAULT_WASAPI_SH_BUFFER_LENGTH -16
#endif

/* Automatically mute audio when fast forward
 * is enabled */
#define DEFAULT_AUDIO_FASTFORWARD_MUTE false

/* MISC */

/* Enables displaying the current frames per second. */
#define DEFAULT_FPS_SHOW false

/* FPS display will be updated at the set interval (in frames) */
#define DEFAULT_FPS_UPDATE_INTERVAL 256

/* Memory status display will be updated at the set interval (in frames) */
#define DEFAULT_MEMORY_UPDATE_INTERVAL 256

/* Enables displaying the current frame count. */
#define DEFAULT_FRAMECOUNT_SHOW false

/* Includes displaying the current memory usage/total with FPS/Frames. */
#define DEFAULT_MEMORY_SHOW false

/* Enables displaying various timing statistics. */
#define DEFAULT_STATISTICS_SHOW false

/* Enables displaying the current netplay room ping. */
#define DEFAULT_NETPLAY_PING_SHOW false

/* Enables use of rewind. This will incur some memory footprint
 * depending on the save state buffer. */
#define DEFAULT_REWIND_ENABLE false

/* When set, any time a cheat is toggled it is immediately applied. */
#define DEFAULT_APPLY_CHEATS_AFTER_TOGGLE false

/* When set, all enabled cheats are auto-applied when a game is loaded. */
#define DEFAULT_APPLY_CHEATS_AFTER_LOAD false


#if defined(RETROFW) || defined(MIYOO)
/*RETROFW jz4760 has signficant slowdown with default settings */
#define DEFAULT_REWIND_BUFFER_SIZE (1 << 20)
#define DEFAULT_REWIND_BUFFER_SIZE_STEP 1 
#define DEFAULT_REWIND_GRANULARITY 6
#else
/* The buffer size for the rewind buffer. This needs to be about
 * 15-20MB per minute. Very game dependant. */
#define DEFAULT_REWIND_BUFFER_SIZE (20 << 20) /* 20MiB */

/* The amount of MB to increase/decrease the rewind_buffer_size when it is changed via the UI. */
#define DEFAULT_REWIND_BUFFER_SIZE_STEP 10 /* 10MB */

/* How many frames to rewind at a time. */
#define DEFAULT_REWIND_GRANULARITY 1
#endif
/* Pause gameplay when gameplay loses focus. */
#if defined(EMSCRIPTEN) || defined(WEBOS)
#define DEFAULT_PAUSE_NONACTIVE false
#else
#define DEFAULT_PAUSE_NONACTIVE true
#endif

/* Saves non-volatile SRAM at a regular interval.
 * It is measured in seconds. A value of 0 disables autosave. */
#if defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(__x86_64__) || defined(_M_X64) || defined(_WIN32) || defined(OSX) || defined(ANDROID) || defined(IOS) || defined(DINGUX)
/* Flush to file every 10 seconds on modern platforms by default */
#define DEFAULT_AUTOSAVE_INTERVAL 10
#else
/* Default to disabled on I/O-constrained platforms */
#define DEFAULT_AUTOSAVE_INTERVAL 0
#endif

/* Show only connectable rooms */
#define DEFAULT_NETPLAY_SHOW_ONLY_CONNECTABLE true

/* Publicly announce netplay */
#define DEFAULT_NETPLAY_PUBLIC_ANNOUNCE true

/* Start netplay in spectator mode */
static const bool netplay_start_as_spectator = false;

/* Netplay chat fading toggle */
static const bool netplay_fade_chat = true;

/* Allow players to pause */
static const bool netplay_allow_pausing = false;

/* Allow connections in slave mode */
static const bool netplay_allow_slaves = true;

/* Require connections only in slave mode */
static const bool netplay_require_slaves = false;

/* Netplay without savestates/rewind */
static const bool netplay_stateless_mode = false;

/* When being client over netplay, use keybinds for
 * user 1 rather than user 2. */
static const bool netplay_client_swap_input = true;

static const bool netplay_nat_traversal = false;

static const unsigned netplay_delay_frames = 16;

static const int netplay_check_frames = 600;

static const bool netplay_use_mitm_server = false;

#define DEFAULT_NETPLAY_MITM_SERVER "nyc"

#ifdef HAVE_NETWORKING
static const unsigned netplay_max_connections = 3;
static const unsigned netplay_max_ping        = 0;

static const unsigned netplay_share_digital = RARCH_NETPLAY_SHARE_DIGITAL_NO_PREFERENCE;
static const unsigned netplay_share_analog  = RARCH_NETPLAY_SHARE_ANALOG_NO_PREFERENCE;
#endif

/* On save state load, block SRAM from being overwritten.
 * This could potentially lead to buggy games. */
#define DEFAULT_BLOCK_SRAM_OVERWRITE false

/* When saving savestates, state index is automatically
 * incremented before saving.
 * When the content is loaded, state index will be set
 * to the highest existing value. */
static const bool savestate_auto_index = false;

/* Specifies the maximum number of savestates to keep
 * when savestate auto index is enabled
 * > When limit is exceeded, savestate with the lowest
 *   index will be deleted automatically when creating
 *   a new savestate
 * > Setting value to zero disables the limit (no
 *   savestates will be deleted in this case) */
#define DEFAULT_SAVESTATE_MAX_KEEP 0

/* Automatically saves a savestate at the end of RetroArch's lifetime.
 * The path is $SRAM_PATH.auto.
 * RetroArch will automatically load any savestate with this path on
 * startup if savestate_auto_load is set. */
static const bool savestate_auto_save = false;
static const bool savestate_auto_load = false;

static const bool savestate_thumbnail_enable = false;

/* When creating save (srm) files, compress
 * written data */
#define DEFAULT_SAVE_FILE_COMPRESSION false

/* When creating save state files, compress
 * written data */
#if defined(__WINRT__) || defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
/* TODO/FIXME Apparently this is an issue on UWP for now, so disable it for now */
#define DEFAULT_SAVESTATE_FILE_COMPRESSION false
#else
#define DEFAULT_SAVESTATE_FILE_COMPRESSION true
#endif

/* Slowmotion ratio. */
#define DEFAULT_SLOWMOTION_RATIO 3.0

/* Maximum fast forward ratio. */
#define DEFAULT_FASTFORWARD_RATIO 0.0

/* Enable runloop for variable refresh rate screens. Force x1 speed while handling fast forward too. */
#define DEFAULT_VRR_RUNLOOP_ENABLE false

/* Run core logic one or more frames ahead then load the state back to reduce perceived input lag. */
#define DEFAULT_RUN_AHEAD_FRAMES 1

/* When using the Run Ahead feature, use a secondary instance of the core. */
#define DEFAULT_RUN_AHEAD_SECONDARY_INSTANCE true

/* Hide warning messages when using the Run Ahead feature. */
#define DEFAULT_RUN_AHEAD_HIDE_WARNINGS false

/* Enable stdin/network command interface. */
static const bool network_cmd_enable = false;
static const uint16_t network_cmd_port = 55355;
static const bool stdin_cmd_enable = false;

static const uint16_t network_remote_base_port = 55400;

#define DEFAULT_NETWORK_BUILDBOT_AUTO_EXTRACT_ARCHIVE true
#define DEFAULT_NETWORK_BUILDBOT_SHOW_EXPERIMENTAL_CORES false

/* Automatically create a backup whenever a core is
 * updated via the online updater
 * > Enable by default on all modern platforms with
 *   online updater support */
#if defined(HAVE_ONLINE_UPDATER) && (defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(__x86_64__) || defined(_M_X64) || defined(_WIN32) || defined(OSX) || defined(ANDROID) || defined(IOS))
#define DEFAULT_CORE_UPDATER_AUTO_BACKUP true
#else
#define DEFAULT_CORE_UPDATER_AUTO_BACKUP false
#endif
/* Number of automatic core backups to retain
 * (oldest backup will be deleted when creating
 * a new one) */
#define DEFAULT_CORE_UPDATER_AUTO_BACKUP_HISTORY_SIZE 1

#if defined(ANDROID) || defined(__APPLE__)
#define DEFAULT_NETWORK_ON_DEMAND_THUMBNAILS true
#else
#define DEFAULT_NETWORK_ON_DEMAND_THUMBNAILS false
#endif

/* Number of entries that will be kept in content history playlist file. */
static const unsigned default_content_history_size = 200;

/* Number of entries that will be kept in content favorites playlist file.
 * -1 == 'unlimited' (99999) */
static const int default_content_favorites_size = 200;

/* Sort all playlists (apart from histories) alphabetically */
#define DEFAULT_PLAYLIST_SORT_ALPHABETICAL true

/* File format to use when writing playlists to disk */
#define DEFAULT_PLAYLIST_USE_OLD_FORMAT false

/* When creating/updating playlists, compress written data */
#define DEFAULT_PLAYLIST_COMPRESSION false

#ifdef HAVE_MENU
/* Specify when to display 'core name' inline on playlist entries */
#define DEFAULT_PLAYLIST_SHOW_INLINE_CORE_NAME PLAYLIST_INLINE_CORE_DISPLAY_HIST_FAV

/* Specifies which runtime record to use on playlist sublabels */
#define DEFAULT_PLAYLIST_SUBLABEL_RUNTIME_TYPE PLAYLIST_RUNTIME_PER_CORE

/* Specifies time/date display format for runtime 'last played' data */
#define DEFAULT_PLAYLIST_SUBLABEL_LAST_PLAYED_STYLE PLAYLIST_LAST_PLAYED_STYLE_YMD_HMS

#define DEFAULT_PLAYLIST_ENTRY_REMOVE_ENABLE PLAYLIST_ENTRY_REMOVE_ENABLE_ALL
#endif

#define DEFAULT_SCAN_WITHOUT_CORE_MATCH false

#ifdef __WINRT__
/* Be paranoid about WinRT file I/O performance, and leave this disabled by
 * default */
#define DEFAULT_PLAYLIST_SHOW_SUBLABELS false
#else
#define DEFAULT_PLAYLIST_SHOW_SUBLABELS true
#endif

#define DEFAULT_PLAYLIST_SHOW_HISTORY_ICONS PLAYLIST_SHOW_HISTORY_ICONS_DEFAULT

/* Show the indices of playlist entries in
 * a menu-driver-specific fashion */
#define DEFAULT_PLAYLIST_SHOW_ENTRY_IDX true

#define DEFAULT_PLAYLIST_FUZZY_ARCHIVE_MATCH false

#define DEFAULT_PLAYLIST_PORTABLE_PATHS false

/* Show Menu start-up screen on boot. */
#define DEFAULT_MENU_SHOW_START_SCREEN true

/* Default scale factor for non-frambuffer-based display
 * drivers and display widgets */
#if defined(VITA)
#define DEFAULT_MENU_SCALE_FACTOR 1.5f
#else
#define DEFAULT_MENU_SCALE_FACTOR 1.0f
#endif
/* Specifies whether display widgets should be scaled
 * automatically using the default menu scale factor */
#define DEFAULT_MENU_WIDGET_SCALE_AUTO true
/* Default scale factor for display widgets when widget
 * auto scaling is disabled (fullscreen mode) */
#define DEFAULT_MENU_WIDGET_SCALE_FACTOR 1.0f
/* Default scale factor for display widgets when widget
 * auto scaling is disabled (windowed mode) */
#define DEFAULT_MENU_WIDGET_SCALE_FACTOR_WINDOWED 1.0f

/* Log level for the frontend */
#define DEFAULT_FRONTEND_LOG_LEVEL 1

/* Log level for libretro cores (GET_LOG_INTERFACE). */
#define DEFAULT_LIBRETRO_LOG_LEVEL 1

#ifndef RARCH_DEFAULT_PORT
#define RARCH_DEFAULT_PORT 55435
#endif

#ifndef RARCH_STREAM_DEFAULT_PORT
#define RARCH_STREAM_DEFAULT_PORT 56400
#endif

/* KEYBINDS, JOYPAD */

/* Axis threshold (between 0.0 and 1.0)
 * How far an axis must be tilted to result in a button press. */
#define DEFAULT_AXIS_THRESHOLD 0.5f

#define DEFAULT_ANALOG_DEADZONE 0.0f

#define DEFAULT_ANALOG_SENSITIVITY 1.0f

/* Describes speed of which turbo-enabled buttons toggle. */
static const unsigned turbo_period        = 6;
static const unsigned turbo_duty_cycle    = 3;
static const unsigned turbo_mode          = 0;
static const unsigned turbo_default_btn   = RETRO_DEVICE_ID_JOYPAD_B;

/* Enable input auto-detection. Will attempt to autoconfigure
 * gamepads, plug-and-play style. */
static const bool input_autodetect_enable = true;

/* Enables accelerometer/gyroscope/illuminance
 * sensor input, if supported */
#if defined(ANDROID)
/* Hardware sensors cause substantial battery
 * drain on Android... */
#define DEFAULT_INPUT_SENSORS_ENABLE false
#else
#define DEFAULT_INPUT_SENSORS_ENABLE true
#endif

/* Automatically enable game focus when running or
 * resuming content */
#define DEFAULT_INPUT_AUTO_GAME_FOCUS AUTO_GAME_FOCUS_OFF

/* Show the input descriptors set by the core instead
 * of the default ones. */
static const bool input_descriptor_label_show = true;

static const bool input_descriptor_hide_unbound = false;

#if defined(DINGUX)
static const unsigned input_max_users = 1;
#else
static const unsigned input_max_users = 5;
#endif

static const unsigned input_poll_type_behavior = 2;

static const unsigned input_bind_timeout = 5;

static const unsigned input_bind_hold = 2;

#define DEFAULT_INPUT_HOTKEY_BLOCK_DELAY 5

static const unsigned gfx_thumbnails_default = 3;

static const unsigned menu_left_thumbnails_default = 0;

static const unsigned gfx_thumbnail_upscale_threshold = 0;

#ifdef HAVE_MENU
#if defined(RS90) || defined(MIYOO)
/* The RS-90 has a hardware clock that is neither
 * configurable nor persistent, rendering it useless.
 * We therefore hide it in the menu by default. */
#define DEFAULT_MENU_TIMEDATE_ENABLE false
#else
#define DEFAULT_MENU_TIMEDATE_ENABLE true
#endif
#define DEFAULT_MENU_TIMEDATE_STYLE          MENU_TIMEDATE_STYLE_DDMM_HM
#define DEFAULT_MENU_TIMEDATE_DATE_SEPARATOR MENU_TIMEDATE_DATE_SEPARATOR_HYPHEN
#endif

static const bool xmb_vertical_thumbnails = false;

static const unsigned xmb_thumbnail_scale_factor = 100;

#ifdef IOS
static const bool ui_companion_start_on_boot = false;
#else
static const bool ui_companion_start_on_boot = true;
#endif

static const bool ui_companion_enable = false;

/* Currently only used to show the WIMP UI on startup */
static const bool ui_companion_toggle = false;

/* Only init the WIMP UI for this session if this is enabled */
#define DEFAULT_DESKTOP_MENU_ENABLE true

/* Keep track of how long each core+content has been running for over time */

#ifdef __WINRT__
/* Be paranoid about WinRT file I/O performance, and leave this disabled by
 * default */
#define DEFAULT_CONTENT_RUNTIME_LOG false
#else
#define DEFAULT_CONTENT_RUNTIME_LOG true
#endif

/* Keep track of how long each content has been running 
 * for over time (ignores core) */
#define DEFAULT_CONTENT_RUNTIME_LOG_AGGREGATE false

#define DEFAULT_UI_MENUBAR_ENABLE true

#if defined(__QNX__) || defined(_XBOX1) || defined(_XBOX360) || (defined(__MACH__) && defined(IOS)) || defined(ANDROID) || defined(WIIU) || defined(HAVE_NEON) || defined(GEKKO) || defined(__ARM_NEON__) || defined(__PS3__)
static const enum resampler_quality audio_resampler_quality_level = RESAMPLER_QUALITY_LOWER;
#elif defined(PSP) || defined(_3DS) || defined(VITA) || defined(PS2) || defined(DINGUX)
static const enum resampler_quality audio_resampler_quality_level = RESAMPLER_QUALITY_LOWEST;
#else
static const enum resampler_quality audio_resampler_quality_level = RESAMPLER_QUALITY_NORMAL;
#endif

/* MIDI */
#define DEFAULT_MIDI_INPUT  "Off"

#define DEFAULT_MIDI_OUTPUT "Off"

static const unsigned midi_volume = 100;

/* Only applies to Android 7.0 (API 24) and up */
static const bool sustained_performance_mode = false;

static const bool vibrate_on_keypress        = false;
static const bool enable_device_vibration    = false;

/* Defines the strength of rumble effects
 * on OpenDingux devices */
#if defined(DINGUX) && defined(HAVE_LIBSHAKE)
#define DEFAULT_RUMBLE_GAIN 50
#else
#define DEFAULT_RUMBLE_GAIN 100
#endif

#ifdef HAVE_VULKAN
#define DEFAULT_VULKAN_GPU_INDEX 0
#endif

#ifdef HAVE_D3D10
#define DEFAULT_D3D10_GPU_INDEX 0
#endif

#ifdef HAVE_D3D11
#define DEFAULT_D3D11_GPU_INDEX 0
#endif

#ifdef HAVE_D3D12
#define DEFAULT_D3D12_GPU_INDEX 0
#endif

#if defined(HAKCHI)
#define DEFAULT_BUILDBOT_SERVER_URL "http://hakchicloud.com/Libretro_Cores/"
#elif defined(ANDROID)
#if defined(ANDROID_ARM_V7)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/android/latest/armeabi-v7a/"
#elif defined(ANDROID_ARM)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/android/latest/armeabi/"
#elif defined(ANDROID_AARCH64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/android/latest/arm64-v8a/"
#elif defined(ANDROID_X86)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/android/latest/x86/"
#elif defined(ANDROID_X64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/android/latest/x86_64/"
#else
#define DEFAULT_BUILDBOT_SERVER_URL ""
#endif
#elif defined(__QNX__)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/blackberry/latest/"
#elif defined(IOS)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/apple/ios/latest/"
#elif defined(OSX)
#if defined(__x86_64__)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/apple/osx/x86_64/latest/"
#elif defined(__i386__) || defined(__i486__) || defined(__i686__)
#define DEFAULT_BUILDBOT_SERVER_URL "http://bot.libretro.com/nightly/apple/osx/x86/latest/"
#elif defined(__aarch64__)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/apple/osx/arm64/latest/"
#else
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/apple/osx/ppc/latest/"
#endif
#elif defined(_WIN32) && !defined(_XBOX)
#if _MSC_VER >= 1910
#ifndef __WINRT__
#if defined(__x86_64__) || defined(_M_X64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-desktop/x64/latest/"
#elif defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(_M_IX86) || defined(_M_IA64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-desktop/x86/latest/"
#elif defined(__arm__) || defined(_M_ARM)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-desktop/arm/latest/"
#elif defined(__aarch64__) || defined(_M_ARM64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-desktop/arm64/latest/"
#endif
#else
#if defined(__x86_64__) || defined(_M_X64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-uwp/x64/latest/"
#elif defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(_M_IX86) || defined(_M_IA64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-uwp/x86/latest/"
#elif defined(__arm__) || defined(_M_ARM)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-uwp/arm/latest/"
#elif defined(__aarch64__) || defined(_M_ARM64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2017-uwp/arm64/latest/"
#endif
#endif
#elif _MSC_VER == 1600
#if defined(__x86_64__) || defined(_M_X64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2010/x86_64/latest/"
#elif defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(_M_IX86) || defined(_M_IA64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2010/x86/latest/"
#endif
#elif _MSC_VER == 1400
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2005/x86/latest/"
#elif _MSC_VER == 1310
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows-msvc2003/x86/latest/"
#else
#if defined(__x86_64__) || defined(_M_X64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows/x86_64/latest/"
#elif defined(__i386__) || defined(__i486__) || defined(__i686__) || defined(_M_IX86) || defined(_M_IA64)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/windows/x86/latest/"
#endif
#endif
#elif defined(__linux__)
#if defined(__x86_64__)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/linux/x86_64/latest/"
#elif defined(__i386__) || defined(__i486__) || defined(__i686__)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/linux/x86/latest/"
#elif defined(__arm__) && __ARM_ARCH == 7 && defined(__ARM_PCS_VFP)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/linux/armhf/latest/"
#else
#define DEFAULT_BUILDBOT_SERVER_URL ""
#endif
#elif defined(WIIU)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/nintendo/wiiu/latest/"
#elif defined(HAVE_LIBNX)
#define DEFAULT_BUILDBOT_SERVER_URL "http://buildbot.libretro.com/nightly/nintendo/switch/libnx/latest/"
#elif defined(_3DS)
#define DEFAULT_BUILDBOT_SERVER_URL envIsHomebrew() ? "http://buildbot.libretro.com/nightly/nintendo/3ds/latest/3dsx/" : "http://buildbot.libretro.com/nightly/nintendo/3ds/latest/cia/"
#else
#define DEFAULT_BUILDBOT_SERVER_URL ""
#endif

#define DEFAULT_BUILDBOT_ASSETS_SERVER_URL "http://buildbot.libretro.com/assets/"

#define DEFAULT_DISCORD_APP_ID "475456035851599874"

#define DEFAULT_AI_SERVICE_SOURCE_LANG 0

#define DEFAULT_AI_SERVICE_TARGET_LANG 0

#define DEFAULT_AI_SERVICE_ENABLE true

#define DEFAULT_AI_SERVICE_PAUSE false

#define DEFAULT_AI_SERVICE_MODE 1

#define DEFAULT_AI_SERVICE_URL "http://localhost:4404/"

#if defined(HAVE_FFMPEG) || defined(HAVE_MPV)
#define DEFAULT_BUILTIN_MEDIAPLAYER_ENABLE true
#else
#define DEFAULT_BUILTIN_MEDIAPLAYER_ENABLE false
#endif

#if defined(HAVE_IMAGEVIEWER)
#define DEFAULT_BUILTIN_IMAGEVIEWER_ENABLE true
#else
#define DEFAULT_BUILTIN_IMAGEVIEWER_ENABLE false
#endif

#define DEFAULT_FILTER_BY_CURRENT_CORE false

#endif
