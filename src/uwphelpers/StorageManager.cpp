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

#include "pch.h"

#include "StorageConfig.h"
#include "StorageManager.h"
#include "StorageExtensions.h"
#include "StorageHandler.h"
#include "StorageAsync.h"
#include "StorageAccess.h"
#include "StorageItemW.h"
#include "StorageLog.h"

#if !defined(_M_ARM)
#include <fileapifromapp.h>
#endif

#include <defaults.h>
#include <ctime>
#include "configuration.h"

using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;

extern std::list<StorageItemW> FutureAccessItems;

#pragma region Locations
std::string GetWorkingFolder() {
	if (AppWorkingFolder.empty()) {
		return GetLocalFolder();
	}
	else {
		return AppWorkingFolder;
	}
}
void SetWorkingFolder(std::string location) {
	AppWorkingFolder = location;
}
std::string GetInstallationFolder() {
	return convert(Package::Current->InstalledLocation->Path);
}
StorageFolder^ GetLocalStorageFolder() {
	return ApplicationData::Current->LocalFolder;
}
std::string GetLocalFolder() {
	return convert(GetLocalStorageFolder()->Path);
}
std::string GetTempFolder() {
	return convert(ApplicationData::Current->TemporaryFolder->Path);
}
std::string GetTempFile(std::string name) {
	StorageFile^ tmpFile;
	ExecuteTask(tmpFile, ApplicationData::Current->TemporaryFolder->CreateFileAsync(convert(name), CreationCollisionOption::GenerateUniqueName));
	if (tmpFile != nullptr) {
		return convert(tmpFile->Path);
	}
	else {
		return "";
	}
}
std::string GetPicturesFolder() {
	// Requires 'picturesLibrary' capability
	return convert(KnownFolders::PicturesLibrary->Path);
}
std::string GetVideosFolder() {
	// Requires 'videosLibrary' capability
	return convert(KnownFolders::VideosLibrary->Path);
}
std::string GetDocumentsFolder() {
	// Requires 'documentsLibrary' capability
	return convert(KnownFolders::DocumentsLibrary->Path);
}
std::string GetMusicFolder() {
	// Requires 'musicLibrary' capability
	return convert(KnownFolders::MusicLibrary->Path);
}
std::string GetPreviewPath(std::string path) {
	std::string pathView = path;
	windowsPath(pathView);
	std::string appData = GetLocalFolder();
	replace(appData, "\\LocalState", "");
	replace(pathView, appData, "AppData");
	return pathView;
}

std::list<std::string> GetLookupLocations() {
	std::list<std::string> locations;
	for (auto fItem : FutureAccessItems) {
		if (fItem.Equal(ApplicationData::Current->LocalFolder) || fItem.Equal(ApplicationData::Current->TemporaryFolder)) {
			continue;
		}
		if (fItem.Equal(Package::Current->InstalledLocation)) {
			continue;
		}
		locations.push_back(fItem.GetPath());
	}

	return locations;
}
#pragma endregion

#pragma region Internal
std::string GetLastErrorAsString() {
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return "Unknown";
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA( // Use FormatMessageA for ANSI
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size - 2); // -2 to strip \r\n appended at the end
	LocalFree(messageBuffer); // Free the buffer allocated by FormatMessage

	return message;
}

PathUWP PathResolver(PathUWP path) {
	auto root = path.GetDirectory();
	auto newPath = path.ToString();
	if (path.IsRoot() || iequals(root, "/") || iequals(root, "\\")) {
		// System requesting file from app data
		replace(newPath, "/", (GetLocalFolder() + (path.size() > 1 ? "/" : "")));
	}
	else if (!path.empty() && !path.IsAbsolute()) {
		RARCH_LOG("File will be redirected to system dir (%s)\n", path.ToString().c_str());
		settings_t* settings = config_get_ptr();
		auto sysDir = GetLocalFolder();
		if (settings) {
			sysDir = settings->paths.directory_system;
		}
		auto currentPath = path.ToString();
		windowsPath(currentPath);
		newPath = sysDir + "\\" + currentPath;
		RARCH_LOG("New path (%s)\n", newPath.c_str());
	}
	//else if (newPath.find(".\\") == 0) {
	//	//Force look into app data
	//	replace(newPath, ".\\", (GetLocalFolder() + (path.size() > 1 ? "/" : "")));
	//}
	//else if (newPath.find("./") == 0) {
	//	replace(newPath, "./", (GetLocalFolder() + (path.size() > 1 ? "/" : "")));
	//}
	path = PathUWP(newPath);
	return path;
}
PathUWP PathResolver(std::string path) {
	return PathResolver(PathUWP(path));
}

// Return closer parent
StorageItemW GetStorageItemParent(PathUWP path) {
	path = PathResolver(path);
	StorageItemW parent;

	for (auto& fItem : FutureAccessItems) {
		if (isChild(fItem.GetPath(), path.ToString())) {
			if (fItem.IsDirectory()) {
				parent = fItem;
				break;
			}
		}
	}

	return parent;
}

StorageItemW GetStorageItem(PathUWP path, bool createIfNotExists = false, bool forceFolderType = false) {
	// Fill call will be ignored internally after the first call
	FillLookupList();

	path = PathResolver(path);
	StorageItemW item;

	// Look for match in FutureAccessItems
	for (auto& fItem : FutureAccessItems) {
		if (fItem.Equal(path)) {
			item = fItem;
			break;
		}
	}

	if (!item.IsValid()) {
		// Look for match inside FutureAccessFolders
		for (auto& fItem : FutureAccessItems) {
			if (fItem.IsDirectory()) {
				IStorageItem^ storageItem;
				if (fItem.Contains(path, storageItem)) {
					item = StorageItemW(storageItem);
					break;
				}
			}
		}
	}

	if (!item.IsValid() && createIfNotExists) {
		// Create and return new folder
		auto parent = GetStorageItemParent(path);
		if (parent.IsValid()) {
			if (!forceFolderType) {
				// File creation must be called in this case
				// Create folder usually will be called from 'CreateDirectory'
				item = StorageItemW(parent.CreateFile(path));
			}
			else {
				item = StorageItemW(parent.CreateFolder(path));
			}
		}
	}
	return item;
}

StorageItemW GetStorageItem(std::string path, bool createIfNotExists = false, bool forceFolderType = false) {
	return GetStorageItem(PathUWP(path), createIfNotExists, forceFolderType);
}

std::list<StorageItemW> GetStorageItemsByParent(PathUWP path) {
	path = PathResolver(path);
	std::list<StorageItemW> items;

	// Look for match in FutureAccessItems
	for (auto& fItem : FutureAccessItems) {
		if (isParent(path.ToString(), fItem.GetPath(), fItem.GetName())) {
			items.push_back(fItem);
		}
	}

	return items;
}

std::list<StorageItemW> GetStorageItemsByParent(std::string path) {
	return GetStorageItemsByParent(PathUWP(path));
}

bool IsContainsAccessibleItems(PathUWP path) {
	path = PathResolver(path);

	for (auto& fItem : FutureAccessItems) {
		if (isParent(path.ToString(), fItem.GetPath(), fItem.GetName())) {
			return true;
		}
	}

	return false;
}

bool IsContainsAccessibleItems(std::string path) {
	return IsContainsAccessibleItems(PathUWP(path));
}

bool IsRootForAccessibleItems(PathUWP path, std::list<std::string>& subRoot, bool breakOnFirstMatch = false) {
	path = PathResolver(path);

	for (auto& fItem : FutureAccessItems) {
		if (isChild(path.ToString(), fItem.GetPath())) {
			if (breakOnFirstMatch) {
				// Just checking, we don't need to loop for each item
				return true;
			}
			auto sub = getSubRoot(path.ToString(), fItem.GetPath());

			// This check can be better, but that's how I can do it in C++
			if (!ends_with(sub, ":")) {
				bool alreadyAdded = false;
				for each (auto sItem in subRoot) {
					if (iequals(sItem, sub)) {
						alreadyAdded = true;
						break;
					}
				}
				if (!alreadyAdded) {
					subRoot.push_back(sub);
				}
			}
		}
	}

	return !subRoot.empty();
}

bool IsRootForAccessibleItems(std::string path, std::list<std::string>& subRoot, bool breakOnFirstMatch = false) {
	return IsRootForAccessibleItems(PathUWP(path), subRoot, breakOnFirstMatch);
}
#pragma endregion

#pragma region Functions
bool CreateIfNotExists(int openMode) {
	switch (openMode)
	{
	case OPEN_ALWAYS:
	case CREATE_ALWAYS:
	case CREATE_NEW:
		return true;
	default:
		return false;
	}
}

HANDLE CreateFileUWP(std::string path, int accessMode, int shareMode, int openMode) {
	HANDLE handle{};
	if (IsValidUWP(path)) {
		bool createIfNotExists = CreateIfNotExists(openMode);
		auto storageItem = GetStorageItem(path, createIfNotExists);

		if (storageItem.IsValid()) {
			RARCH_DBG("Getting handle (%s)\n", path.c_str());
			HRESULT hr = storageItem.GetHandle(&handle, accessMode, shareMode);
			if (hr == E_FAIL) {
				handle = INVALID_HANDLE_VALUE;
			}
		}
		else {
			handle = INVALID_HANDLE_VALUE;
			RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
		}
	}
	return handle;
}

HANDLE CreateFileUWP(std::wstring path, int accessMode, int shareMode, int openMode) {
	auto pathString = convert(path);
	return CreateFileUWP(pathString, accessMode, shareMode, openMode);
}

bool IsRootForAccessibleItems(std::string path) {
	std::list<std::string> tmp;
	return IsRootForAccessibleItems(path, tmp, true);
}

std::map<std::string, bool> accessState;
bool CheckDriveAccess(std::string driveName, bool checkIfContainsFutureAccessItems)
{
	bool state = false;

	auto keyIter = accessState.find(driveName);
	if (keyIter != accessState.end())
	{
		state = keyIter->second;
	}
	else
	{
		try {
			auto dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			auto dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
			auto dwCreationDisposition = CREATE_ALWAYS;

			auto testFile = std::string(driveName);
			testFile.append("\\.UWPAccessCheck");
			replace(testFile, "\\\\", "\\");
#if defined(_M_ARM) || IS_LEVEL_93
			HANDLE h = CreateFile2(convertToLPCWSTR(testFile), dwDesiredAccess, dwShareMode, dwCreationDisposition, nullptr);
#else
			HANDLE h = CreateFile2(convertToLPCWSTR(testFile), dwDesiredAccess, dwShareMode, dwCreationDisposition, nullptr);
#endif
			DWORD errorMessageId = ::GetLastError();
			auto errorString = GetLastErrorAsString();
			if (h != INVALID_HANDLE_VALUE) {
				//Double check
				CloseHandle(h);
				FILE* tFile = fopen(testFile.c_str(), "r+");
				if (tFile) {
					fclose(tFile);
					state = true;
#if defined(_M_ARM) || IS_LEVEL_93
					DeleteFileW(convertToLPCWSTR(testFile));
#else
					DeleteFileW(convertToLPCWSTR(testFile));
#endif
				}
			}
			accessState.insert(std::make_pair(driveName, state));
		}
		catch (...) {
		}
	}

	if (!state && checkIfContainsFutureAccessItems)
	{
		// Consider the drive accessible in case it contain files/folder selected before to avoid empty results
		state = IsRootForAccessibleItems(driveName) || IsContainsAccessibleItems(driveName);
	}
	return state;
}

bool IsValidUWP(std::string path, bool allowForAppData) {
	// The idea of this functions is to determine whether we need to use native UWP fallback,
	// this usually help to avoid unnecessary checks if file is not exists within accessible path,
	// usually API will fail and storage manager will try to use native fallback solution,
	// in this case as example we shouldn't fallback to check using UWP, it should end at API level,
	// so the result will be reversed at the end, 
	// means file/folder is not accessible by default and something prevent the API to work.
	auto p = PathResolver(path);

	//Check valid path
	if (p.Type() == PathTypeUWP::UNDEFINED || !p.IsAbsolute()) {
		// Nothing to do here
		UWP_VERBOSE_LOG(UWPSMT, "File is not valid (%s)", p.ToString().c_str());
		return false;
	}

	bool state = false;

	if (!allowForAppData) {
		auto resolvedPathStr = p.ToString();
		if (ends_with(resolvedPathStr, "LocalState") || ends_with(resolvedPathStr, "TempState") ||
			ends_with(resolvedPathStr, "LocalCache"))
		{
			state = true;
		}
		else if (isChild(GetLocalFolder(), resolvedPathStr))
		{
			state = true;
		}
		else if (isChild(GetInstallationFolder(), resolvedPathStr))
		{
			state = true;
		}
		else if (isChild(GetTempFolder(), resolvedPathStr))
		{
			state = true;
		}

		if (!state)
		{
			auto p = PathUWP(path);
			std::string driveName = p.GetRootVolume().ToString();
			state = CheckDriveAccess(driveName, false);
		}
	}

	return !state;
}

bool IsValidUWP(std::wstring path) {
	return IsValidUWP(convert(path));
}

bool IsExistsUWP(std::string path) {
	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {
			return true;
		}

		// If folder is not accessible but contains accessible items
		// consider it exists
		if (IsContainsAccessibleItems(path)) {
			return true;
		}

		// If folder is not accessible but is part of accessible items
		// consider it exists
		std::list<std::string> tmp;
		if (IsRootForAccessibleItems(path, tmp, true)) {
			return true;
		}
	}
	//  RARCH_DBG( "Couldn't find or access (%s)", path.c_str());
	return false;
}

bool IsExistsUWP(std::wstring path) {
	return IsExistsUWP(convert(path));
}

bool IsDirectoryUWP(std::string path) {
	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {
			if (storageItem.IsDirectory()) {
				return true;
			}
		}
	}
	return false;
}

bool IsDirectoryUWP(std::wstring path) {
	return IsDirectoryUWP(convert(path));
}

FILE* GetFileStream(std::string path, const char* mode) {
	FILE* file{};
	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {
			file = storageItem.GetStream(mode);
		}
		else {
			// Forward the request to parent folder
			auto p = PathUWP(path);
			auto itemName = p.GetFilename();
			auto rootPath = p.GetDirectory();
			if (IsValidUWP(rootPath)) {
				storageItem = GetStorageItem(rootPath);
				if (storageItem.IsValid()) {
					file = storageItem.GetFileStream(itemName, mode);
				}
				else {
					RARCH_DBG("Couldn't find or access (%s)\n", rootPath.c_str());
					RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
				}
			}
		}
	}

	return file;
		}

FILE* GetFileStream(std::wstring path, const char* mode) {
	return GetFileStream(convert(path), mode);
}

std::string ResolvePathUWP(std::string path) {
	return PathResolver(path).ToString();
}

FILE* GetFileStreamFromApp(std::string path, const char* mode) {

	FILE* file{};

	auto pathResolved = PathUWP(ResolvePathUWP(path));
	HANDLE handle = INVALID_HANDLE_VALUE;

	auto fileMode = GetFileMode(mode);
	if (fileMode) {
#if defined(_M_ARM) || IS_LEVEL_93
		handle = CreateFile2(pathResolved.ToWString().c_str(), fileMode->dwDesiredAccess, fileMode->dwShareMode, fileMode->dwCreationDisposition, nullptr);
#else
		handle = CreateFile2FromAppW(pathResolved.ToWString().c_str(), fileMode->dwDesiredAccess, fileMode->dwShareMode, fileMode->dwCreationDisposition, nullptr);
#endif
	}
	if (handle != INVALID_HANDLE_VALUE) {
		file = _fdopen(_open_osfhandle((intptr_t)handle, fileMode->flags), mode);
	}

	return file;
}

#pragma region Content Helpers
ItemInfoUWP GetFakeFolderInfo(std::string folder) {
	ItemInfoUWP info;
	auto folderPath = PathUWP(folder);
	info.name = folderPath.GetFilename();
	info.fullName = folderPath.ToString();

	info.isDirectory = true;

	info.size = 1;
	info.lastAccessTime = 1000;
	info.lastWriteTime = 1000;
	info.changeTime = 1000;
	info.creationTime = 1000;

	info.attributes = FILE_ATTRIBUTE_DIRECTORY;

	return info;
}

#pragma endregion

std::list<ItemInfoUWP> GetFolderContents(std::string path, bool deepScan) {
	std::list<ItemInfoUWP> contents;

	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {

			// Files
			// deepScan is slow, try to avoid it
			auto rfiles = deepScan ? storageItem.GetAllFiles() : storageItem.GetFiles();
			for each (auto file in rfiles) {
				contents.push_back(file.GetFileInfo());
			}

			// Folders
			// deepScan is slow, try to avoid it
			auto rfolders = deepScan ? storageItem.GetAllFolders() : storageItem.GetFolders();
			for each (auto folder in rfolders) {
				contents.push_back(folder.GetFolderInfo());
			}
		}
		else {
			RARCH_DBG("Cannot get contents!, checking for other options.. (%s)\n", path.c_str());
		}
		;
	}

	if (contents.size() == 0) {
		// Folder maybe not accessible or not exists
			// if not accessible, maybe some items inside it were selected before
			// and they already in our accessible list
		if (IsContainsAccessibleItems(path)) {
			RARCH_DBG("Folder contains accessible items (%s)\n", path.c_str());

			// Check contents
			auto cItems = GetStorageItemsByParent(path);
			if (!cItems.empty()) {
				for each (auto item in cItems) {
					RARCH_DBG("Appending accessible item (%s)\n", item.GetPath().c_str());
					contents.push_back(item.GetItemInfo());
				}
			}
		}
		else
		{
			// Check if this folder is root for accessible item
			// then add fake folder as sub root to avoid empty results
			std::list<std::string> subRoot;
			if (IsRootForAccessibleItems(path, subRoot)) {
				RARCH_DBG("Folder is root for accessible items (%s)\n", path.c_str());

				if (!subRoot.empty()) {
					for each (auto sItem in subRoot) {
						RARCH_DBG("Appending fake folder (%s)\n", sItem.c_str());
						contents.push_back(GetFakeFolderInfo(sItem));
					}
				}
			}
			else {
				RARCH_DBG("Cannot get any content!.. (%s)\n", path.c_str());
			}
		}
	}
	return contents;
}
std::list<ItemInfoUWP> GetFolderContents(std::wstring path, bool deepScan) {
	return GetFolderContents(convert(path), deepScan);
}

ItemInfoUWP GetItemInfoUWP(std::string path) {
	ItemInfoUWP info;
	info.size = -1;
	info.attributes = INVALID_FILE_ATTRIBUTES;

	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {
			info = storageItem.GetItemInfo();
		}
		else {
			RARCH_LOG("Couldn't find or access (%s)\n", path.c_str());
		}
	}

	return info;
}

ItemInfoUWP GetItemInfoUWP(std::wstring path) {
	return GetItemInfoUWP(convert(path));
}
#pragma endregion

#pragma region Basics
int64_t GetSizeUWP(std::string path) {
	int64_t size = 0;
	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {
			size = storageItem.GetSize();
		}
		else {
			RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
		}
	}
	return size;
}

int64_t GetSizeUWP(std::wstring path) {
	return GetSizeUWP(convert(path));
}

bool DeleteUWP(std::string path) {
	bool state = false;
	if (IsValidUWP(path)) {
		auto storageItem = GetStorageItem(path);
		if (storageItem.IsValid()) {
			RARCH_DBG("Delete (%s)\n", path.c_str());
			state = storageItem.Delete();
		}
		else {
			RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
		}
	}

	return state;
}

bool DeleteUWP(std::wstring path) {
	return DeleteUWP(convert(path));
}

bool CreateDirectoryUWP(std::string path, bool replaceExisting) {
	bool state = false;
	auto p = PathUWP(path);
	auto itemName = p.GetFilename();
	auto rootPath = p.GetDirectory();

	if (IsValidUWP(rootPath)) {
		auto storageItem = GetStorageItem(rootPath);
		if (storageItem.IsValid()) {
			RARCH_DBG("Create new folder (%s)\n", path.c_str());
			state = storageItem.CreateFolder(itemName, replaceExisting);
		}
		else {
			RARCH_DBG("Couldn't find or access (%s)\n", rootPath.c_str());
		}
	}

	return state;
}

bool CreateDirectoryUWP(std::wstring path, bool replaceExisting) {
	return CreateDirectoryUWP(convert(path), replaceExisting);
}

bool CopyUWP(std::string path, std::string dest) {
	bool state = false;

	if (IsValidUWP(path, true) && IsValidUWP(dest, true)) {
		auto srcStorageItem = GetStorageItem(path);
		if (srcStorageItem.IsValid()) {
			auto destDir = dest;
			auto srcName = srcStorageItem.GetName();
			auto dstPath = PathUWP(dest);
			auto dstName = dstPath.GetFilename();
			// Destination must be parent folder
			destDir = dstPath.GetDirectory();
			auto dstStorageItem = GetStorageItem(destDir, true, true);
			if (dstStorageItem.IsValid()) {
				RARCH_DBG("Copy (%s) to (%s)\n", path.c_str(), dest.c_str());
				state = srcStorageItem.Copy(dstStorageItem, dstName);
			}
			else {
				RARCH_DBG("Couldn't find or access (%s)\n", dest.c_str());
			}
		}
		else {
			RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
		}
	}

	return state;
}

bool CopyUWP(std::wstring path, std::wstring dest) {
	return CopyUWP(convert(path), convert(dest));
}

bool MoveUWP(std::string path, std::string dest) {
	bool state = false;

	if (IsValidUWP(path, true) && IsValidUWP(dest, true)) {
		auto srcStorageItem = GetStorageItem(path);

		if (srcStorageItem.IsValid()) {
			auto destDir = dest;
			auto srcName = srcStorageItem.GetName();
			auto dstPath = PathUWP(dest);
			auto dstName = dstPath.GetFilename();
			// Destination must be parent folder
			destDir = dstPath.GetDirectory();
			auto dstStorageItem = GetStorageItem(destDir, true, true);
			if (dstStorageItem.IsValid()) {
				RARCH_DBG("Move (%s) to (%s)\n", path.c_str(), dest.c_str());
				state = srcStorageItem.Move(dstStorageItem, dstName);
			}
			else {
				RARCH_DBG("Couldn't find or access (%s)\n", dest.c_str());
			}
		}
		else {
			RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
		}
	}

	return state;
}

bool MoveUWP(std::wstring path, std::wstring dest) {
	return MoveUWP(convert(path), convert(dest));
}

bool RenameUWP(std::string path, std::string name) {
	bool state = false;

	auto srcRoot = PathUWP(path).GetDirectory();
	auto dstRoot = PathUWP(name).GetDirectory();
	// Check if system using rename to move
	if (iequals(srcRoot, dstRoot)) {
		auto srcStorageItem = GetStorageItem(path);
		if (srcStorageItem.IsValid()) {
			RARCH_DBG("Rename (%s) to (%s)\n", path.c_str(), name.c_str());
			state = srcStorageItem.Rename(name);
		}
		else {
			RARCH_DBG("Couldn't find or access (%s)\n", path.c_str());
		}
	}
	else {
		RARCH_DBG(" Rename used as move -> call move (%s) to (%s)\n", path.c_str(), name.c_str());
		state = MoveUWP(path, name);
	}

	return state;
}

bool RenameUWP(std::wstring path, std::wstring name) {
	return RenameUWP(convert(path), convert(name));
}
#pragma endregion


#pragma region Helpers
bool OpenFile(std::string path) {
	bool state = false;

	auto storageItem = GetStorageItem(path);
	if (storageItem.IsValid()) {
		if (!storageItem.IsDirectory()) {
			ExecuteTask(state, Windows::System::Launcher::LaunchFileAsync(storageItem.GetStorageFile()), false);
		}
	}
	else {
		auto uri = ref new Windows::Foundation::Uri(convert(path));
		ExecuteTask(state, Windows::System::Launcher::LaunchUriAsync(uri), false);
	}
	return state;
}

bool OpenFile(std::wstring path) {
	return OpenFile(convert(path));
}

bool OpenFolder(std::string path) {
	bool state = false;
	PathUWP itemPath(path);
	Platform::String^ wString = ref new Platform::String(itemPath.ToWString().c_str());
	StorageFolder^ storageItem;
	ExecuteTask(storageItem, StorageFolder::GetFolderFromPathAsync(wString));
	if (storageItem != nullptr) {
		ExecuteTask(state, Windows::System::Launcher::LaunchFolderAsync(storageItem), false);
	}
	else {
		// Try as it's file
		PathUWP parent = PathUWP(itemPath.GetDirectory());
		Platform::String^ wParentString = ref new Platform::String(parent.ToWString().c_str());

		ExecuteTask(storageItem, StorageFolder::GetFolderFromPathAsync(wParentString));
		if (storageItem != nullptr) {
			ExecuteTask(state, Windows::System::Launcher::LaunchFolderAsync(storageItem), false);
		}
	}
	return state;
}

bool OpenFolder(std::wstring path) {
	return OpenFolder(convert(path));
}

bool GetDriveFreeSpace(PathUWP path, int64_t& space) {

	bool state = false;
	Platform::String^ wString = ref new Platform::String(path.ToWString().c_str());
	StorageFolder^ storageItem;
	ExecuteTask(storageItem, StorageFolder::GetFolderFromPathAsync(wString));
	if (storageItem != nullptr) {
		Platform::String^ freeSpaceKey = ref new Platform::String(L"System.FreeSpace");
		Platform::Collections::Vector<Platform::String^>^ propertiesToRetrieve = ref new Platform::Collections::Vector<Platform::String^>();
		propertiesToRetrieve->Append(freeSpaceKey);
		Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ result;
		ExecuteTask(result, storageItem->Properties->RetrievePropertiesAsync(propertiesToRetrieve));
		if (result != nullptr && result->Size > 0) {
			try {
				auto value = result->Lookup(L"System.FreeSpace");
				space = (uint64_t)value;
				state = true;
			}
			catch (...) {

			}
		}
	}

	return state;
}

bool IsFirstStart() {
	auto firstrun = GetDataFromLocalSettings("first_run");
	AddDataToLocalSettings("first_run", "done", true);
	return firstrun.empty();
}
#pragma endregion

#pragma region Logs
// Get log file name
std::string currentLogFile;
std::string getLogFileName() {
	//Initial new name each session/launch
	if (currentLogFile.empty() || currentLogFile.size() == 0) {
		std::time_t now = std::time(0);
		char mbstr[100];
		std::strftime(mbstr, 100, "rarch %d-%m-%Y (%T).txt", std::localtime(&now));
		std::string formatedDate(mbstr);
		std::replace(formatedDate.begin(), formatedDate.end(), ':', '-');
		currentLogFile = formatedDate;
	}

	return currentLogFile;
}

// Get current log file location
StorageFolder^ GetLogsStorageFolder() {
	// Ensure 'LOGS' folder is created
	auto workingFolder = GetStorageItem(GetWorkingFolder());
	StorageFolder^ logsFolder;
	if (workingFolder.IsValid()) {
		auto workingStorageFolder = workingFolder.GetStorageFolder();
		ExecuteTask(logsFolder, workingStorageFolder->CreateFolderAsync("LOGS", CreationCollisionOption::OpenIfExists));
	}
	return logsFolder;
}
std::string GetLogFile() {
	std::string logFilePath = "";

	// Ensure 'LOGS' folder is created
	StorageFolder^ logsFolder = GetLogsStorageFolder();

	if (logsFolder != nullptr) {
		auto logFileName = convert(getLogFileName());
		StorageFile^ logFile;
		ExecuteTask(logFile, logsFolder->CreateFileAsync(logFileName, CreationCollisionOption::OpenIfExists));

		if (logFile != nullptr) {
			logFilePath = convert(logFile->Path);
		}
	}

	return logFilePath;
}

// Save logs to folder selected by the user
bool SaveLogs() {
	try {
		auto folderPicker = ref new Windows::Storage::Pickers::FolderPicker();
		folderPicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::Desktop;
		folderPicker->FileTypeFilter->Append("*");

		StorageFolder^ saveFolder;
		ExecuteTask(saveFolder, folderPicker->PickSingleFolderAsync());

		if (saveFolder != nullptr) {
			StorageFolder^ logsFolder = GetLogsStorageFolder();

			if (logsFolder != nullptr) {
				StorageFolderW logsCache(logsFolder);
				logsCache.Copy(saveFolder);
			}
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

void CleanupLogs() {
	StorageFolder^ logsFolder = GetLogsStorageFolder();
	if (logsFolder != nullptr) {
		StorageFolderW logsCache(logsFolder);
		int total = 0;
		std::list<StorageFileW> files = logsCache.GetFiles(total);
		if (!files.empty()) {
			for each (auto fItem in files) {
				if (fItem.GetSize() == 0) {
					fItem.Delete();
				}
			}
		}
	}
}
#pragma endregion
