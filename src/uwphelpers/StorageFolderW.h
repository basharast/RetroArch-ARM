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
#include <collection.h>

#include "StorageLog.h"
#include "StoragePath.h"
#include "StorageExtensions.h"
#include "StorageHandler.h"
#include "StorageAsync.h"
#include "StorageFileW.h"
#include "StorageInfo.h"

using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Search;

class StorageFolderW {
public:
	StorageFolderW() {
	}
	StorageFolderW(StorageFolder^ folder) {
		storageFolder = folder;
		folderSize = 0;
	}
	StorageFolderW(IStorageItem^ folder) {
		storageFolder = (StorageFolder^)folder;
		folderSize = 0;
	}
	~StorageFolderW() {
		delete storageFolder;
		delete properties;
	}

	// Detect if storage folder is not null
	bool IsValid() {
		return (storageFolder != nullptr);
	}

	// Get folder path
	std::string GetPath() {
		return convert(storageFolder->Path);
	}

	// Delete folder
	bool Delete() {
		bool state = ExecuteTask(storageFolder->DeleteAsync());
		if (state) {
			storageFolder = nullptr;
		}
		return state;
	}

	// Compare folder with std::string
	bool Equal(std::string path) {
		std::string folderPath = GetPath();
		
		// Fix slashs back from '/' to '\'
		windowsPath(path);
		return iequals(folderPath, path);
	}

	// Compare folder with Platform::String
	bool Equal(Platform::String^ path) {
		return storageFolder->Path->Equals(path);
	}

	// Compare folder with Path
	bool Equal(PathUWP path) {
		return Equal(path.ToString());
	}

	// Compare folder with StorageFolder
	bool Equal(StorageFolder^ folder) {
		return Equal(folder->Path);
	}

	// Get folder name
	std::string GetName() {
		return convert(storageFolder->Name);
	}

	// Create new folder
	bool CreateFolder(std::string name, bool replaceExisting = true) {
		bool state = false;
		StorageFolder^ newFolder;
		ExecuteTask(newFolder, storageFolder->CreateFolderAsync(convert(name), replaceExisting ? CreationCollisionOption::ReplaceExisting : CreationCollisionOption::OpenIfExists));
		
		if (newFolder != nullptr) {
			state = true;
		}

		delete newFolder;
		return state;
	}

	// Create new file
	bool CreateFile(std::string name, bool replaceExisting = true) {
		bool state = false;
		StorageFile^ newFile;
		ExecuteTask(newFile, storageFolder->CreateFileAsync(convert(name), replaceExisting ? CreationCollisionOption::ReplaceExisting : CreationCollisionOption::OpenIfExists));

		if (newFile != nullptr) {
			state = true;
		}

		delete newFile;
		return state;
	}

	// Rename folder
	bool Rename(std::string name) {
		auto path = PathUWP(name);
		if (path.IsAbsolute()) {
			name = path.GetFilename();
		}
		return ExecuteTask(storageFolder->RenameAsync(convert(name)));
	}

	// Get files 1st level only, no deep scan
	std::list<StorageFileW> GetFiles(int& total) {
		std::list<StorageFileW> files;

		IVectorView<StorageFile^>^ sFiles;
      RARCH_DBG("Getting files for %s\n", GetPath().c_str());

		ExecuteTask(sFiles, storageFolder->GetFilesAsync());
		if (sFiles != nullptr) {
         total = sFiles->Size;
			for (auto it = 0; it != sFiles->Size; ++it) {
				auto sItem = sFiles->GetAt(it);
				if (sItem != nullptr) {
					files.push_back(StorageFileW(sItem));
				}
			}
		}
		delete sFiles;
      RARCH_DBG("Total files added (%d) in (%s)\n", files.size(), GetPath().c_str());

		return files;
	}

	std::list<StorageFolderW> GetFolders() {
		std::list<StorageFolderW> folders;

		IVectorView<StorageFolder^>^ sFolders;
      RARCH_DBG("Getting folders for %s\n", GetPath().c_str());

		ExecuteTask(sFolders, storageFolder->GetFoldersAsync());
		if (sFolders != nullptr) {
         RARCH_DBG("Sub folders founded (%d) in (%s)\n", sFolders->Size, GetPath().c_str());
			for (auto it = 0; it != sFolders->Size; ++it) {
				auto sItem = sFolders->GetAt(it);
				if (sItem != nullptr) {
					folders.push_back(StorageFolderW(sItem));
				}
			}
		}
		delete sFolders;
      RARCH_DBG("Total folders added (%d) in (%s)\n", folders.size(), GetPath().c_str());

		return folders;
	}

	// Get all files including files in sub folders (deep scan)
	std::list<StorageFileW> GetAllFiles(int& total, bool useWindowsIndexer = false) {
			std::list<StorageFileW> files; // No structure one-level list

         RARCH_DBG("Getting all files for %s\n", GetPath().c_str());
			IVectorView<StorageFile^>^ sFiles;

			// Set query options to create groups of files within result
			QueryOptions^ queryOptions = ref new QueryOptions(CommonFolderQuery::DefaultQuery);
			queryOptions->FolderDepth = FolderDepth::Deep; // Search in all levels
			
			// Windows indexer is very bad, it will return missing results if the files recently copied
			// It's better to search without it, even if it will be slower
			queryOptions->IndexerOption = useWindowsIndexer ? IndexerOption::UseIndexerWhenAvailable : IndexerOption::DoNotUseIndexer;
			StorageFileQueryResult^ filesResult = storageFolder->CreateFileQueryWithOptions(queryOptions);

			// Windows search query is slow but it's the only solution (or we need to build safe recursive function)
			// Regular 'StorageFolder->GetFilesAsync()' will not search in sub dirs
			ExecuteTask(sFiles, filesResult->GetFilesAsync());
			
			if (sFiles != nullptr) {
            total = sFiles->Size;
				for (auto it = 0; it != sFiles->Size; ++it) {
					auto sItem = sFiles->GetAt(it);
					if (sItem != nullptr) {
						files.push_back(StorageFileW(sItem));
					}
				}
			}
			delete sFiles;
			
         RARCH_DBG("Total files added (%d) in (%s)\n", files.size(), GetPath().c_str());

		return files;
	}

	// Get all sub folders (deep scan)
	std::list<StorageFolderW> GetAllFolders(bool useWindowsIndexer = false) {
		std::list<StorageFolderW> folders;

			IVectorView<StorageFolder^>^ sFolders;
         RARCH_DBG("Getting all folders for %s\n", GetPath().c_str());

			// Set query options to create groups of files within result
			QueryOptions^ queryOptions = ref new QueryOptions(CommonFolderQuery::DefaultQuery);
			queryOptions->FolderDepth = FolderDepth::Deep; // Search in all levels
			
			// Windows indexer is very bad, it will return missing results if the files recently copied
			// It's better to search without it, even if it will be slower
			queryOptions->IndexerOption = useWindowsIndexer ? IndexerOption::UseIndexerWhenAvailable : IndexerOption::DoNotUseIndexer;
			StorageFolderQueryResult^ foldersResult = storageFolder->CreateFolderQueryWithOptions(queryOptions);

			// Windows search query is slow but it's the only solution (or we need to build safe recursive function)
			// Regular 'StorageFolder->GetFoldersAsync()' will not search in sub dirs
			ExecuteTask(sFolders, foldersResult->GetFoldersAsync());
			if (sFolders != nullptr) {
            RARCH_DBG("Sub folders founded (%d) in (%s)\n", sFolders->Size, GetPath().c_str());
				for (auto it = 0; it != sFolders->Size; ++it) {
					auto sItem = sFolders->GetAt(it);
					if (sItem != nullptr) {
						folders.push_back(StorageFolderW(sItem));
					}
				}
			}
			delete sFolders;
         RARCH_DBG("Total folders added (%d) in (%s)\n", folders.size(), GetPath().c_str());

		return folders;
	}

	// Ensure item path without root
	std::string CleanItemPath(PathUWP& path) {
		std::string itemName = path.ToString();
		// Ensure slashs changed from '/' to '\'
		windowsPath(itemName);

		if (path.IsAbsolute()) {

			// If full path detected item in sub location,
			// root path must be removed
			replace(itemName, convert(storageFolder->Path + "\\"), "");
			replace(itemName, GetPath(), "");
		}

		// Do some fixes because 'TryGetItemAsync' is very sensetive 
		replace(itemName, "\\/", "\\");
		replace(itemName, "\\\\", "\\");
		replace(itemName, "//", "/");
		replace(itemName, "\\.\\", "\\");
		replace(itemName, "\\.", "\\");
		replace(itemName, ".\\", "\\");
		rtrim(itemName, ":"); // remove ':' at the end of the path (if any)
      ltrim(itemName, "\\"); // remove '\' at the start of the path (if any)

		path = PathUWP(itemName);

		return itemName;
	}

	// Check if folder contains item by name or path
	bool Contains(PathUWP path, IStorageItem^& storageItem) {
		auto pathString = CleanItemPath(path);

		// If the path is for parent then ignore
		if (!path.IsAbsolute()) {
         RARCH_DBG("Looking for (%s) in (%s)\n", pathString.c_str(), GetPath().c_str());
			ExecuteTask(storageItem, storageFolder->TryGetItemAsync(convert(pathString)));
		}

		return storageItem != nullptr;
	}

	bool Contains(std::string path) {
		IStorageItem^ tempItem;
		return Contains(PathUWP(path), tempItem);
	}

	void BuildStructure(StorageFolder^& folder, std::string path) {
		std::string folderName;
		std::vector<std::string> locationParts = split(path, '\\');
		for each (auto dir in locationParts) {
			folderName.append(dir);
			// Create folder
			ExecuteTask(folder, storageFolder->CreateFolderAsync(convert(folderName), CreationCollisionOption::OpenIfExists));
			folderName.append("\\");
		}
	}

   void BuildStructure(StorageFolder^& folder, std::string path, StorageFolder^ target, bool targetIsDest = false) {
      IStorageItem^ test = nullptr;
      ExecuteTask(test, target->TryGetItemAsync(convert(path)));
      if (test == nullptr) {
         std::string folderName;
         std::vector<std::string> locationParts = split(path, '\\');
         for each (auto dir in locationParts) {
            folderName.append(dir);
            // Create folder
            ExecuteTask(folder, target->CreateFolderAsync(convert(folderName), CreationCollisionOption::OpenIfExists));
            folderName.append("\\");
         }
      }
      else {
         folder = (StorageFolder^)test;
      }
   }

	StorageFolder^ GetOrCreateFolder(PathUWP path) {
		StorageFolder^ folder;
		auto pathString = CleanItemPath(path);
		BuildStructure(folder, pathString);

		return folder;
	}

	StorageFile^ GetOrCreateFile(PathUWP path) {
		StorageFile^ file;
		StorageFolder^ folder;
		auto name = path.GetFilename();
		auto dir = PathUWP(path.GetDirectory());
		auto dirString = CleanItemPath(dir);
		BuildStructure(folder, dirString);
		if (folder != nullptr) {
			ExecuteTask(file, folder->CreateFileAsync(convert(name), CreationCollisionOption::OpenIfExists));
		}
		return file;

	}

	// Copy to another folder
	bool Copy(StorageFolderW folder, bool move = false, bool deepScan = false, retro_task_t* copyTask = nullptr, bool targetIsDest = false) {
      int total = 0;
      auto files = deepScan ? GetAllFiles(total) : GetFiles(total);
		bool state = false;
		int failedCount = 0;
		if (!files.empty()) {
			auto destination = folder.GetStorageFolder();

			// Copy files one by one to avoid 'access violation' issues with deep-level tasks
			std::string rootName = convert(storageFolder->Name);
			std::string rootPath = PathUWP(GetPath()).GetDirectory();
         windowsPath(rootPath);

			if (destination != nullptr) {
            auto indexer = 0;
				for each (auto file in files) {
               indexer++;
               if (copyTask != nullptr) {
                  auto precentage = ((double)indexer / (double)total) * 100.0;
                  //char str[1024];
                  //sprintf(str, "Copy %d of %d", indexer, (int)total);
                  //task_set_title(copyTask, str);
                  task_set_progress(copyTask, precentage);
               }
					auto fItem = file.GetStorageFile();

					// Get file full path
					std::string targetLocation = convert(fItem->Path);
					// Remove root path but keep the parent name
					replace(targetLocation, rootPath, "");
					replace(targetLocation, convert("\\" + fItem->Name), "");
               ltrim(targetLocation, "\\");
               rtrim(targetLocation, "\\");

					// Build folder structure
					StorageFolder^ targetFolder;
					BuildStructure(targetFolder, targetLocation, destination, targetIsDest);

					if (targetFolder != nullptr) {
						// Copy file
						StorageFile^ testFile;
						if (move) {
							ExecuteTask(fItem->MoveAsync((IStorageFolder^)targetFolder, fItem->Name, NameCollisionOption::ReplaceExisting));
							ExecuteTask(testFile, targetFolder->GetFileAsync(fItem->Name)); // testing, it can be ignored
						}
						else {
							ExecuteTask(testFile, fItem->CopyAsync((IStorageFolder^)targetFolder, fItem->Name, NameCollisionOption::ReplaceExisting));
						}

						if (testFile == nullptr) {
							// File failed to copy, we can handle this later
							failedCount++;
						}
					}
				}
				// Try to get the new folder
				IStorageItem^ newFolder;
				ExecuteTask(newFolder, destination->TryGetItemAsync(convert(rootName)));
				if (newFolder != nullptr) {
					if (move) {
						if (failedCount == 0) {
							// If all files moved, we can safely remove the folder
							ExecuteTask(storageFolder->DeleteAsync());
						}
						storageFolder = (StorageFolder^)newFolder;
					}
					state = true;
				}
			}
		}
		return state;
	}

	// Copy to another folder using StorageFolder^
	bool Copy(StorageFolder^ folder, bool move = false, bool deepScan = false, retro_task_t* copyTask = nullptr) {
		return Copy(StorageFolderW(folder), move, deepScan, copyTask);
	}

	// Move to another folder
	bool Move(StorageFolderW destination) {
		return Copy(destination, true);
	}

	// Get storage folder handle
	HRESULT GetHandle(HANDLE* handle, int accessMode = GENERIC_READ, int shareMode = FILE_SHARE_READ) {
		return GetFolderHandle(storageFolder, handle, GetAccessMode(accessMode),GetShareMode(shareMode));
	}

	// Get storage folder handle
	HRESULT GetHandleForFile(HANDLE* handle, std::string filename, int accessMode = GENERIC_READ, int shareMode = FILE_SHARE_READ, int openMode = OPEN_EXISTING) {
		return GetFileHandleFromFolder(storageFolder, filename, handle, GetAccessMode(accessMode), GetShareMode(shareMode), GetOpenMode(openMode));
	}

   // Get file stream
   FILE* GetFileStream(std::string name, const char* mode) {
      FILE* file{};

      auto fileMode = GetFileMode(mode);
      bool createIfNotExists = fileMode->isCreate;
      bool isAppend = fileMode->isAppend;

      StorageFile^ sfile;

      if (createIfNotExists) {
         auto createMode = isAppend ? CreationCollisionOption::OpenIfExists : CreationCollisionOption::ReplaceExisting;
         ExecuteTask(sfile, storageFolder->CreateFileAsync(convert(name), CreationCollisionOption::OpenIfExists));
      }
      else {
         IStorageItem^ tempItem;
         ExecuteTask(tempItem, storageFolder->TryGetItemAsync(convert(name)));
         sfile = (StorageFile^)tempItem;
      }

      StorageFileW storageFile(sfile);
      if (storageFile.IsValid()) {
         file = storageFile.GetStream(mode);
      }

      return file;
   }

	// Get folder size
	__int64 GetSize(bool updateCache = false) {
		if (folderSize == 0 || updateCache) {
			// Let's try getting size by handle first
			HANDLE handle;
			HRESULT hr = GetHandle(&handle);
			if (handle == INVALID_HANDLE_VALUE || hr != S_OK) {
				// We have no other option, fallback to UWP
				// This need to sum all files inside
            /*int total = 0;
				auto files = GetAllFiles(total, true);
				for each (auto file in files) {
					folderSize += file.GetSize();
				}*/
            // Don't try to fetch using UWP, mostly will stuck
            folderSize = 1; // Anything
			}
			else {
				LARGE_INTEGER size{ 0 };
				if (FALSE == GetFileSizeEx(handle, &size)) {
					LARGE_INTEGER end_offset;
					const LARGE_INTEGER zero{};
					if (SetFilePointerEx(handle, zero, &end_offset, FILE_END) == 0) {
						CloseHandle(handle);
					}
					else {
						folderSize = end_offset.QuadPart;
						SetFilePointerEx(handle, zero, nullptr, FILE_BEGIN);
						CloseHandle(handle);
					}
				}
				else {
					folderSize = size.QuadPart;
					CloseHandle(handle);
				}
			}
			
		}
		return folderSize;
	}

	// Get folder basic properties
	FILE_BASIC_INFO* GetProperties() {
		HANDLE handle;
		HRESULT hr = GetHandle(&handle);

		size_t size = sizeof(FILE_BASIC_INFO);
		FILE_BASIC_INFO* information = (FILE_BASIC_INFO*)(malloc(size));
		if (hr == S_OK && handle != INVALID_HANDLE_VALUE && information) {
			information->FileAttributes = (DWORD)storageFolder->Attributes;

			if (FALSE == GetFileInformationByHandleEx(handle, FileBasicInfo, information, (DWORD)size)) {
				// Fallback to UWP method (Slow)
				auto props = FetchProperties();
				information->ChangeTime.QuadPart = props->DateModified.UniversalTime;
				information->CreationTime.QuadPart = props->ItemDate.UniversalTime;
				information->LastAccessTime.QuadPart = props->DateModified.UniversalTime;
				information->LastWriteTime.QuadPart = props->DateModified.UniversalTime;
			}
			CloseHandle(handle);
		}

		return information;
	}

	// Get main storage folder
	StorageFolder^ GetStorageFolder() {
		return storageFolder;
	}


    time_t  filetime_to_timet(LARGE_INTEGER ull) const {
		return ull.QuadPart / 10000000ULL - 11644473600ULL;
	}
	ItemInfoUWP GetFolderInfo() {
		ItemInfoUWP info;
		info.name = GetName();
		info.fullName = GetPath();
		info.isDirectory = true;

		
		auto sProperties = GetProperties();

		info.size = (uint64_t)GetSize();
		info.lastAccessTime = (uint64_t)filetime_to_timet(sProperties->LastAccessTime);
		info.lastWriteTime = (uint64_t)filetime_to_timet(sProperties->LastWriteTime);
		info.changeTime = (uint64_t)filetime_to_timet(sProperties->ChangeTime);
		info.creationTime = (uint64_t)filetime_to_timet(sProperties->CreationTime);


		info.attributes = sProperties->FileAttributes;

		return info;
	}

private:
	StorageFolder^ storageFolder;
	BasicProperties^ properties;
	__int64 folderSize = 0;

	BasicProperties^ FetchProperties() {
		if (properties == nullptr) {
			// Very bad and slow way in UWP to get size and other properties
			// not preferred to be used on big list of files
			ExecuteTask(properties, storageFolder->GetBasicPropertiesAsync());
		}
		return properties;
	}
};
