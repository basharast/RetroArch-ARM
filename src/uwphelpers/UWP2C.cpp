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

#include "UWP2C.h"
#include "StorageManager.h"
#include "StorageExtensions.h"

#ifdef __cplusplus
extern "C" {
#endif
bool CheckDriveAccess(const char* path) {
	std::string fn = convert(path);
	return CheckDriveAccess(fn, true);
}

void* CreateFileUWP(const char* path, int accessMode, int shareMode, int openMode) {
	std::string fn = convert(path);
	
	return (void*)CreateFileUWP(fn, accessMode, shareMode, openMode);
}

int GetFileAttributesUWP(const void* name, void* lpFileInformation) {
	size_t size = sizeof(WIN32_FILE_ATTRIBUTE_DATA);
	WIN32_FILE_ATTRIBUTE_DATA* file_attributes = (WIN32_FILE_ATTRIBUTE_DATA*)(malloc(size));
	
	std::string fn = convert((const char*)name);
	HANDLE handle = CreateFileUWP(fn);

	FILETIME createTime{};
	FILETIME changeTime{};
	
	DWORD fileSizeHigh = 0;
	DWORD fileSizeLow = 0;
	DWORD fileAttributes = 32;

	if (handle != INVALID_HANDLE_VALUE) {
		size_t size = sizeof(FILE_BASIC_INFO);
		FILE_BASIC_INFO* information = (FILE_BASIC_INFO*)(malloc(size));
		if (information) {
			if (FALSE != GetFileInformationByHandleEx(handle, FileBasicInfo, information, (DWORD)size)) {
				createTime.dwHighDateTime = information->CreationTime.HighPart;
				createTime.dwLowDateTime = information->CreationTime.LowPart;
				changeTime.dwHighDateTime = information->ChangeTime.HighPart;
				changeTime.dwLowDateTime = information->ChangeTime.LowPart;
				fileAttributes = information->FileAttributes;
			}
		}
		LARGE_INTEGER fsize{ 0 };
		if (FALSE == GetFileSizeEx(handle, &fsize)) {
			LARGE_INTEGER end_offset;
			const LARGE_INTEGER zero{};
			if (SetFilePointerEx(handle, zero, &end_offset, FILE_END) == 0) {
				CloseHandle(handle);
			}
			else {
				fileSizeHigh = (DWORD)end_offset.HighPart;
				fileSizeLow = (DWORD)end_offset.LowPart;
				SetFilePointerEx(handle, zero, nullptr, FILE_BEGIN);
				CloseHandle(handle);
			}
		}
		else {
			fileSizeHigh = (DWORD)fsize.HighPart;
			fileSizeLow = (DWORD)fsize.LowPart;

			CloseHandle(handle);
		}
	}

	((WIN32_FILE_ATTRIBUTE_DATA*)lpFileInformation)->ftCreationTime = createTime;
	((WIN32_FILE_ATTRIBUTE_DATA*)lpFileInformation)->ftLastAccessTime = changeTime;
	((WIN32_FILE_ATTRIBUTE_DATA*)lpFileInformation)->ftLastWriteTime = changeTime;
	((WIN32_FILE_ATTRIBUTE_DATA*)lpFileInformation)->nFileSizeHigh = fileSizeHigh;
	((WIN32_FILE_ATTRIBUTE_DATA*)lpFileInformation)->nFileSizeLow = fileSizeLow;
	((WIN32_FILE_ATTRIBUTE_DATA*)lpFileInformation)->dwFileAttributes = fileAttributes;

	return 1;
}

int DeleteFileUWP(const void* name) {
	std::string fn = convert((const char*)name);
	bool state = DeleteUWP(fn);

	return state ? 1 : 0;
}

bool CopyUWP(char* path, char* dest) {
	return CopyUWP(std::string(path), std::string(dest));
}
bool CopyUWP2(wchar_t* path, wchar_t* dest) {
	return CopyUWP(std::wstring(path), std::wstring(dest));
}

bool CreateDirectoryUWP(char* path, bool replaceExisting) {
	return CreateDirectoryUWP(std::string(path), replaceExisting);
}
#ifdef __cplusplus
}
#endif
