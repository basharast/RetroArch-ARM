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

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <cctype>

typedef struct {
   DWORD dwDesiredAccess;
   DWORD dwShareMode;
   DWORD dwCreationDisposition;
   int flags;
   bool isWrite;
   bool isAppend;
   bool isCreate;
} FILE_OPEN_UWP_MODE;

bool replace(std::string& str, const std::string& from, const std::string& to);
std::string replace2(const std::string str, const std::string& from, const std::string& to);
std::vector<std::string> split(const std::string s, char seperator);
// Parent full path, child full path
bool isChild(std::string parent, std::string child);
// Parent full path, child full path, child name only
bool isParent(std::string parent, std::string child, std::string childName);

bool iequals(const std::string a, const std::string b);
bool equals(const std::string a, const std::string b);
bool ends_with(std::string const& value, std::string const& ending);
bool starts_with(std::string str, std::string prefix);

Platform::String^ convert(const std::string input);
std::wstring convertToWString(const std::string input);
std::string convert(Platform::String^ input);
std::string convert(std::wstring input);
std::string convert(const char* input);
LPCWSTR convertToLPCWSTR(std::string input);
LPCWSTR convertToLPCWSTR(Platform::String^ input);
const char* convertToChar(Platform::String^ input);

void tolower(std::string& input);
void tolower(Platform::String^& input);
void toupper(std::string& input);
void toupper(Platform::String^& input);

void windowsPath(std::string& path);
void windowsPath(Platform::String^& path);

std::string merge(std::string targetFullPath, std::string subFullPath);

std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& trim(std::string& s, const char* t = " \t\n\r\f\v");

template<typename T>
bool findInList(std::list<T>& inputList, T& str) {
	return (std::find(inputList.begin(), inputList.end(), str) != inputList.end());
};

FILE_OPEN_UWP_MODE* GetFileMode(const char* mode);

// Parent and child full path
std::string getSubRoot(std::string parent, std::string child);

