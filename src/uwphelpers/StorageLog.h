/*  RetroArch - A frontend for libretro.
 *
 *  Copyright (C) 2023-2024 - Bashar Astifan
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

#pragma once

#include "../verbosity.h"
#include "../libretro-common/include/queues/task_queue.h"

#define UWPSMT = 4; // Change it based on your needs

// Link your debug function below
#define UWP_ERROR_LOG(t,...)   do {  } while (false)
#define UWP_WARN_LOG(t,...)    do {  } while (false)
#define UWP_NOTICE_LOG(t,...)  do {  } while (false)
#define UWP_INFO_LOG(t,...)    do {  } while (false)
#define UWP_DEBUG_LOG(t,...)   do {  } while (false)
#define UWP_VERBOSE_LOG(t,...) do {  } while (false)
