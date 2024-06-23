// UWP STORAGE MANAGER
// Copyright (c) 2023 Bashar Astifan.
// Email: bashar@astifan.online
// Telegram: @basharastifan
// GitHub: https://github.com/basharast/UWP2Win32

// This code must keep support for lower builds (15063+)
// Try always to find possible way to keep that support

// Functions:
// GetDataFromLocalSettings(std::string key)
// AddDataToLocalSettings(std::string key, std::string data, bool replace)
// 
// FillLookupList()

#pragma once

#include <string>

// Local settings
std::string GetDataFromLocalSettings(std::string key);
bool AddDataToLocalSettings(std::string key, std::string data, bool replace);

// Lookup list
void FillLookupList();
