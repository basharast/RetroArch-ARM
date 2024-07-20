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

#include <string>

// Known locations
// ACTIVATE BELOW ONLY IF YOU ADDED THE CAPABILITY
#define APPEND_APP_LOCALDATA_LOCATION 1 // recommended to be always 1
#define APPEND_APP_INSTALLATION_LOCATION 1 // recommended to be always 1
#define APPEND_DOCUMENTS_LOCATION 0 // (requires 'documentsLibrary' capability)
#define APPEND_VIDEOS_LOCATION 0 // (requires 'videosLibrary' capability)
#define APPEND_MUSIC_LOCATION 0 // (requires musicLibrary' capability)
#define APPEND_PICTURES_LOCATION 0 // (requires 'picturesLibrary' capability)


// Working folder
// set this value by calling `SetWorkingFolder` from `StorageManager.h`
static std::string AppWorkingFolder;
