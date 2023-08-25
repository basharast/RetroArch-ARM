/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2017 - Higor Euripedes
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
#ifndef TASK_POWER_STATE_H
#define TASK_POWER_STATE_H

#include <stdint.h>

#include <boolean.h>
#include <retro_common_api.h>
#include <retro_miscellaneous.h>

#include "../frontend/frontend_driver.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

RETRO_BEGIN_DECLS

void task_push_get_powerstate(void);

enum frontend_powerstate get_last_powerstate(int *percent);

RETRO_END_DECLS

#endif
