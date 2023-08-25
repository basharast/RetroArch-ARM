/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2016-2017 - Hans-Kristian Arntzen
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

#include <compat/strl.h>
#include <string/stdstring.h>
#include <retro_timers.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#include "../../frontend/frontend_driver.h"
#include "../common/vulkan_common.h"
#include "../../verbosity.h"
#include "../../configuration.h"

typedef struct
{
   gfx_ctx_vulkan_data_t vk;
   int swap_interval;
   unsigned width;
   unsigned height;
} khr_display_ctx_data_t;

static void gfx_ctx_khr_display_destroy(void *data)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;
   if (!khr)
      return;

   vulkan_context_destroy(&khr->vk, true);
#ifdef HAVE_THREADS
   if (khr->vk.context.queue_lock)
      slock_free(khr->vk.context.queue_lock);
#endif
   free(khr);
}

static void gfx_ctx_khr_display_get_video_size(void *data,
      unsigned *width, unsigned *height)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;

   *width  = khr->width;
   *height = khr->height;
}

static void *gfx_ctx_khr_display_init(void *video_driver)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)
      calloc(1, sizeof(*khr));
   if (!khr)
       return NULL;

   if (!vulkan_context_init(&khr->vk, VULKAN_WSI_DISPLAY))
   {
      RARCH_ERR("[Vulkan]: Failed to create Vulkan context.\n");
      goto error;
   }

   frontend_driver_install_signal_handler();

   return khr;

error:
   gfx_ctx_khr_display_destroy(khr);
   return NULL;
}

static void gfx_ctx_khr_display_check_window(void *data, bool *quit,
      bool *resize, unsigned *width, unsigned *height)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;

   *resize = khr->vk.need_new_swapchain;

   if (khr->width != *width || khr->height != *height)
   {
      *width  = khr->width;
      *height = khr->height;
      *resize = true;
   }

   if ((bool)frontend_driver_get_signal_handler_state())
      *quit = true;
}

static bool gfx_ctx_khr_display_set_resize(void *data,
      unsigned width, unsigned height)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;

   khr->width                  = width;
   khr->height                 = height;

   if (!vulkan_create_swapchain(&khr->vk, khr->width, khr->height,
            khr->swap_interval))
   {
      RARCH_ERR("[Vulkan]: Failed to update swapchain.\n");
      return false;
   }

   if (khr->vk.created_new_swapchain)
      vulkan_acquire_next_image(&khr->vk);

   khr->vk.context.invalid_swapchain = true;
   khr->vk.need_new_swapchain        = false;
   return true;
}

static bool gfx_ctx_khr_display_set_video_mode(void *data,
      unsigned width, unsigned height,
      bool fullscreen)
{
   struct vulkan_display_surface_info info;
   khr_display_ctx_data_t *khr    = (khr_display_ctx_data_t*)data;
   settings_t *settings           = config_get_ptr();
   unsigned video_monitor_index   = settings->uints.video_monitor_index;

   if (!fullscreen)
   {
      width                       = 0;
      height                      = 0;
   }

   info.width                     = width;
   info.height                    = height;
   info.monitor_index             = video_monitor_index;

   if (!vulkan_surface_create(&khr->vk, VULKAN_WSI_DISPLAY, &info, NULL,
            0, 0, khr->swap_interval))
   {
      RARCH_ERR("[Vulkan]: Failed to create KHR_display surface.\n");
      goto error;
   }

   khr->width                     = khr->vk.context.swapchain_width;
   khr->height                    = khr->vk.context.swapchain_height;

   return true;

error:
   gfx_ctx_khr_display_destroy(data);
   return false;
}

static void gfx_ctx_khr_display_input_driver(void *data,
      const char *joypad_name,
      input_driver_t **input, void **input_data)
{
#ifdef HAVE_X11
   settings_t *settings = config_get_ptr();

   /* We cannot use the X11 input driver for DRM/KMS */
   if (string_is_equal(settings->arrays.input_driver, "x"))
   {
#ifdef HAVE_UDEV
      {
         /* Try to set it to udev instead */
         void *udev = input_driver_init_wrap(&input_udev, joypad_name);
         if (udev)
         {
            *input       = &input_udev;
            *input_data  = udev;
            return;
         }
      }
#endif
#if defined(__linux__) && !defined(ANDROID)
      {
         /* Try to set it to linuxraw instead */
         void *linuxraw = input_driver_init_wrap(&input_linuxraw, joypad_name);
         if (linuxraw)
         {
            *input       = &input_linuxraw;
            *input_data  = linuxraw;
            return;
         }
      }
#endif
   }
#endif

   *input      = NULL;
   *input_data = NULL;
}

static enum gfx_ctx_api gfx_ctx_khr_display_get_api(void *data)
{
   return GFX_CTX_VULKAN_API;
}

static bool gfx_ctx_khr_display_bind_api(void *data,
      enum gfx_ctx_api api, unsigned major, unsigned minor)
{
   if (api == GFX_CTX_VULKAN_API)
      return true;
   return false;
}

static void gfx_ctx_khr_display_set_flags(void *data, uint32_t flags) { }
static bool gfx_ctx_khr_display_has_focus(void *data) { return true; }
static bool gfx_ctx_khr_display_suppress_screensaver(void *data, bool enable) { return false; }
static gfx_ctx_proc_t gfx_ctx_khr_display_get_proc_address(const char *symbol) { return NULL; }

static void gfx_ctx_khr_display_set_swap_interval(void *data,
      int swap_interval)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;

   if (khr->swap_interval != swap_interval)
   {
      khr->swap_interval = swap_interval;
      if (khr->vk.swapchain)
         khr->vk.need_new_swapchain = true;
   }
}

static void gfx_ctx_khr_display_swap_buffers(void *data)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;
   if (khr->vk.context.has_acquired_swapchain)
   {
      khr->vk.context.has_acquired_swapchain = false;
      if (khr->vk.swapchain == VK_NULL_HANDLE)
      {
         retro_sleep(10);
      }
      else
         vulkan_present(&khr->vk, khr->vk.context.current_swapchain_index);
   }
   vulkan_acquire_next_image(&khr->vk);
}

static uint32_t gfx_ctx_khr_display_get_flags(void *data)
{
   uint32_t flags = 0;

#if defined(HAVE_SLANG) && defined(HAVE_SPIRV_CROSS)
   BIT32_SET(flags, GFX_CTX_FLAGS_SHADERS_SLANG);
#endif

   return flags;
}

static void *gfx_ctx_khr_display_get_context_data(void *data)
{
   khr_display_ctx_data_t *khr = (khr_display_ctx_data_t*)data;
   return &khr->vk.context;
}

const gfx_ctx_driver_t gfx_ctx_khr_display = {
   gfx_ctx_khr_display_init,
   gfx_ctx_khr_display_destroy,
   gfx_ctx_khr_display_get_api,
   gfx_ctx_khr_display_bind_api,
   gfx_ctx_khr_display_set_swap_interval,
   gfx_ctx_khr_display_set_video_mode,
   gfx_ctx_khr_display_get_video_size,
   NULL,                                        /* get_refresh_rate */
   NULL,                                        /* get_video_output_size */
   NULL,                                        /* get_video_output_prev */
   NULL,                                        /* get_video_output_next */
   NULL,                                        /* get_metrics */
   NULL,
   NULL,                                        /* update_title */
   gfx_ctx_khr_display_check_window,
   gfx_ctx_khr_display_set_resize,
   gfx_ctx_khr_display_has_focus,
   gfx_ctx_khr_display_suppress_screensaver,
   false,                                       /* has_windowed */
   gfx_ctx_khr_display_swap_buffers,
   gfx_ctx_khr_display_input_driver,
   gfx_ctx_khr_display_get_proc_address,
   NULL,
   NULL,
   NULL,
   "khr_display",
   gfx_ctx_khr_display_get_flags,
   gfx_ctx_khr_display_set_flags,
   NULL,
   gfx_ctx_khr_display_get_context_data,
   NULL                                         /* make_current */
};
