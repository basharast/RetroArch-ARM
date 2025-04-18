/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2015-2016 - Andre Leiradella
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

#include <string.h>
#include <ctype.h>

#include <file/file_path.h>
#include <string/stdstring.h>
#include <streams/interface_stream.h>
#include <streams/file_stream.h>
#include <features/features_cpu.h>
#include <formats/cdfs.h>
#include <formats/m3u_file.h>
#include <compat/strl.h>
#include <retro_miscellaneous.h>
#include <retro_math.h>
#include <retro_timers.h>
#include <net/net_http.h>
#include <libretro.h>
#include <lrc_hash.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#ifdef HAVE_GFX_WIDGETS
#include "../gfx/gfx_widgets.h"
#endif

#ifdef HAVE_THREADS
#include <rthreads/rthreads.h>
#endif

#ifdef HAVE_CHEATS
#include "../cheat_manager.h"
#endif

#ifdef HAVE_CHD
#include "streams/chd_stream.h"
#endif

#include "cheevos.h"
#include "cheevos_client.h"
#include "cheevos_menu.h"
#include "cheevos_locals.h"

#include "../network/netplay/netplay.h"

#include "../audio/audio_driver.h"
#include "../file_path_special.h"
#include "../paths.h"
#include "../command.h"
#include "../configuration.h"
#include "../performance_counters.h"
#include "../msg_hash.h"
#include "../retroarch.h"
#include "../runtime_file.h"
#include "../core.h"
#include "../core_option_manager.h"

#include "../tasks/tasks_internal.h"

#include "../deps/rcheevos/include/rc_runtime.h"
#include "../deps/rcheevos/include/rc_runtime_types.h"
#include "../deps/rcheevos/include/rc_hash.h"
#include "../deps/rcheevos/src/rcheevos/rc_libretro.h"

/* Define this macro to prevent cheevos from being deactivated when they trigger. */
#undef CHEEVOS_DONT_DEACTIVATE

static rcheevos_locals_t rcheevos_locals =
{
   {0},  /* runtime */
   {0},  /* game */
   {{0}},/* memory */
#ifdef HAVE_THREADS
   CMD_EVENT_NONE, /* queued_command */
#endif
   "",   /* displayname */
   "",   /* username */
   "",   /* token */
   "",   /* user_agent_prefix */
   "",   /* user_agent_core */
#ifdef HAVE_MENU
   NULL, /* menuitems */
   0,    /* menuitem_capacity */
   0,    /* menuitem_count */
#endif
   {RCHEEVOS_LOAD_STATE_NONE, 0, 0 },  /* load_info */
   false,/* hardcore_active */
   false,/* loaded */
   true, /* core_supports */
   false,/* leaderboards_enabled */
   false,/* leaderboard_notifications */
   false /* leaderboard_trackers */
};

rcheevos_locals_t* get_rcheevos_locals(void)
{
   return &rcheevos_locals;
}

#define CHEEVOS_MB(x)   ((x) * 1024 * 1024)

/*****************************************************************************
Supporting functions.
*****************************************************************************/

#ifndef CHEEVOS_VERBOSE
void rcheevos_log(const char *fmt, ...)
{
   (void)fmt;
}
#endif


static void rcheevos_achievement_disabled(
      rcheevos_racheevo_t* cheevo, unsigned address)
{
   if (!cheevo)
      return;

   CHEEVOS_ERR(RCHEEVOS_TAG
         "Achievement %u disabled (invalid address %06X): %s\n",
         cheevo->id, address, cheevo->title);
   CHEEVOS_FREE(cheevo->memaddr);
   cheevo->memaddr = NULL;
   cheevo->active |= RCHEEVOS_ACTIVE_UNSUPPORTED;
}

static void rcheevos_lboard_disabled(
      rcheevos_ralboard_t* lboard, unsigned address)
{
   if (!lboard)
      return;

   CHEEVOS_ERR(RCHEEVOS_TAG
         "Leaderboard %u disabled (invalid address %06X): %s\n",
         lboard->id, address, lboard->title);
   CHEEVOS_FREE(lboard->mem);
   lboard->mem = NULL;
}

static void rcheevos_handle_log_message(const char* message)
{
   CHEEVOS_LOG(RCHEEVOS_TAG "%s\n", message);
}

static void rcheevos_get_core_memory_info(unsigned id,
      rc_libretro_core_memory_info_t* info)
{
   retro_ctx_memory_info_t ctx_info;
   if (!info)
      return;

   ctx_info.id = id;
   if (core_get_memory(&ctx_info))
   {
      info->data = (unsigned char*)ctx_info.data;
      info->size = ctx_info.size;
   }
   else
   {
      info->data = NULL;
      info->size = 0;
   }
}

static int rcheevos_init_memory(rcheevos_locals_t* locals)
{
   unsigned i;
   int result;
   struct retro_memory_map mmap;
   rarch_system_info_t* system                 = &runloop_state_get_ptr()->system;
   rarch_memory_map_t* mmaps                   = &system->mmaps;
   struct retro_memory_descriptor *descriptors = (struct retro_memory_descriptor*)malloc(mmaps->num_descriptors * sizeof(*descriptors));
   if (!descriptors)
      return 0;

   mmap.descriptors = &descriptors[0];
   mmap.num_descriptors = mmaps->num_descriptors;

   /* RetroArch wraps the retro_memory_descriptor's 
    * in rarch_memory_descriptor_t's, pull them back out */
   for (i = 0; i < mmap.num_descriptors; ++i)
      memcpy(&descriptors[i], &mmaps->descriptors[i].core,
            sizeof(descriptors[0]));

   rc_libretro_init_verbose_message_callback(rcheevos_handle_log_message);
   result = rc_libretro_memory_init(&locals->memory, &mmap,
         rcheevos_get_core_memory_info, locals->game.console_id);

   free(descriptors);
   return result;
}

uint8_t* rcheevos_patch_address(unsigned address)
{
   /* Memory map was not previously initialized 
    * (no achievements for this game?), try now */
   if (rcheevos_locals.memory.count == 0)
      rcheevos_init_memory(&rcheevos_locals);
   return rc_libretro_memory_find(&rcheevos_locals.memory, address);
}

static unsigned rcheevos_peek(unsigned address,
      unsigned num_bytes, void* ud)
{
   uint8_t* data = rc_libretro_memory_find(
         &rcheevos_locals.memory, address);

   if (data)
   {
      switch (num_bytes)
      {
         case 4:
            return (data[3] << 24) | (data[2] << 16) | 
                   (data[1] <<  8) | (data[0]);
         case 3:
            return (data[2] << 16) | (data[1] << 8) | (data[0]);
         case 2:
            return (data[1] << 8)  | (data[0]);
         case 1:
            return data[0];
      }
   }

   return 0;
}

static void rcheevos_activate_achievements(void)
{
   unsigned i;
   int result;
   rcheevos_racheevo_t* achievement = rcheevos_locals.game.achievements;
   settings_t* settings = config_get_ptr();
   const uint8_t active_flag = rcheevos_locals.hardcore_active ? RCHEEVOS_ACTIVE_HARDCORE : RCHEEVOS_ACTIVE_SOFTCORE;

   for (i = 0; i < rcheevos_locals.game.achievement_count;
         i++, achievement++)
   {
      if ((achievement->active & active_flag) != 0)
      {
         result = rc_runtime_activate_achievement(&rcheevos_locals.runtime, achievement->id, achievement->memaddr, NULL, 0);
         if (result != RC_OK)
         {
            char buffer[256];
            buffer[0] = '\0';
            /* TODO/FIXME - localize */
            snprintf(buffer, sizeof(buffer),
               "Could not activate achievement %u \"%s\": %s",
               achievement->id, achievement->title, rc_error_str(result));

            if (settings->bools.cheevos_verbose_enable)
               runloop_msg_queue_push(buffer, 0, 4 * 60, false, NULL,
                  MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);

            CHEEVOS_ERR(RCHEEVOS_TAG "%s: mem %s\n", buffer, achievement->memaddr);
            achievement->active &= ~(RCHEEVOS_ACTIVE_HARDCORE | RCHEEVOS_ACTIVE_SOFTCORE);
            achievement->active |= RCHEEVOS_ACTIVE_UNSUPPORTED;

            CHEEVOS_FREE(achievement->memaddr);
            achievement->memaddr = NULL;
         }
      }
   }
}

static rcheevos_racheevo_t* rcheevos_find_cheevo(unsigned id)
{
   rcheevos_racheevo_t* cheevo = rcheevos_locals.game.achievements;
   rcheevos_racheevo_t* stop   = cheevo 
      + rcheevos_locals.game.achievement_count;

   for(; cheevo < stop; ++cheevo)
   {
      if (cheevo->id == id)
         return cheevo;
   }

   return NULL;
}

static bool rcheevos_is_player_active(void)
{
   if (netplay_driver_ctl(RARCH_NETPLAY_CTL_IS_SPECTATING, NULL))
      return false;

   /* TODO: disallow player slots other than player one unless it's a [Multi] set */

   return true;
}

void rcheevos_award_achievement(rcheevos_locals_t* locals,
      rcheevos_racheevo_t* cheevo, bool widgets_ready)
{
   const settings_t *settings = config_get_ptr();

   if (!cheevo)
      return;

   /* Deactivates the acheivement. */
   rc_runtime_deactivate_achievement(&locals->runtime, cheevo->id);

   cheevo->active &= ~RCHEEVOS_ACTIVE_SOFTCORE;
   if (locals->hardcore_active)
      cheevo->active &= ~RCHEEVOS_ACTIVE_HARDCORE;

   cheevo->unlock_time = cpu_features_get_time_usec();

   if (!rcheevos_is_player_active())
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Not awarding achievement %u, player not active\n",
            cheevo->id);
      return;
   }

   CHEEVOS_LOG(RCHEEVOS_TAG "Awarding achievement %u: %s (%s)\n",
         cheevo->id, cheevo->title, cheevo->description);

   /* Show the on screen message. */
   if (settings->bools.cheevos_visibility_unlock)
   {
#if defined(HAVE_GFX_WIDGETS)
      if (widgets_ready)
         gfx_widgets_push_achievement(msg_hash_to_str(MSG_ACHIEVEMENT_UNLOCKED), cheevo->title, cheevo->badge);
      else
#endif
      {
         char buffer[256];
         size_t _len    = strlcpy(buffer,
            msg_hash_to_str(MSG_ACHIEVEMENT_UNLOCKED),
            sizeof(buffer));
         buffer[_len  ] = ':';
         buffer[_len+1] = ' ';
         buffer[_len+2] = '\0';
         strlcat(buffer, cheevo->title, sizeof(buffer));
         runloop_msg_queue_push(buffer, 0, 2 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
         runloop_msg_queue_push(cheevo->description, 0, 3 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
      }
   }

   /* Start the award task (unofficial achievement 
    * unlocks are not submitted). */
   if (!(cheevo->active & RCHEEVOS_ACTIVE_UNOFFICIAL))
      rcheevos_client_award_achievement(cheevo->id);

#ifdef HAVE_AUDIOMIXER
   /* Play the unlock sound */
   if (settings->bools.cheevos_unlock_sound_enable)
      audio_driver_mixer_play_menu_sound(
            AUDIO_MIXER_SYSTEM_SLOT_ACHIEVEMENT_UNLOCK);
#endif

#ifdef HAVE_SCREENSHOTS
   /* Take a screenshot of the achievement. */
   if (settings->bools.cheevos_auto_screenshot)
   {
      size_t shotname_len  = sizeof(char) * 8192;
      char *shotname       = (char*)malloc(shotname_len);

      if (shotname)
      {
         snprintf(shotname, shotname_len, "%s/%s-cheevo-%u",
               settings->paths.directory_screenshot,
               path_basename(path_get(RARCH_PATH_BASENAME)),
               cheevo->id);
         shotname[shotname_len - 1] = '\0';

         if (take_screenshot(settings->paths.directory_screenshot,
                  shotname, true,
                  video_driver_cached_frame_has_valid_framebuffer(),
                  false, true))
            CHEEVOS_LOG(RCHEEVOS_TAG
                  "Captured screenshot for achievement %u\n",
                  cheevo->id);
         else
            CHEEVOS_LOG(RCHEEVOS_TAG
                  "Failed to capture screenshot for achievement %u\n",
                  cheevo->id);

         free(shotname);
      }
   }
#endif
}

static rcheevos_ralboard_t* rcheevos_find_lboard(unsigned id)
{
   rcheevos_ralboard_t* lboard = rcheevos_locals.game.leaderboards;
   rcheevos_ralboard_t* stop   = lboard 
      + rcheevos_locals.game.leaderboard_count;

   for (; lboard < stop; ++lboard)
   {
      if (lboard->id == id)
         return lboard;
   }

   return NULL;
}

static void rcheevos_lboard_submit(rcheevos_locals_t* locals,
      rcheevos_ralboard_t* lboard, int value, bool widgets_ready)
{
   size_t _len;
   char buffer[256];
   char formatted_value[16];

#if defined(HAVE_GFX_WIDGETS)
   /* Hide the tracker */
   if (gfx_widgets_ready())
      gfx_widgets_set_leaderboard_display(lboard->id, NULL);
#endif

   rc_runtime_format_lboard_value(formatted_value,
         sizeof(formatted_value),
         value, lboard->format);

   if (!rcheevos_is_player_active())
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Not submitting %s for leaderboard %u, player not active\n",
            formatted_value, lboard->id);
      return;
   }

   CHEEVOS_LOG(RCHEEVOS_TAG "Submitting %s for leaderboard %u\n",
         formatted_value, lboard->id);

   /* Show the on-screen message (regardless of notifications setting). */
   strlcpy(buffer, "Submitted ", sizeof(buffer));
   _len           = strlcat(buffer, formatted_value, sizeof(buffer));
   buffer[_len  ] = ' ';
   buffer[_len+1] = 'f';
   buffer[_len+2] = 'o';
   buffer[_len+3] = 'r';
   buffer[_len+4] = ' ';
   buffer[_len+5] = '\0';
   strlcat(buffer, lboard->title, sizeof(buffer));
   runloop_msg_queue_push(buffer, 0, 2 * 60, false, NULL,
         MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);

   /* Start the submit task */
   rcheevos_client_submit_lboard_entry(lboard->id, value);
}

static void rcheevos_lboard_canceled(rcheevos_ralboard_t * lboard,
      bool widgets_ready)
{
   char buffer[256];
   if (!lboard)
      return;

   CHEEVOS_LOG(RCHEEVOS_TAG "Leaderboard %u canceled: %s\n",
         lboard->id, lboard->title);

#if defined(HAVE_GFX_WIDGETS)
   if (widgets_ready)
      gfx_widgets_set_leaderboard_display(lboard->id, NULL);
#endif

   if (rcheevos_locals.leaderboard_notifications)
   {
      strlcpy(buffer, "Leaderboard attempt failed: ",
            sizeof(buffer));
      strlcat(buffer, lboard->title, sizeof(buffer));
      runloop_msg_queue_push(buffer, 0, 2 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
   }
}

static void rcheevos_lboard_started(
      rcheevos_ralboard_t * lboard, int value,
      bool widgets_ready)
{
   char buffer[256];
   if (!lboard)
      return;

   CHEEVOS_LOG(RCHEEVOS_TAG "Leaderboard %u started: %s\n",
         lboard->id, lboard->title);

   if (!rcheevos_is_player_active())
      return;

#if defined(HAVE_GFX_WIDGETS)
   if (widgets_ready && rcheevos_locals.leaderboard_trackers)
   {
      rc_runtime_format_lboard_value(buffer,
            sizeof(buffer), value, lboard->format);
      gfx_widgets_set_leaderboard_display(lboard->id, buffer);
   }
#endif

   if (rcheevos_locals.leaderboard_notifications)
   {
      size_t _len;
      strlcpy(buffer, "Leaderboard attempt started: ",
            sizeof(buffer));
      _len = strlcat(buffer, lboard->title, sizeof(buffer));
      if (lboard->description && *lboard->description)
      {
         buffer[_len  ] = ' ';
         buffer[_len+1] = '-';
         buffer[_len+2] = ' ';
         buffer[_len+3] = '\0';
         strlcat(buffer, lboard->description, sizeof(buffer));
      }

      runloop_msg_queue_push(buffer, 0, 2 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
   }
}

#if defined(HAVE_GFX_WIDGETS)
static void rcheevos_lboard_updated(
      rcheevos_ralboard_t* lboard, int value,
      bool widgets_ready)
{
   if (!lboard)
      return;

   if (widgets_ready && rcheevos_locals.leaderboard_trackers)
   {
      char buffer[32];
      rc_runtime_format_lboard_value(buffer,
            sizeof(buffer), value, lboard->format);
      gfx_widgets_set_leaderboard_display(lboard->id,
         rcheevos_is_player_active() ? buffer : NULL);
   }
}

static void rcheevos_challenge_started(
      rcheevos_racheevo_t* cheevo, int value,
      bool widgets_ready)
{
   settings_t* settings = config_get_ptr();
   if (     cheevo 
         && widgets_ready 
         && settings->bools.cheevos_challenge_indicators
         && rcheevos_is_player_active())
      gfx_widgets_set_challenge_display(cheevo->id, cheevo->badge);
}

static void rcheevos_challenge_ended(
      rcheevos_racheevo_t* cheevo, int value,
      bool widgets_ready)
{
   if (cheevo && widgets_ready)
      gfx_widgets_set_challenge_display(cheevo->id, NULL);
}

#endif

int rcheevos_get_richpresence(char *s, size_t len)
{
   if (rcheevos_is_player_active())
   {
      int ret = rc_runtime_get_richpresence(
            &rcheevos_locals.runtime, s, (unsigned)len,
            &rcheevos_peek, NULL, NULL);

      if (ret <= 0 && rcheevos_locals.game.title)
      {
         /* TODO/FIXME - localize */
         strlcpy(s, "Playing ", len);
         strlcat(s, rcheevos_locals.game.title, len);
      }
      return ret;
   }
   if (rcheevos_locals.game.title)
   {
      /* TODO/FIXME - localize */
      strlcpy(s, "Spectating ", len);
      return (int)strlcat(s, rcheevos_locals.game.title, len);
   }
   return 0;
}

void rcheevos_reset_game(bool widgets_ready)
{
#if defined(HAVE_GFX_WIDGETS)
   /* Hide any visible trackers */
   if (widgets_ready)
   {
      unsigned i;
      rcheevos_racheevo_t* cheevo;
      rcheevos_ralboard_t* lboard = rcheevos_locals.game.leaderboards;
      for (i = 0; i < rcheevos_locals.game.leaderboard_count;
            ++i, ++lboard)
         gfx_widgets_set_leaderboard_display(lboard->id, NULL);
      cheevo = rcheevos_locals.game.achievements;
      for (i = 0; i < rcheevos_locals.game.achievement_count;
            ++i, ++cheevo)
         gfx_widgets_set_challenge_display(cheevo->id, NULL);
   }
#endif

   rc_runtime_reset(&rcheevos_locals.runtime);

   /* Some cores reallocate memory on reset, 
    * make sure we update our pointers */
   if (rcheevos_locals.memory.total_size > 0)
      rcheevos_init_memory(&rcheevos_locals);
}

bool rcheevos_hardcore_active(void)
{
   return rcheevos_locals.hardcore_active;
}

void rcheevos_pause_hardcore(void)
{
   if (rcheevos_locals.hardcore_active)
      rcheevos_toggle_hardcore_paused();
}

#ifdef HAVE_THREADS
static bool rcheevos_timer_check(void* userdata)
{
   retro_time_t stop_time = *(retro_time_t*)userdata;
   retro_time_t now       = cpu_features_get_time_usec();
   return (now < stop_time);
}
#endif

bool rcheevos_unload(void)
{
   settings_t* settings  = config_get_ptr();

   /* Immediately mark the game as unloaded 
      so the ping thread will terminate normally */
   rcheevos_locals.game.id         = -1;
   rcheevos_locals.game.console_id = 0;
   rcheevos_locals.game.hash       = NULL;

#ifdef HAVE_THREADS
   if (rcheevos_locals.load_info.state < RCHEEVOS_LOAD_STATE_DONE &&
       rcheevos_locals.load_info.state != RCHEEVOS_LOAD_STATE_NONE)
   {
      /* allow up to 5 seconds for pending tasks to run */
      retro_time_t stop_time = cpu_features_get_time_usec() + 5000000;

      rcheevos_locals.load_info.state = RCHEEVOS_LOAD_STATE_ABORTED;
      CHEEVOS_LOG(RCHEEVOS_TAG "Asked the load tasks to terminate\n");

      /* Wait for pending tasks to run */
      task_queue_wait(rcheevos_timer_check, &stop_time);
      /* Clean up after completed tasks */
      task_queue_check();
   }

   rcheevos_locals.queued_command = CMD_EVENT_NONE;
#endif

   if (rcheevos_locals.memory.count > 0)
      rc_libretro_memory_destroy(&rcheevos_locals.memory);

   if (rcheevos_locals.loaded)
   {
#ifdef HAVE_MENU
      rcheevos_menu_reset_badges();

      if (rcheevos_locals.menuitems)
      {
         CHEEVOS_FREE(rcheevos_locals.menuitems);
         rcheevos_locals.menuitems              = NULL;
         rcheevos_locals.menuitem_capacity      =
            rcheevos_locals.menuitem_count      = 0;
      }
#endif

      if (rcheevos_locals.game.title)
      {
         CHEEVOS_FREE(rcheevos_locals.game.title);
         rcheevos_locals.game.title             = NULL;
      }

      rcheevos_locals.loaded                    = false;
      rcheevos_locals.hardcore_active           = false;

      rc_libretro_hash_set_destroy(&rcheevos_locals.game.hashes);
   }

#ifdef HAVE_THREADS
   rcheevos_locals.queued_command = CMD_EVENT_NONE;
#endif

   rc_runtime_destroy(&rcheevos_locals.runtime);

   /* If the config-level token has been cleared, 
    * we need to re-login on loading the next game */
   if (!settings->arrays.cheevos_token[0])
      rcheevos_locals.token[0]                  = '\0';

   rcheevos_locals.load_info.state = RCHEEVOS_LOAD_STATE_NONE;
   return true;
}

static void rcheevos_toggle_hardcore_achievements(
      rcheevos_locals_t *locals)
{
   const unsigned active_mask  = 
      RCHEEVOS_ACTIVE_SOFTCORE | RCHEEVOS_ACTIVE_HARDCORE | RCHEEVOS_ACTIVE_UNSUPPORTED;
   rcheevos_racheevo_t* cheevo = locals->game.achievements;
   rcheevos_racheevo_t* stop   = cheevo + locals->game.achievement_count;

   while (cheevo < stop)
   {
      if ((cheevo->active & active_mask) == RCHEEVOS_ACTIVE_HARDCORE)
      {
         /* player has unlocked achievement in non-hardcore,
          * but has not unlocked in hardcore. Toggle state */
         if (locals->hardcore_active)
         {
            rc_runtime_activate_achievement(&locals->runtime, cheevo->id, cheevo->memaddr, NULL, 0);
            CHEEVOS_LOG(RCHEEVOS_TAG "Achievement %u activated: %s\n", cheevo->id, cheevo->title);
         }
         else
         {
            rc_runtime_deactivate_achievement(&locals->runtime, cheevo->id);
            CHEEVOS_LOG(RCHEEVOS_TAG "Achievement %u deactivated: %s\n", cheevo->id, cheevo->title);
         }
      }

      ++cheevo;
   }
}

static void rcheevos_activate_leaderboards(void)
{
   unsigned i;
   int result;
   rcheevos_ralboard_t* leaderboard = rcheevos_locals.game.leaderboards;
   const settings_t *settings       = config_get_ptr();

   for (i = 0; i < rcheevos_locals.game.leaderboard_count;
         ++i, ++leaderboard)
   {
      if (!leaderboard->mem)
         continue;

      result = rc_runtime_activate_lboard(
            &rcheevos_locals.runtime, leaderboard->id,
            leaderboard->mem, NULL, 0);
      if (result != RC_OK)
      {
         char buffer[256];
         buffer[0] = '\0';
         /* TODO/FIXME - localize */
         snprintf(buffer, sizeof(buffer),
            "Could not activate leaderboard %u \"%s\": %s",
            leaderboard->id, leaderboard->title, rc_error_str(result));

         if (settings->bools.cheevos_verbose_enable)
            runloop_msg_queue_push(buffer, 0, 4 * 60, false, NULL,
               MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);

         CHEEVOS_ERR(RCHEEVOS_TAG "%s: mem %s\n", buffer, leaderboard->mem);

         CHEEVOS_FREE(leaderboard->mem);
         leaderboard->mem = NULL;
      }
   }
}

static void rcheevos_deactivate_leaderboards(void)
{
   rcheevos_ralboard_t* lboard = rcheevos_locals.game.leaderboards;
   rcheevos_ralboard_t* stop   = lboard + 
      rcheevos_locals.game.leaderboard_count;

   for (; lboard < stop; ++lboard)
   {
      if (lboard->mem)
      {
         rc_runtime_deactivate_lboard(&rcheevos_locals.runtime,
               lboard->id);

#if defined(HAVE_GFX_WIDGETS)
         /* Hide any visible trackers */
         gfx_widgets_set_leaderboard_display(lboard->id, NULL);
#endif
      }
   }
}

void rcheevos_leaderboards_enabled_changed(void)
{
   const settings_t* settings           = config_get_ptr();
   const bool leaderboards_enabled      = rcheevos_locals.leaderboards_enabled;
   const bool leaderboard_trackers      = rcheevos_locals.leaderboard_trackers;

   rcheevos_locals.leaderboards_enabled = rcheevos_locals.hardcore_active;

   if (string_is_equal(settings->arrays.cheevos_leaderboards_enable, "true"))
   {
      rcheevos_locals.leaderboard_notifications = true;
      rcheevos_locals.leaderboard_trackers = true;
   }
#if defined(HAVE_GFX_WIDGETS)
   else if (string_is_equal(
            settings->arrays.cheevos_leaderboards_enable, "trackers"))
   {
      rcheevos_locals.leaderboard_notifications = false;
      rcheevos_locals.leaderboard_trackers      = true;
   }
   else if (string_is_equal(
            settings->arrays.cheevos_leaderboards_enable, "notifications"))
   {
      rcheevos_locals.leaderboard_notifications = true;
      rcheevos_locals.leaderboard_trackers      = false;
   }
#endif
   else
   {
      rcheevos_locals.leaderboards_enabled      = false;
      rcheevos_locals.leaderboard_notifications = false;
      rcheevos_locals.leaderboard_trackers      = false;
   }

   if (rcheevos_locals.loaded)
   {
      if (leaderboards_enabled != rcheevos_locals.leaderboards_enabled)
      {
         if (rcheevos_locals.leaderboards_enabled)
            rcheevos_activate_leaderboards();
         else
            rcheevos_deactivate_leaderboards();
      }

#if defined(HAVE_GFX_WIDGETS)
      if (!rcheevos_locals.leaderboard_trackers && leaderboard_trackers)
      {
         /* Hide any visible trackers */
         unsigned i;
         rcheevos_ralboard_t* lboard = rcheevos_locals.game.leaderboards;

         for (i = 0; i < rcheevos_locals.game.leaderboard_count; ++i, ++lboard)
         {
            if (lboard->mem)
               gfx_widgets_set_leaderboard_display(lboard->id, NULL);
         }
      }
#endif
   }
}

static void rcheevos_enforce_hardcore_settings(void)
{
   /* disable slowdown */
   runloop_state_get_ptr()->flags &= ~RUNLOOP_FLAG_SLOWMOTION;
}

static void rcheevos_toggle_hardcore_active(rcheevos_locals_t* locals)
{
   settings_t* settings = config_get_ptr();
   bool rewind_enable   = settings->bools.rewind_enable;

   if (!locals->hardcore_active)
   {
      /* Activate hardcore */
      locals->hardcore_active = true;

      /* If one or more invalid settings is enabled, abort*/
      rcheevos_validate_config_settings();
      if (!locals->hardcore_active)
         return;

#ifdef HAVE_CHEATS
      /* If one or more emulator managed cheats is active, abort */
      cheat_manager_apply_cheats();
      if (!locals->hardcore_active)
         return;
#endif

      if (locals->loaded)
      {
         const char* msg = msg_hash_to_str(
               MSG_CHEEVOS_HARDCORE_MODE_ENABLE);
         CHEEVOS_LOG("%s\n", msg);
         runloop_msg_queue_push(msg, 0, 3 * 60, true, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);

         rcheevos_enforce_hardcore_settings();

         /* Reactivate leaderboards */
         if (locals->leaderboards_enabled)
            rcheevos_activate_leaderboards();

         /* reset the game */
         command_event(CMD_EVENT_RESET, NULL);
      }

      /* deinit rewind */
      if (rewind_enable)
      {
#ifdef HAVE_THREADS
         if (!task_is_on_main_thread())
         {
            /* have to "schedule" this.
             * CMD_EVENT_REWIND_DEINIT should
             * only be called on the main thread */
            rcheevos_locals.queued_command = CMD_EVENT_REWIND_DEINIT;
         }
         else
#endif
            command_event(CMD_EVENT_REWIND_DEINIT, NULL);
      }
   }
   else
   {
      /* pause hardcore */
      locals->hardcore_active = false;

      if (locals->loaded)
      {
         CHEEVOS_LOG(RCHEEVOS_TAG "Hardcore paused\n");

         /* deactivate leaderboards */
         rcheevos_deactivate_leaderboards();
      }

      /* re-init rewind */
      if (rewind_enable)
      {
#ifdef HAVE_THREADS
         if (!task_is_on_main_thread())
         {
            /* have to "schedule" this.
             * CMD_EVENT_REWIND_INIT should
             * only be called on the main thread */
            rcheevos_locals.queued_command = CMD_EVENT_REWIND_INIT;
         }
         else
#endif
            command_event(CMD_EVENT_REWIND_INIT, NULL);
      }
   }

   if (locals->loaded)
      rcheevos_toggle_hardcore_achievements(locals);
}

void rcheevos_toggle_hardcore_paused(void)
{
   settings_t* settings = config_get_ptr();
   /* if hardcore mode is not enabled, we can't toggle whether its active */
   if (settings->bools.cheevos_hardcore_mode_enable)
      rcheevos_toggle_hardcore_active(&rcheevos_locals);
}

void rcheevos_hardcore_enabled_changed(void)
{
   /* called whenever a setting that could potentially affect hardcore enabledness changes
    * (i.e. cheevos_enable, hardcore_mode_enable) to synchronize the internal state to the configs.
    * also called when a game is first loaded to synchronize the internal state to the configs. */
   const settings_t* settings = config_get_ptr();
   const bool enabled         = settings 
      && settings->bools.cheevos_enable 
      && settings->bools.cheevos_hardcore_mode_enable;

   if (enabled != rcheevos_locals.hardcore_active)
   {
      rcheevos_toggle_hardcore_active(&rcheevos_locals);

      /* update leaderboard state flags */
      rcheevos_leaderboards_enabled_changed();
   }
   else if (rcheevos_locals.hardcore_active && rcheevos_locals.loaded)
   {
      /* hardcore enabledness didn't change, but hardcore is active, so make
       * sure to enforce the restrictions. */
      rcheevos_enforce_hardcore_settings();
   }
}

void rcheevos_validate_config_settings(void)
{
   int i;
   const rc_disallowed_setting_t 
      *disallowed_settings          = NULL;
   core_option_manager_t* coreopts  = NULL;
   struct retro_system_info *system = 
      &runloop_state_get_ptr()->system.info;
   const settings_t* settings = config_get_ptr();

   if (!system->library_name || !rcheevos_locals.hardcore_active)
      return;

   if (!settings->bools.video_frame_delay_auto && settings->uints.video_frame_delay != 0) {
      const char* error = "Hardcore paused. Manual video frame delay setting not allowed.";
      CHEEVOS_LOG(RCHEEVOS_TAG "%s\n", error);
      rcheevos_pause_hardcore();

      runloop_msg_queue_push(error, 0, 4 * 60, false, NULL,
         MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_WARNING);
      return;
   }

   if (!(disallowed_settings 
            = rc_libretro_get_disallowed_settings(system->library_name)))
      return;

   if (!retroarch_ctl(RARCH_CTL_CORE_OPTIONS_LIST_GET, &coreopts))
      return;

   for (i = 0; i < (int)coreopts->size; i++)
   {
      const char* key = coreopts->opts[i].key;
      const char* val = core_option_manager_get_val(coreopts, i);
      if (!rc_libretro_is_setting_allowed(disallowed_settings, key, val))
      {
         size_t _len;
         char buffer[256];
         strlcpy(buffer,
               "Hardcore paused. Setting not allowed: ",
               sizeof(buffer));
         _len           = strlcat(buffer, key, sizeof(buffer));
         buffer[_len  ] = '=';
         buffer[_len+1] = '\0';
         strlcat(buffer, val, sizeof(buffer));
         CHEEVOS_LOG(RCHEEVOS_TAG "%s\n", buffer);
         rcheevos_pause_hardcore();

         runloop_msg_queue_push(buffer, 0, 4 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_WARNING);

         break;
      }
   }

   if (rcheevos_locals.game.console_id &&
      !rc_libretro_is_system_allowed(system->library_name, rcheevos_locals.game.console_id))
   {
      char buffer[256];
      buffer[0] = '\0';
      /* TODO/FIXME - localize */
      snprintf(buffer, sizeof(buffer),
            "Hardcore paused. You cannot earn hardcore achievements for %s using %s",
            rc_console_name(rcheevos_locals.game.console_id), system->library_name);
      CHEEVOS_LOG(RCHEEVOS_TAG "%s\n", buffer);
      rcheevos_pause_hardcore();

      runloop_msg_queue_push(buffer, 0, 4 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_WARNING);
      return;
   }
}

static void rcheevos_runtime_event_handler(
      const rc_runtime_event_t* runtime_event)
{
#if defined(HAVE_GFX_WIDGETS)
   bool widgets_ready = gfx_widgets_ready();
#else
   bool widgets_ready = false;
#endif

   switch (runtime_event->type)
   {
#if defined(HAVE_GFX_WIDGETS)
      case RC_RUNTIME_EVENT_LBOARD_UPDATED:
         rcheevos_lboard_updated(
               rcheevos_find_lboard(runtime_event->id),
               runtime_event->value, widgets_ready);
         break;

      case RC_RUNTIME_EVENT_ACHIEVEMENT_PRIMED:
         rcheevos_challenge_started(
               rcheevos_find_cheevo(runtime_event->id),
               runtime_event->value, widgets_ready);
         break;

      case RC_RUNTIME_EVENT_ACHIEVEMENT_UNPRIMED:
         rcheevos_challenge_ended(
               rcheevos_find_cheevo(runtime_event->id),
               runtime_event->value, widgets_ready);
         break;
#endif

      case RC_RUNTIME_EVENT_ACHIEVEMENT_TRIGGERED:
         rcheevos_award_achievement(
               &rcheevos_locals,
               rcheevos_find_cheevo(runtime_event->id), widgets_ready);
         break;

      case RC_RUNTIME_EVENT_LBOARD_STARTED:
         rcheevos_lboard_started(
               rcheevos_find_lboard(runtime_event->id),
               runtime_event->value, widgets_ready);
         break;

      case RC_RUNTIME_EVENT_LBOARD_CANCELED:
         rcheevos_lboard_canceled(
               rcheevos_find_lboard(runtime_event->id),
               widgets_ready);
         break;

      case RC_RUNTIME_EVENT_LBOARD_TRIGGERED:
         rcheevos_lboard_submit(
               &rcheevos_locals,
               rcheevos_find_lboard(runtime_event->id),
               runtime_event->value, widgets_ready);
         break;

      case RC_RUNTIME_EVENT_ACHIEVEMENT_DISABLED:
         rcheevos_achievement_disabled(
               rcheevos_find_cheevo(runtime_event->id),
               runtime_event->value);
         break;

      case RC_RUNTIME_EVENT_LBOARD_DISABLED:
         rcheevos_lboard_disabled(
               rcheevos_find_lboard(runtime_event->id),
               runtime_event->value);
         break;

      default:
         break;
   }
}

static int rcheevos_runtime_address_validator(unsigned address)
{
   return rc_libretro_memory_find(
            &rcheevos_locals.memory, address) != NULL;
}

static void rcheevos_validate_memrefs(rcheevos_locals_t* locals)
{
   if (!rcheevos_init_memory(locals))
   {
      const settings_t* settings = config_get_ptr();
      /* some cores (like Mupen64-Plus) don't expose the memory until the
       * first call to retro_run. in that case, there will be a total_size
       * of memory reported by the core, but init will return false, as
       * all of the pointers were null. if we're still loading the game,
       * just reset the memory count and we'll re-evaluate in 
       * rcheevos_test()
       */
      if (!locals->loaded)
      {
         /* If no memory was exposed, report the error now 
          * instead of waiting */
         if (locals->memory.total_size != 0)
         {
            locals->memory.count = 0;
            return;
         }
      }

      rcheevos_locals.core_supports = false;

      CHEEVOS_ERR(RCHEEVOS_TAG "No memory exposed by core\n");

      if (settings && settings->bools.cheevos_verbose_enable)
         runloop_msg_queue_push(msg_hash_to_str(MENU_ENUM_LABEL_VALUE_CANNOT_ACTIVATE_ACHIEVEMENTS_WITH_THIS_CORE),
            0, 4 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_WARNING);

      rcheevos_unload();
      rcheevos_pause_hardcore();
      return;
   }

   rc_runtime_validate_addresses(&locals->runtime,
         rcheevos_runtime_event_handler,
         rcheevos_runtime_address_validator);
}

/*****************************************************************************
Test all the achievements (call once per frame).
*****************************************************************************/
void rcheevos_test(void)
{
#ifdef HAVE_THREADS
   if (rcheevos_locals.queued_command != CMD_EVENT_NONE)
   {
      command_event(rcheevos_locals.queued_command, NULL);
      rcheevos_locals.queued_command = CMD_EVENT_NONE;
   }
#endif

   if (!rcheevos_locals.loaded)
      return;

   /* We were unable to initialize memory earlier, try now */
   if (rcheevos_locals.memory.count == 0)
   {
      rcheevos_validate_memrefs(&rcheevos_locals);

      /* rcheevos_validate_memrefs may decide the core doesn't support achievements and
       * disable them. if so, bail. */
      if (!rcheevos_locals.loaded)
         return;
   }

   rc_runtime_do_frame(&rcheevos_locals.runtime,
         &rcheevos_runtime_event_handler, rcheevos_peek, NULL, 0);
}

size_t rcheevos_get_serialize_size(void)
{
   if (!rcheevos_locals.loaded)
      return 0;
   return rc_runtime_progress_size(&rcheevos_locals.runtime, NULL);
}

bool rcheevos_get_serialized_data(void* buffer)
{
   if (!rcheevos_locals.loaded)
      return false;
   return (rc_runtime_serialize_progress(
            buffer, &rcheevos_locals.runtime, NULL) == RC_OK);
}

bool rcheevos_set_serialized_data(void* buffer)
{
   if (rcheevos_locals.loaded && buffer)
   {
      const int result = rc_runtime_deserialize_progress(
         &rcheevos_locals.runtime, (const unsigned char*)buffer, NULL);

#if defined(HAVE_GFX_WIDGETS)
      if (gfx_widgets_ready() && rcheevos_is_player_active())
      {
         settings_t* settings = config_get_ptr();

         if (rcheevos_locals.leaderboard_trackers)
         {
            unsigned i;
            rc_runtime_lboard_t* lboard = rcheevos_locals.runtime.lboards;
            for (i = 0; i < rcheevos_locals.runtime.lboard_count; ++i, ++lboard)
            {
               if (!lboard->lboard)
                  continue;

               if (lboard->lboard->state == RC_LBOARD_STATE_STARTED)
               {
                  rcheevos_ralboard_t* ralboard = rcheevos_find_lboard(lboard->id);
                  if (ralboard != NULL)
                  {
                     char value[32];
                     rc_runtime_format_lboard_value(value, sizeof(value), lboard->value, ralboard->format);
                     gfx_widgets_set_leaderboard_display(lboard->id, value);
                  }
               }
               else
               {
                  gfx_widgets_set_leaderboard_display(lboard->id, NULL);
               }
            }
         }

         if (settings->bools.cheevos_challenge_indicators)
         {
            unsigned i;
            rc_runtime_trigger_t* cheevo = rcheevos_locals.runtime.triggers;
            for (i = 0; i < rcheevos_locals.runtime.trigger_count; ++i, ++cheevo)
            {
               if (!cheevo->trigger)
                  continue;

               if (cheevo->trigger->state == RC_TRIGGER_STATE_PRIMED)
               {
                  rcheevos_racheevo_t* racheevo = rcheevos_find_cheevo(cheevo->id);
                  if (racheevo != NULL)
                     gfx_widgets_set_challenge_display(racheevo->id, racheevo->badge);
               }
               else
               {
                  gfx_widgets_set_challenge_display(cheevo->id, NULL);
               }
            }
         }
      }
#endif

      return (result == RC_OK);
   }

   return false;
}

void rcheevos_set_support_cheevos(bool state)
{
   rcheevos_locals.core_supports = state;
}

bool rcheevos_get_support_cheevos(void)
{
   return rcheevos_locals.core_supports;
}

const char* rcheevos_get_hash(void)
{
   return (rcheevos_locals.game.hash != NULL) ?
      rcheevos_locals.game.hash :
      msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NOT_AVAILABLE);
}

/* hooks for rc_hash library */

static void* rc_hash_handle_file_open(const char* path)
{
   return intfstream_open_file(path,
         RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);
}

static void rc_hash_handle_file_seek(
      void* file_handle, int64_t offset, int origin)
{
   intfstream_seek((intfstream_t*)file_handle, offset, origin);
}

static int64_t rc_hash_handle_file_tell(void* file_handle)
{
   return intfstream_tell((intfstream_t*)file_handle);
}

static size_t rc_hash_handle_file_read(
      void* file_handle, void* buffer, size_t requested_bytes)
{
   return intfstream_read((intfstream_t*)file_handle,
         buffer, requested_bytes);
}

static void rc_hash_handle_file_close(void* file_handle)
{
   intfstream_close((intfstream_t*)file_handle);
   CHEEVOS_FREE(file_handle);
}

#ifdef HAVE_CHD
static void* rc_hash_handle_chd_open_track(
      const char* path, uint32_t track)
{
   cdfs_track_t* cdfs_track;

   switch (track)
   {
      case RC_HASH_CDTRACK_FIRST_DATA:
         cdfs_track = cdfs_open_data_track(path);
         break;

      case RC_HASH_CDTRACK_LAST:
         cdfs_track = cdfs_open_track(path, CHDSTREAM_TRACK_LAST);
         break;

      case RC_HASH_CDTRACK_LARGEST:
         cdfs_track = cdfs_open_track(path, CHDSTREAM_TRACK_PRIMARY);
         break;

      default:
         cdfs_track = cdfs_open_track(path, track);
         break;
   }

   if (cdfs_track)
   {
      cdfs_file_t* file = (cdfs_file_t*)malloc(sizeof(cdfs_file_t));
      if (cdfs_open_file(file, cdfs_track, NULL))
         return file; /* ASSERT: file owns cdfs_track now */

      CHEEVOS_FREE(file);
      cdfs_close_track(cdfs_track); /* ASSERT: this free()s cdfs_track */
   }

   return NULL;
}

static size_t rc_hash_handle_chd_read_sector(
      void* track_handle, uint32_t sector,
      void* buffer, size_t requested_bytes)
{
   cdfs_file_t* file = (cdfs_file_t*)track_handle;
   uint32_t track_sectors = cdfs_get_num_sectors(file);

   sector -= cdfs_get_first_sector(file);
   if (sector >= track_sectors)
      return 0;

   cdfs_seek_sector(file, sector);
   return cdfs_read_file(file, buffer, requested_bytes);
}

static uint32_t rc_hash_handle_chd_first_track_sector(
   void* track_handle)
{
   cdfs_file_t* file = (cdfs_file_t*)track_handle;
   return cdfs_get_first_sector(file);
}

static void rc_hash_handle_chd_close_track(void* track_handle)
{
   cdfs_file_t* file = (cdfs_file_t*)track_handle;
   if (file)
   {
      cdfs_close_track(file->track);
      cdfs_close_file(file); /* ASSERT: this does not free() file */
      CHEEVOS_FREE(file);
   }
}

#endif

static void rc_hash_reset_cdreader_hooks(void);

static void* rc_hash_handle_cd_open_track(
      const char* path, uint32_t track)
{
   struct rc_hash_cdreader cdreader;

   if (string_is_equal_noncase(path_get_extension(path), "chd"))
   {
#ifdef HAVE_CHD
      /* special handlers for CHD file */
      memset(&cdreader, 0, sizeof(cdreader));
      cdreader.open_track = rc_hash_handle_cd_open_track;
      cdreader.read_sector = rc_hash_handle_chd_read_sector;
      cdreader.close_track = rc_hash_handle_chd_close_track;
      cdreader.first_track_sector = rc_hash_handle_chd_first_track_sector;
      rc_hash_init_custom_cdreader(&cdreader);

      return rc_hash_handle_chd_open_track(path, track);
#else
      CHEEVOS_LOG(RCHEEVOS_TAG "Cannot generate hash from CHD without HAVE_CHD compile flag\n");
      return NULL;
#endif
   }
   else
   {
      /* not a CHD file, use the default handlers */
      rc_hash_get_default_cdreader(&cdreader);
      rc_hash_reset_cdreader_hooks();
      return cdreader.open_track(path, track);
   }
}

static void rc_hash_reset_cdreader_hooks(void)
{
   struct rc_hash_cdreader cdreader;
   rc_hash_get_default_cdreader(&cdreader);
   cdreader.open_track = rc_hash_handle_cd_open_track;
   rc_hash_init_custom_cdreader(&cdreader);
}

/* end hooks */

void rcheevos_show_mastery_placard(void)
{
   char title[256];
   const settings_t* settings = config_get_ptr();

   if (rcheevos_locals.game.mastery_placard_shown)
      return;

   rcheevos_locals.game.mastery_placard_shown = true;

   snprintf(title, sizeof(title),
      msg_hash_to_str(rcheevos_locals.hardcore_active 
         ? MSG_CHEEVOS_MASTERED_GAME 
         : MSG_CHEEVOS_COMPLETED_GAME),
      rcheevos_locals.game.title);
   title[sizeof(title) - 1] = '\0';
   CHEEVOS_LOG(RCHEEVOS_TAG "%s\n", title);

   if (settings->bools.cheevos_visibility_mastery)
   {
#if defined (HAVE_GFX_WIDGETS)
      if (gfx_widgets_ready())
      {
         const bool content_runtime_log      = settings->bools.content_runtime_log;
         const bool content_runtime_log_aggr = settings->bools.content_runtime_log_aggregate;
         char msg[128];
         size_t len = strlcpy(msg, rcheevos_locals.displayname, sizeof(msg));

         if (len < sizeof(msg) - 12 &&
            (content_runtime_log || content_runtime_log_aggr))
         {
            const char* content_path   = path_get(RARCH_PATH_CONTENT);
            const char* core_path      = path_get(RARCH_PATH_CORE);
            runtime_log_t* runtime_log = runtime_log_init(
               content_path, core_path,
               settings->paths.directory_runtime_log,
               settings->paths.directory_playlist,
               !content_runtime_log_aggr);

            if (runtime_log)
            {
               const runloop_state_t* runloop_state = runloop_state_get_ptr();
               runtime_log_add_runtime_usec(runtime_log,
                  runloop_state->core_runtime_usec);

               len += snprintf(msg + len, sizeof(msg) - len, " | ");
               runtime_log_get_runtime_str(runtime_log, msg + len, sizeof(msg) - len);
               msg[sizeof(msg) - 1] = '\0';

               free(runtime_log);
            }
         }

         gfx_widgets_push_achievement(title, msg, rcheevos_locals.game.badge_name);
      }
      else
#endif
         runloop_msg_queue_push(title, 0, 3 * 60, false, NULL, MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
   }
}

static void rcheevos_show_game_placard(void)
{
   char msg[256];
   const settings_t* settings        = config_get_ptr();
   const rcheevos_racheevo_t* cheevo = rcheevos_locals.game.achievements;
   const rcheevos_racheevo_t* end    = cheevo 
      + rcheevos_locals.game.achievement_count;
   int number_of_active              = 0;
   int number_of_unsupported         = 0;
   int number_of_core                = 0;
   int mode                          = RCHEEVOS_ACTIVE_SOFTCORE;

   if (rcheevos_locals.hardcore_active)
      mode = RCHEEVOS_ACTIVE_HARDCORE;

   for (; cheevo < end; cheevo++)
   {
      if (cheevo->active & RCHEEVOS_ACTIVE_UNOFFICIAL)
         continue;

      number_of_core++;
      if (cheevo->active & RCHEEVOS_ACTIVE_UNSUPPORTED)
         number_of_unsupported++;
      else if (cheevo->active & mode)
         number_of_active++;
   }

   /* TODO/FIXME - localize strings */
   if (number_of_core == 0)
      strlcpy(msg, "This game has no achievements.", sizeof(msg));
   else if (!number_of_unsupported)
   {
      if (settings->bools.cheevos_start_active)
         snprintf(msg, sizeof(msg),
            "All %d achievements activated for this session.",
            number_of_core);
      else
         snprintf(msg, sizeof(msg),
            "You have %d of %d achievements unlocked.",
            number_of_core - number_of_active, number_of_core);
   }
   else
   {
      if (settings->bools.cheevos_start_active)
         snprintf(msg, sizeof(msg),
            "All %d achievements activated for this session (%d unsupported).",
            number_of_core, number_of_unsupported);
      else
         snprintf(msg, sizeof(msg),
            "You have %d of %d achievements unlocked (%d unsupported).",
            number_of_core - number_of_active - number_of_unsupported,
            number_of_core, number_of_unsupported);
   }

   msg[sizeof(msg) - 1] = 0;
   CHEEVOS_LOG(RCHEEVOS_TAG "%s\n", msg);

   if (settings->uints.cheevos_visibility_summary == RCHEEVOS_SUMMARY_ALLGAMES ||
       (number_of_core > 0 && settings->uints.cheevos_visibility_summary == RCHEEVOS_SUMMARY_HASCHEEVOS))
   {
#if defined (HAVE_GFX_WIDGETS)
      if (gfx_widgets_ready())
         gfx_widgets_push_achievement(rcheevos_locals.game.title, msg, rcheevos_locals.game.badge_name);
      else
#endif
         runloop_msg_queue_push(msg, 0, 3 * 60, false, NULL, MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
   }
}

static void rcheevos_end_load(void)
{
   CHEEVOS_LOG(RCHEEVOS_TAG "Load finished\n");
   rcheevos_locals.load_info.state = RCHEEVOS_LOAD_STATE_DONE;
}

static void rcheevos_fetch_badges_callback(void* userdata)
{
   rcheevos_end_load();
}

static void rcheevos_fetch_badges(void)
{
   /* this function manages the 
    * RCHEEVOS_LOAD_STATE_FETCHING_BADGES state */
   rcheevos_client_fetch_badges(rcheevos_fetch_badges_callback, NULL);
}

static void rcheevos_start_session_async(retro_task_t* task)
{
   const bool needs_runtime =
      (  rcheevos_locals.game.achievement_count > 0
      || rcheevos_locals.game.leaderboard_count > 0
      || rcheevos_locals.runtime.richpresence);

   if (rcheevos_load_aborted())
      return;

   /* We don't have to wait for this to complete
    * to proceed to the next loading state */
   rcheevos_client_start_session(rcheevos_locals.game.id);

   rcheevos_begin_load_state(RCHEEVOS_LOAD_STATE_STARTING_SESSION);

   if (needs_runtime)
   {
      /* activate the achievements and leaderboards
       * (rich presence has already been activated) */
      rcheevos_activate_achievements();

      if (rcheevos_locals.leaderboards_enabled
         && rcheevos_locals.hardcore_active)
         rcheevos_activate_leaderboards();

      /* disable any unsupported achievements */
      rcheevos_validate_memrefs(&rcheevos_locals);

      /* Let the runtime start processing the achievements */
      rcheevos_locals.loaded = true;
   }

#if HAVE_REWIND
   if (!rcheevos_locals.hardcore_active)
   {
      /* Re-enable rewind. If rcheevos_locals.loaded is true,
       * additional space will be allocated for the achievement
       * state data */
      const settings_t* settings = config_get_ptr();
      if (settings->bools.rewind_enable)
      {
#ifdef HAVE_THREADS
         if (!task_is_on_main_thread())
         {
            /* Have to "schedule" this. CMD_EVENT_REWIND_INIT should
             * only be called on the main thread */
            rcheevos_locals.queued_command = CMD_EVENT_REWIND_INIT;
         }
         else
#endif
            command_event(CMD_EVENT_REWIND_INIT, NULL);
      }
   }
#endif

   if (!needs_runtime)
   {
      /* if there's nothing for the runtime to process,
       * disable hardcore. */
      rcheevos_pause_hardcore();
   }
   else if (rcheevos_locals.hardcore_active)
   {
      /* hardcore is active. we're going to start processing
       * achievements. make sure restrictions are enforced */
      rcheevos_enforce_hardcore_settings();
   }

   task_set_finished(task, true);

   if (rcheevos_end_load_state() == 0)
      rcheevos_fetch_badges();
}

static void rcheevos_start_session_finish(retro_task_t* task, void* data, void* userdata, const char* error)
{
   (void)task;
   (void)data;
   (void)userdata;
   (void)error;

   /* this must be called on the main thread */
   rcheevos_show_game_placard();
}

static void rcheevos_start_session(void)
{
   retro_task_t* task;

   if (rcheevos_load_aborted())
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Load aborted before starting session\n");
      return;
   }

   /* re-validate the config settings now that we know
    * which console_id is active */
   rcheevos_validate_config_settings();

   task           = task_init();
   task->handler  = rcheevos_start_session_async;
   task->callback = rcheevos_start_session_finish;
   task_queue_push(task);
}

static void rcheevos_initialize_runtime_callback(void* userdata)
{
   rcheevos_start_session();
}

static void rcheevos_fetch_game_data(void)
{
   if (     rcheevos_locals.load_info.state 
         == RCHEEVOS_LOAD_STATE_NETWORK_ERROR)
   {
      rcheevos_locals.game.hash = NULL;
      rcheevos_pause_hardcore();
      return;
   }

   if (rcheevos_locals.game.id <= 0)
   {
      const settings_t* settings = config_get_ptr();
      if (settings->bools.cheevos_verbose_enable)
         runloop_msg_queue_push(
            "RetroAchievements: Game could not be identified.",
            0, 3 * 60, false, NULL, MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);

      CHEEVOS_LOG(RCHEEVOS_TAG "Game could not be identified\n");
      if (rcheevos_locals.load_info.hashes_tried > 1)
         rcheevos_locals.game.hash = NULL;

      rcheevos_locals.load_info.state = RCHEEVOS_LOAD_STATE_UNKNOWN_GAME;
      rcheevos_pause_hardcore();
      return;
   }

   if (!rcheevos_locals.token[0])
   {
      rcheevos_locals.load_info.state = RCHEEVOS_LOAD_STATE_LOGIN_FAILED;
      rcheevos_pause_hardcore();
      return;
   }

   /* fetch the game data and the user unlocks */
   rcheevos_begin_load_state(RCHEEVOS_LOAD_STATE_FETCHING_GAME_DATA);

#if HAVE_REWIND
   if (!rcheevos_locals.hardcore_active)
   {
      /* deactivate rewind while we activate the achievements */
      const settings_t* settings = config_get_ptr();
      if (settings->bools.rewind_enable)
      {
#ifdef HAVE_THREADS
         if (!task_is_on_main_thread())
         {
            /* have to "schedule" this. CMD_EVENT_REWIND_DEINIT should only be called on the main thread */
            rcheevos_locals.queued_command = CMD_EVENT_REWIND_DEINIT;

            /* wait for rewind to be disabled */
            while (rcheevos_locals.queued_command != CMD_EVENT_NONE)
               retro_sleep(1);
         }
         else
#endif
            command_event(CMD_EVENT_REWIND_DEINIT, NULL);
      }
   }
#endif

   rcheevos_client_initialize_runtime(rcheevos_locals.game.id, rcheevos_initialize_runtime_callback, NULL);

   if (rcheevos_end_load_state() == 0)
      rcheevos_start_session();
}

struct rcheevos_identify_game_data
{
   struct rc_hash_iterator iterator;
   char* path;
   uint8_t* datacopy;
   char hash[33];
};

static void rcheevos_identify_game_callback(void* userdata)
{
   struct rcheevos_identify_game_data* data = 
      (struct rcheevos_identify_game_data*)userdata;

   rcheevos_locals.load_info.hashes_tried++;

   if (rcheevos_locals.game.id == 0)
   {
      /* previous hash didn't match, try the next one */
      char new_hash[33];
      int found_new_hash;
      while ((found_new_hash = rc_hash_iterate(new_hash, &data->iterator)) != 0)
      {
         if (!rc_libretro_hash_set_get_game_id(&rcheevos_locals.game.hashes, new_hash))
            break;

         CHEEVOS_LOG(RCHEEVOS_TAG "Ignoring [%s]. Already tried.\n", new_hash);
      }

      if (found_new_hash)
      {
         memcpy(data->hash, new_hash, sizeof(data->hash));
         rcheevos_client_identify_game(data->hash,
               rcheevos_identify_game_callback, data);
         return;
      }
   }

   rc_libretro_hash_set_add(&rcheevos_locals.game.hashes,
      data->path, rcheevos_locals.game.id, data->hash);
   rcheevos_locals.game.hash =
      rc_libretro_hash_set_get_hash(&rcheevos_locals.game.hashes, data->path);

   if (data->iterator.path && strcmp(data->iterator.path, data->path) != 0)
   {
      rc_libretro_hash_set_add(&rcheevos_locals.game.hashes,
         data->iterator.path, rcheevos_locals.game.id, data->hash);
      rcheevos_locals.game.hash =
         rc_libretro_hash_set_get_hash(&rcheevos_locals.game.hashes, data->iterator.path);
   }

   /* no more hashes generated, free the iterator data */
   rc_hash_destroy_iterator(&data->iterator);
   if (data->datacopy)
      free(data->datacopy);
   if (data->path)
      free(data->path);
   free(data);

   /* hash resolution complete, proceed to fetching game data */
   if (rcheevos_end_load_state() == 0)
      rcheevos_fetch_game_data();
}

static int rcheevos_get_image_path(unsigned index, char* buffer, size_t buffer_size)
{
   rarch_system_info_t* system = &runloop_state_get_ptr()->system;
   if (!system->disk_control.cb.get_image_path)
      return 0;

   return system->disk_control.cb.get_image_path(index, buffer, buffer_size);
}

static bool rcheevos_identify_game(const struct retro_game_info* info)
{
   struct rcheevos_identify_game_data* data;
   struct rc_hash_filereader filereader;
   size_t len;
#ifndef DEBUG
   settings_t* settings = config_get_ptr();
#endif

   data = (struct rcheevos_identify_game_data*)
         calloc(1, sizeof(struct rcheevos_identify_game_data));
   if (!data)
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "allocation failed\n");
      return false;
   }

   /* provide hooks for reading files */
   memset(&filereader, 0, sizeof(filereader));
   filereader.open = rc_hash_handle_file_open;
   filereader.seek = rc_hash_handle_file_seek;
   filereader.tell = rc_hash_handle_file_tell;
   filereader.read = rc_hash_handle_file_read;
   filereader.close = rc_hash_handle_file_close;
   rc_hash_init_custom_filereader(&filereader);

   rc_hash_init_error_message_callback(rcheevos_handle_log_message);

#ifndef DEBUG
   /* in DEBUG mode, always initialize the verbose message handler */
   if (settings->bools.cheevos_verbose_enable)
#endif
   {
      rc_hash_init_verbose_message_callback(rcheevos_handle_log_message);
   }

   rc_hash_reset_cdreader_hooks();

   /* fetch the first hash */
   rc_hash_initialize_iterator(&data->iterator,
         info->path, (uint8_t*)info->data, info->size);
   if (!rc_hash_iterate(data->hash, &data->iterator))
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "no hashes generated\n");
      rc_hash_destroy_iterator(&data->iterator);
      free(data);
      return false;
   }

   rc_libretro_hash_set_init(&rcheevos_locals.game.hashes, info->path, rcheevos_get_image_path);
   data->path = strdup(info->path);

   if (data->iterator.consoles[data->iterator.index] != 0)
   {
      /* multiple potential matches, clone the data for the next attempt */
      if (info->data)
      {
         len = info->size;
         if (len > CHEEVOS_MB(64))
            len = CHEEVOS_MB(64);

         data->datacopy = (uint8_t*)malloc(len);
         if (!data->datacopy)
         {
            CHEEVOS_LOG(RCHEEVOS_TAG "allocation failed\n");
            rc_hash_destroy_iterator(&data->iterator);
            free(data);
            return false;
         }

         memcpy(data->datacopy, info->data, len);
         data->iterator.buffer = data->datacopy;
      }
   }

   rcheevos_begin_load_state(RCHEEVOS_LOAD_STATE_IDENTIFYING_GAME);
   rcheevos_client_identify_game(data->hash,
         rcheevos_identify_game_callback, data);
   return true;
}

static void rcheevos_login_callback(void* userdata)
{
   if (rcheevos_locals.token[0])
   {
      const settings_t* settings = config_get_ptr();
      if (settings->bools.cheevos_visibility_account)
      {
         char msg[256];
         msg[0] = '\0';
         /* TODO/FIXME - localize */
         snprintf(msg, sizeof(msg),
            "RetroAchievements: Logged in as \"%s\".",
            rcheevos_locals.displayname);
         runloop_msg_queue_push(msg, 0, 2 * 60, false, NULL,
               MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_INFO);
      }
   }

   if (rcheevos_end_load_state() == 0)
      rcheevos_fetch_game_data();
}

/* Increment the outstanding requests counter and set the load state */
void rcheevos_begin_load_state(enum rcheevos_load_state state)
{
#ifdef HAVE_THREADS
   slock_lock(rcheevos_locals.load_info.request_lock);
#endif
   ++rcheevos_locals.load_info.outstanding_requests;
   rcheevos_locals.load_info.state = state;
#ifdef HAVE_THREADS
   slock_unlock(rcheevos_locals.load_info.request_lock);
#endif
}

/* Decrement and return the outstanding requests counter. 
 * If non-zero, requests are still outstanding */
int rcheevos_end_load_state(void)
{
   int requests = 0;

#ifdef HAVE_THREADS
   slock_lock(rcheevos_locals.load_info.request_lock);
#endif
   if (rcheevos_locals.load_info.outstanding_requests > 0)
      --rcheevos_locals.load_info.outstanding_requests;
   requests = rcheevos_locals.load_info.outstanding_requests;
#ifdef HAVE_THREADS
   slock_unlock(rcheevos_locals.load_info.request_lock);
#endif

   return requests;
}

bool rcheevos_load_aborted(void)
{
   switch (rcheevos_locals.load_info.state)
   {
      /* Unload has been called */
      case RCHEEVOS_LOAD_STATE_ABORTED:
      /* Unload quit waiting and ran to completion */      
      case RCHEEVOS_LOAD_STATE_NONE:
      /* Login/resolve hash failed after several attempts */
      case RCHEEVOS_LOAD_STATE_NETWORK_ERROR: 
         return true;
      default:
         break;
   }
   return false;
}

bool rcheevos_load(const void *data)
{
   const struct retro_game_info *info = (const struct retro_game_info*)
      data;
   settings_t *settings               = config_get_ptr();
   bool cheevos_enable                = settings 
      && settings->bools.cheevos_enable;

   memset(&rcheevos_locals.load_info, 0,
         sizeof(rcheevos_locals.load_info));

   rcheevos_locals.loaded             = false;
   rcheevos_locals.game.id            = -1;
   rcheevos_locals.game.console_id    = 0;
   rcheevos_locals.game.mastery_placard_shown = false;
#ifdef HAVE_THREADS
   rcheevos_locals.queued_command     = CMD_EVENT_NONE;
#endif
   rc_runtime_init(&rcheevos_locals.runtime);

   /* If achievements are not enabled, or the core doesn't 
    * support achievements, disable hardcore and bail */
   if (!cheevos_enable || !rcheevos_locals.core_supports || !data)
   {
      rcheevos_locals.game.id = 0;
      rcheevos_pause_hardcore();
      return false;
   }

   if (string_is_empty(settings->arrays.cheevos_username))
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Cannot login (no username)\n");
      runloop_msg_queue_push("Missing RetroAchievements account information.", 0, 5 * 60, false, NULL,
         MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_ERROR);
      rcheevos_locals.game.id = 0;
      rcheevos_pause_hardcore();
      return false;
   }

#ifdef HAVE_THREADS
   if (!rcheevos_locals.load_info.request_lock)
      rcheevos_locals.load_info.request_lock = slock_new();
#endif
   rcheevos_begin_load_state(RCHEEVOS_LOAD_STATE_IDENTIFYING_GAME);

   /* reset hardcore mode and leaderboard settings based on configs */
   rcheevos_hardcore_enabled_changed();
   CHEEVOS_LOG(RCHEEVOS_TAG "Load started, hardcore %sactive\n", rcheevos_hardcore_active() ? "" : "not ");

   rcheevos_validate_config_settings();
   rcheevos_leaderboards_enabled_changed();

   /* Refresh the user agent in case it's not set or has changed */
   rcheevos_client_initialize();
   rcheevos_get_user_agent(&rcheevos_locals,
      rcheevos_locals.user_agent_core,
      sizeof(rcheevos_locals.user_agent_core));

   /* === ACHIEVEMENT INITIALIZATION PROCESS ===

      1. RCHEEVOS_LOAD_STATE_IDENTIFYING_GAME
         a. iterate possible hashes to identify game [rcheevos_identify_game]
            i. if game not found, display "no achievements for this game" and abort [rcheevos_identify_game_callback]
         b. Login
            i. if already logged in, skip this step
            ii. start login request [rcheevos_client_login_with_password/rcheevos_client_login_with_token]
            iii. complete login, store user/token [rcheevos_login_callback]
      2. RCHEEVOS_LOAD_STATE_FETCHING_GAME_DATA [rcheevos_client_initialize_runtime]
         a. begin game data request [rc_api_init_fetch_game_data_request]
         b. fetch user unlocks
            i. if encore mode, skip this step
            ii. begin user unlocks hardcore request [rc_api_init_fetch_user_unlocks_request]
            iii. begin user unlocks softcore request [rc_api_init_fetch_user_unlocks_request]
      3. RCHEEVOS_LOAD_STATE_STARTING_SESSION [rcheevos_initialize_runtime_callback]
         a. activate achievements [rcheevos_activate_achievements]
         b. schedule rich presence periodic update [rcheevos_client_start_session]
         c. start session on server [rcheevos_client_start_session]
         d. show title card [rcheevos_show_game_placard]
      4. RCHEEVOS_LOAD_STATE_FETCHING_BADGES
         a. download from server [rcheevos_client_fetch_badges]
      5. RCHEEVOS_LOAD_STATE_DONE

    */

   /* Identify the game and log the user in. 
    * These will run asynchronously. */
   if (!rcheevos_identify_game(info))
   {
      /* No hashes could be generated for the game, 
       * disable hardcore and bail */
      rcheevos_locals.game.id = 0;
      rcheevos_end_load_state();
      rcheevos_pause_hardcore();
      return false;
   }

   if (!rcheevos_locals.token[0])
   {
      rcheevos_begin_load_state(RCHEEVOS_LOAD_STATE_IDENTIFYING_GAME);
      if (!string_is_empty(settings->arrays.cheevos_token))
      {
         CHEEVOS_LOG(RCHEEVOS_TAG "Attempting to login %s (with token)\n",
               settings->arrays.cheevos_username);
         rcheevos_client_login_with_token(
               settings->arrays.cheevos_username,
               settings->arrays.cheevos_token,
               rcheevos_login_callback, NULL);
      }
      else if (!string_is_empty(settings->arrays.cheevos_password))
      {
         CHEEVOS_LOG(RCHEEVOS_TAG "Attempting to login %s (with password)\n",
               settings->arrays.cheevos_username);
         rcheevos_client_login_with_password(
               settings->arrays.cheevos_username,
               settings->arrays.cheevos_password,
               rcheevos_login_callback, NULL);
      }
      else
      {
         CHEEVOS_LOG(RCHEEVOS_TAG "Cannot login %s (no password or token)\n",
               settings->arrays.cheevos_username);
         runloop_msg_queue_push("No password provided for RetroAchievements account", 0, 5 * 60, false, NULL,
               MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_ERROR);
         rcheevos_unload();
         return false;
      }
   }

   if (rcheevos_end_load_state() == 0)
      rcheevos_fetch_game_data();

   return true;
}

struct rcheevos_identify_changed_disc_data
{
   int real_game_id;
   char* path;
   char hash[33];
};

static void rcheevos_identify_game_disc_callback(void* userdata)
{
   struct rcheevos_identify_changed_disc_data* changed_disc_data =
      (struct rcheevos_identify_changed_disc_data*)userdata;

   /* rcheevos_locals.game.id has the game id for the new hash, swap it with the old game id */
   const int hash_game_id = rcheevos_locals.game.id;
   rcheevos_locals.game.id = changed_disc_data->real_game_id;

   /* rcheevos_client_identify_game will update rcheevos_locals.game.id */
   if (rcheevos_locals.game.id == hash_game_id)
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Hash valid for current game\n");
   }
   else if (hash_game_id != 0)
   {
      /* when changing discs, if the disc is recognized but belongs to another game, allow it.
       * this allows loading known game discs for games that leverage user-provided discs. */
      CHEEVOS_LOG(RCHEEVOS_TAG "Hash identified for game %d\n", hash_game_id);
   }
   else
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Disc not recognized\n");
      if (rcheevos_hardcore_active())
      {
         /* don't allow unknown game discs in hardcore.
            * assume it's a modified version of the base game. */
         runloop_msg_queue_push("Hardcore paused. Game disc unrecognized.", 0, 5 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_ERROR);
         rcheevos_pause_hardcore();
      }
   }

   /* disc is valid, add it to the known disk list */
   rc_libretro_hash_set_add(&rcheevos_locals.game.hashes,
      changed_disc_data->path, hash_game_id, changed_disc_data->hash);

   rcheevos_locals.game.hash =
      rc_libretro_hash_set_get_hash(&rcheevos_locals.game.hashes, changed_disc_data->hash);

   free(changed_disc_data->path);
   free(changed_disc_data);
}

static void rcheevos_identify_initial_disc_callback(void* userdata)
{
   struct rcheevos_identify_changed_disc_data* changed_disc_data =
      (struct rcheevos_identify_changed_disc_data*)userdata;

   /* rcheevos_client_identify_game will update rcheevos_locals.game.id */
   if (rcheevos_locals.game.id != changed_disc_data->real_game_id)
   {
      if (rcheevos_locals.game.id == 0)
      {
         CHEEVOS_LOG(RCHEEVOS_TAG "Disc not recognized\n");
         runloop_msg_queue_push("Disabling achievements. Game disc unrecognized.", 0, 5 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_ERROR);
      }
      else
      {
         CHEEVOS_LOG(RCHEEVOS_TAG "Initial disc for game %d\n", rcheevos_locals.game.id);
         runloop_msg_queue_push("Disabling achievements. Not for loaded game.", 0, 5 * 60, false, NULL,
            MESSAGE_QUEUE_ICON_DEFAULT, MESSAGE_QUEUE_CATEGORY_ERROR);
      }

      rcheevos_locals.game.hash = NULL;
      rcheevos_unload();
   }
   else
   {
      /* disc is valid, add it to the known disk list */
      CHEEVOS_LOG(RCHEEVOS_TAG "Hash valid for current game\n");
      rc_libretro_hash_set_add(&rcheevos_locals.game.hashes,
         changed_disc_data->path, rcheevos_locals.game.id, changed_disc_data->hash);

      rcheevos_locals.game.hash =
         rc_libretro_hash_set_get_hash(&rcheevos_locals.game.hashes, changed_disc_data->hash);
   }

   free(changed_disc_data->path);
   free(changed_disc_data);
}

static void rcheevos_validate_initial_disc_handler(retro_task_t* task)
{
   char* new_disc_path = (char*)task->user_data;

   if (rcheevos_locals.game.id == 0)
   {
      /* could not identify game. don't bother identifying initial disc */
   }
   else
   {
      if (rcheevos_locals.game.console_id == 0)
      {
         /* not ready yet. try again in another 500ms */
         task->when = cpu_features_get_time_usec() + 500 * 1000;
         return;
      }

      /* game ready. attempt to validate the initial disc */
      rcheevos_change_disc(new_disc_path, true);
   }

   free(new_disc_path);
   task_set_finished(task, true);
}

void rcheevos_change_disc(const char* new_disc_path, bool initial_disc)
{
   struct rcheevos_identify_changed_disc_data* data;
   char hash[33];
   int hash_game_id;

   /* no game loaded */
   if (rcheevos_locals.game.id == 0)
      return;

   /* see if we've already identified this file */
   rcheevos_locals.game.hash =
      rc_libretro_hash_set_get_hash(&rcheevos_locals.game.hashes, new_disc_path);
   if (rcheevos_locals.game.hash)
   {
      CHEEVOS_LOG(RCHEEVOS_TAG "Switched to known hash: %s\n", rcheevos_locals.game.hash);
      return;
   }

   /* don't check the disc until the game is done loading */
   if (rcheevos_locals.game.console_id == 0)
   {
      retro_task_t* task = task_init();
      task->handler = rcheevos_validate_initial_disc_handler;
      task->user_data = strdup(new_disc_path);
      task->progress = -1;
      task->when = cpu_features_get_time_usec() + 500 * 1000; /* 500ms */
      task_queue_push(task);
      return;
   }

   /* attempt to identify the file */
   if (rc_hash_generate_from_file(hash, rcheevos_locals.game.console_id, new_disc_path))
   {
      /* check to see if the hash is already known */
      hash_game_id = rc_libretro_hash_set_get_game_id(&rcheevos_locals.game.hashes, hash);
      if (hash_game_id)
      {
         /* hash identical to some other file - probably the first disc matching the m3u. */
         CHEEVOS_LOG(RCHEEVOS_TAG "Hash valid for current game\n");
      }
   }
   else
   {
      /* when changing discs, if the disc is not supported by the system, allow it. this is
       * primarily for games that support user-provided audio CDs, but does allow using discs
       * from other systems for games that leverage user-provided discs. */
      CHEEVOS_LOG(RCHEEVOS_TAG "No hash generated\n");
      hash_game_id = -1;
      strlcpy(hash, "[NO HASH]", sizeof(hash));
   }

   if (hash_game_id)
   {
      /* we know how to handle this disc. no need to call the server */
      rc_libretro_hash_set_add(&rcheevos_locals.game.hashes, new_disc_path, hash_game_id, hash);
      rcheevos_locals.game.hash =
         rc_libretro_hash_set_get_hash(&rcheevos_locals.game.hashes, new_disc_path);
      return;
   }

   /* call the server to make sure the hash is valid for the loaded game */
   data = (struct rcheevos_identify_changed_disc_data*)
      calloc(1, sizeof(struct rcheevos_identify_changed_disc_data));
   if (data) {
      data->real_game_id = rcheevos_locals.game.id;
      data->path = strdup(new_disc_path);
      memcpy(data->hash, hash, sizeof(data->hash));

      rcheevos_client_identify_game(data->hash,
         initial_disc ? rcheevos_identify_initial_disc_callback :
            rcheevos_identify_game_disc_callback, data);
   }
}
