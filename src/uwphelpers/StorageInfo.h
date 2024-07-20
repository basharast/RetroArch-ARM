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

#include "pch.h"

#include <string>
#include <vector>
#include <cstdio>
#include <inttypes.h>

struct ItemInfoUWP {
	std::string name;
	std::string fullName;

	bool isDirectory = false;

	uint64_t size = 0;
	uint64_t lastAccessTime = 0;
	uint64_t lastWriteTime = 0;
	uint64_t changeTime = 0;
	uint64_t creationTime = 0;

	DWORD attributes = 0;
};
