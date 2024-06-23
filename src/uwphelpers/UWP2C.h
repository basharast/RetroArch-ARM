// UWP STORAGE MANAGER
// Copyright (c) 2023 Bashar Astifan.
// Email: bashar@astifan.online
// Telegram: @basharastifan
// GitHub: https://github.com/basharast/UWP2Win32

// This is small bridge to invoke UWP Storage manager

// This code must keep support for lower builds (15063+)
// Try always to find possible way to keep that support

// Functions:
// CreateFileUWP(const char* path, int accessMode, int shareMode, int openMode)
// GetFileAttributesUWP(const void* name, void* lpFileInformation)
// DeleteFileUWP(const void* name)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void* CreateFileUWP(const char* path, int accessMode, int shareMode, int openMode);
int GetFileAttributesUWP(const void* name, void* lpFileInformation);
int DeleteFileUWP(const void* name);
bool CopyUWP(char* path, char* dest);
bool CopyUWP2(wchar_t* path, wchar_t* dest);
bool CreateDirectoryUWP(char* path, bool replaceExisting);

#ifdef __cplusplus
}
#endif
