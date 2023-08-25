/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
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

#ifndef _MENU_DIALOG_H
#define _MENU_DIALOG_H

#include <stdint.h>
#include <stdlib.h>

#include <boolean.h>

#include <retro_common_api.h>

#include "../msg_hash.h"

enum menu_dialog_type
{
   MENU_DIALOG_NONE = 0,
   MENU_DIALOG_WELCOME,
   MENU_DIALOG_HELP_EXTRACT,
   MENU_DIALOG_HELP_CONTROLS,
   MENU_DIALOG_HELP_CHEEVOS_DESCRIPTION,
   MENU_DIALOG_HELP_LOADING_CONTENT,
   MENU_DIALOG_HELP_WHAT_IS_A_CORE,
   MENU_DIALOG_HELP_CHANGE_VIRTUAL_GAMEPAD,
   MENU_DIALOG_HELP_AUDIO_VIDEO_TROUBLESHOOTING,
   MENU_DIALOG_HELP_SEND_DEBUG_INFO,
   MENU_DIALOG_HELP_SCANNING_CONTENT,
   MENU_DIALOG_QUIT_CONFIRM,
   MENU_DIALOG_INFORMATION,
   MENU_DIALOG_QUESTION,
   MENU_DIALOG_WARNING,
   MENU_DIALOG_ERROR,
   MENU_DIALOG_LAST
};

struct menu_dialog
{
   unsigned              current_id;
   enum menu_dialog_type current_type;
   bool                  pending_push;
};

typedef struct menu_dialog menu_dialog_t;

RETRO_BEGIN_DECLS

void menu_dialog_push_pending(enum menu_dialog_type type);

void menu_dialog_unset_pending_push(void);

void menu_dialog_set_current_id(unsigned id);

RETRO_END_DECLS

#endif
