// UWP STORAGE MANAGER
// Copyright (c) 2023 Bashar Astifan.
// Email: bashar@astifan.online
// Telegram: @basharastifan
// GitHub: https://github.com/basharast/UWP2Win32

// This code must keep support for lower builds (15063+)
// Try always to find possible way to keep that support

// Functions:
// GetWorkingFolder()
// SetWorkingFolder(std::string location)
// GetInstallationFolder()
// GetLocalFolder()
// GetTempFolder()
// GetPicturesFolder()
// GetVideosFolder()
// GetDocumentsFolder()
// GetMusicFolder()
// GetPreviewPath(std::string path)
//
// CreateFileUWP(std::string path, int accessMode, int shareMode, int openMode)
// CreateFileUWP(std::wstring path, int accessMode, int shareMode, int openMode)
// GetFileStream(std::string path, const char* mode)
// IsValidUWP(std::string path)
// IsExistsUWP(std::string path)
// IsDirectoryUWP(std::string path)
// 
// GetFolderContents(std::string path, bool deepScan = false)
// GetFolderContents(std::wstring path, bool deepScan = false)
// GetFileInfoUWP(std::string path)
//
// GetSizeUWP(std::string path)
// DeleteUWP(std::string path)
// CreateDirectoryUWP(std::string path, bool replaceExisting)
// RenameUWP(std::string path, std::string name)
// CopyUWP(std::string path, std::string name)
// MoveUWP(std::string path, std::string name)
//
// OpenFile(std::string path)
// OpenFolder(std::string path)
// IsFirstStart()
//
// GetLogFile()
// SaveLogs()
// CleanupLogs()

#pragma once 

#include <list>

#include "StoragePath.h"
#include "StorageInfo.h"
#include "StorageAccess.h"
#include "StoragePickers.h"

// Locations
std::string GetWorkingFolder(); // Where main data is, default is app data
void SetWorkingFolder(std::string location); // Change working location
std::string GetInstallationFolder();
std::string GetLocalFolder();
std::string GetTempFolder();
std::string GetTempFile(std::string name);
std::string GetPicturesFolder(); // Requires 'picturesLibrary' capability
std::string GetVideosFolder(); // Requires 'videosLibrary' capability
std::string GetDocumentsFolder(); // Requires 'documentsLibrary' capability
std::string GetMusicFolder(); // Requires 'musicLibrary' capability
std::string GetPreviewPath(std::string path);
std::list<std::string> GetLookupLocations();

// Management
HANDLE CreateFileUWP(std::string path, int accessMode = GENERIC_READ, int shareMode = FILE_SHARE_READ, int openMode = OPEN_EXISTING);
HANDLE CreateFileUWP(std::wstring path, int accessMode = GENERIC_READ, int shareMode = FILE_SHARE_READ, int openMode = OPEN_EXISTING);
FILE* GetFileStream(std::string path, const char* mode);
bool IsValidUWP(std::string path, bool allowForAppData = false);
bool IsValidUWP(std::wstring path);
bool IsExistsUWP(std::string path);
bool IsExistsUWP(std::wstring path);
bool IsDirectoryUWP(std::string path);
bool IsDirectoryUWP(std::wstring path);

std::list<ItemInfoUWP> GetFolderContents(std::string path, bool deepScan = false);
std::list<ItemInfoUWP> GetFolderContents(std::wstring path, bool deepScan = false);
ItemInfoUWP GetItemInfoUWP(std::string path);
ItemInfoUWP GetItemInfoUWP(std::wstring path);

// Basics
int64_t GetSizeUWP(std::string path);
int64_t GetSizeUWP(std::wstring path);
bool DeleteUWP(std::string path);
bool DeleteUWP(std::wstring path);
bool CreateDirectoryUWP(std::string path, bool replaceExisting = true);
bool CreateDirectoryUWP(std::wstring path, bool replaceExisting = true);
bool RenameUWP(std::string path, std::string name);
bool RenameUWP(std::wstring path, std::wstring name);
// Add file name to destination path
bool CopyUWP(std::string path, std::string dest);
bool CopyUWP(std::wstring path, std::wstring dest);
// Add file name to destination path
bool MoveUWP(std::string path, std::string dest);
bool MoveUWP(std::wstring path, std::wstring dest);

// Helpers
bool OpenFile(std::string path);
bool OpenFile(std::wstring path);
bool OpenFolder(std::string path);
bool OpenFolder(std::wstring path);
bool IsFirstStart();
bool GetDriveFreeSpace(PathUWP path, int64_t& space);
bool CheckDriveAccess(std::string driveName, bool checkIfContainsFutureAccessItems);

// Log helpers
std::string GetLogFile();
bool SaveLogs(); // With picker
void CleanupLogs();
