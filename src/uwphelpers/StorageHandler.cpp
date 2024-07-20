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

#include "StorageHandler.h"
#include "StorageExtensions.h"

HRESULT GetFileHandle(StorageFile^ file, HANDLE* handle, HANDLE_ACCESS_OPTIONS accessMode, HANDLE_SHARING_OPTIONS shareMode)
{
	if (file != nullptr) {
		Microsoft::WRL::ComPtr<IUnknown> abiPointer(reinterpret_cast<IUnknown*>(file));
		Microsoft::WRL::ComPtr<IStorageItemHandleAccess> handleAccess;
		if (SUCCEEDED(abiPointer.As(&handleAccess)))
		{
			HANDLE hFile = INVALID_HANDLE_VALUE;

			if (SUCCEEDED(handleAccess->Create(accessMode,
				shareMode,
				HO_NONE,
				nullptr,
				&hFile)))
			{
				*handle = hFile;
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

HRESULT GetFileHandleFromFolder(StorageFolder^ folder, std::string filename, HANDLE* handle, HANDLE_ACCESS_OPTIONS accessMode, HANDLE_SHARING_OPTIONS shareMode, HANDLE_CREATION_OPTIONS openMode)
{
	if (folder != nullptr) {
		Microsoft::WRL::ComPtr<IUnknown> abiPointer(reinterpret_cast<IUnknown*>(folder));
		Microsoft::WRL::ComPtr<IStorageFolderHandleAccess> handleAccess;
		if (SUCCEEDED(abiPointer.As(&handleAccess)))
		{
			HANDLE hFolder = INVALID_HANDLE_VALUE;
			auto fn = convertToLPCWSTR(filename);
			if (SUCCEEDED(handleAccess->Create(fn,
				openMode,
				accessMode,
				shareMode,
				HO_NONE,
				nullptr,
				&hFolder)))
			{
				*handle = hFolder;
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

HRESULT GetFolderHandle(StorageFolder^ folder, HANDLE* handle, HANDLE_ACCESS_OPTIONS accessMode, HANDLE_SHARING_OPTIONS shareMode)
{
	if (folder != nullptr) {
		Microsoft::WRL::ComPtr<IUnknown> abiPointer(reinterpret_cast<IUnknown*>(folder));
		Microsoft::WRL::ComPtr<IStorageItemHandleAccess> handleAccess;
		if (SUCCEEDED(abiPointer.As(&handleAccess)))
		{
			HANDLE hFolder = INVALID_HANDLE_VALUE;

			if (SUCCEEDED(handleAccess->Create(accessMode,
				shareMode,
				HO_NONE,
				nullptr,
				&hFolder)))
			{
				*handle = hFolder;
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

HANDLE_ACCESS_OPTIONS GetAccessMode(int accessMode) {
	switch (accessMode) {
	case GENERIC_READ:
		return HAO_READ | HAO_READ_ATTRIBUTES;
	case GENERIC_WRITE:
		return HAO_WRITE | HAO_READ;
	case GENERIC_ALL:
		return HAO_READ | HAO_READ_ATTRIBUTES | HAO_WRITE | HAO_DELETE;
	default:
		return HAO_READ;
	}
}

HANDLE_SHARING_OPTIONS GetShareMode(int shareMode) {
	switch (shareMode)
	{
	case FILE_SHARE_READ:
		return HSO_SHARE_READ;
	case FILE_SHARE_WRITE:
		return HSO_SHARE_READ | HSO_SHARE_WRITE;
	case FILE_SHARE_DELETE:
		return HSO_SHARE_DELETE;
	default:
		return HSO_SHARE_READ;
	}
}

HANDLE_CREATION_OPTIONS GetOpenMode(int openMode) {
	switch (openMode)
	{
	case CREATE_NEW:
		return HCO_CREATE_NEW;
	case CREATE_ALWAYS:
		return HCO_CREATE_ALWAYS;
	case OPEN_ALWAYS:
		return HCO_OPEN_ALWAYS;
	case OPEN_EXISTING:
		return HCO_OPEN_EXISTING;
	default:
		return HCO_OPEN_EXISTING;
	}
}
