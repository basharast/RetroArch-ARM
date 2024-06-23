/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
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

#include <stdint.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#include <orbis/libScePad.h>
#include <defines/ps4_defines.h>

#include <boolean.h>
#include <libretro.h>
#include <retro_miscellaneous.h>

#include "../input_driver.h"

typedef struct ps4_input
{
   const input_device_driver_t *joypad;
} ps4_input_t;

int16_t ps4_input_state(void *data,
         const input_device_driver_t *joypad_data,
         const input_device_driver_t *sec_joypad_data,
         rarch_joypad_info_t *joypad_info,
         const retro_keybind_set *retro_keybinds,
         bool keyboard_mapping_blocked,
         unsigned port, unsigned device, unsigned index, unsigned id)
{
   ps4_input_t *ps4           = (ps4_input_t*)data;

   switch (device)
   {
      case RETRO_DEVICE_JOYPAD:
         if (id == RETRO_DEVICE_ID_JOYPAD_MASK)
         {
            unsigned i;
            int16_t ret = 0;
            for (i = 0; i < RARCH_FIRST_CUSTOM_BIND; i++)
            {
               /* Auto-binds are per joypad, not per user. */
               const uint64_t joykey  = (retro_keybinds[port][i].joykey != NO_BTN)
                  ? retro_keybinds[port][i].joykey : joypad_info->auto_binds[i].joykey;
               const uint32_t joyaxis = (retro_keybinds[port][i].joyaxis != AXIS_NONE)
                  ? retro_keybinds[port][i].joyaxis : joypad_info->auto_binds[i].joyaxis;

               if ((uint16_t)joykey != NO_BTN && ps4->joypad->button(
                        joypad_info->joy_idx, (uint16_t)joykey))
               {
                  ret |= (1 << i);
                  continue;
               }
               if (((float)abs(ps4->joypad->axis(joypad_info->joy_idx, joyaxis)) / 0x8000) > joypad_info->axis_threshold)
               {
                  ret |= (1 << i);
                  continue;
               }
            }

            return ret;
         }
         else
         {
            /* Auto-binds are per joypad, not per user. */
            const uint64_t joykey  = (retro_keybinds[port][id].joykey != NO_BTN)
               ? retro_keybinds[port][id].joykey : joypad_info->auto_binds[id].joykey;
            const uint32_t joyaxis = (retro_keybinds[port][id].joyaxis != AXIS_NONE)
               ? retro_keybinds[port][id].joyaxis : joypad_info->auto_binds[id].joyaxis;

            if ((uint16_t)joykey != NO_BTN && ps4->joypad->button(
                     joypad_info->joy_idx, (uint16_t)joykey))
               return true;
            if (((float)abs(ps4->joypad->axis(joypad_info->joy_idx, joyaxis)) / 0x8000) > joypad_info->axis_threshold)
               return true;
         }
         break;
      case RETRO_DEVICE_ANALOG:
#if 0
         if (retro_keybinds[port])
            return input_joypad_analog(ps4->joypad, joypad_info, port, idx, id, retro_keybinds[port]);
#endif
         break;
   }

   return 0;
}
static void ps4_input_free_input(void *data) 
{
   ps4_input_t *ps4 = (ps4_input_t*)data;

   if (ps4 && ps4->joypad)
      ps4->joypad->destroy();

   free(data); 

}
static void* ps4_input_initialize(const char *joypad_driver) 
{
   ps4_input_t *ps4 = (ps4_input_t*)calloc(1, sizeof(*ps4));
   if (!ps4)
      return NULL;

   ps4->joypad = input_joypad_init_driver(joypad_driver, ps4);
   return ps4; 
}
static void ps4_input_poll(void *data)
{
   ps4_input_t *ps4 = (ps4_input_t*)data;

   if (ps4 && ps4->joypad)
      ps4->joypad->poll();
}

static uint64_t ps4_input_get_capabilities(void *data)
{
   return   (1 << RETRO_DEVICE_JOYPAD) 
          | (1 << RETRO_DEVICE_ANALOG);
}

input_driver_t input_ps4 = {
   ps4_input_initialize,
   ps4_input_poll,                         /* poll */
   ps4_input_state,                         /* input_state */
   ps4_input_free_input,
   NULL,
   NULL,
   ps4_input_get_capabilities,
   "ps4",
   NULL,                         /* grab_mouse */
   NULL,
   NULL
};
