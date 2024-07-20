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

#include <ppl.h>
#include <ppltasks.h>
#include <wrl.h>
#include <wrl/implements.h>

#include "StorageLog.h"
#include "StorageExtensions.h"

using namespace Windows::UI::Core;

// Don't add 'using' 'Windows::Foundation'
// it might cause confilct with some types like 'Point'

#pragma region Async Handlers

template<typename T>
T TaskHandler(std::function<concurrency::task<T>()> wtask, T def)
{
	T result = def;
	bool done = false;
	wtask().then([&](concurrency::task<T> t) {
		try
	    {
		result = t.get();
	    }
	    catch (Platform::Exception^ exception_)
	    {
			 RARCH_ERR(convertToChar(exception_->Message+"\n"));
	    }
		done = true;
	});

	CoreWindow^ corewindow = CoreWindow::GetForCurrentThread();
	while (!done)
	{
		try {
			if (corewindow) {
				corewindow->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			}
			else {
				corewindow = CoreWindow::GetForCurrentThread();
			}
		}
		catch (...) {

		}
	}

	return result;
};

template<typename T>
T TaskPass(Windows::Foundation::IAsyncOperation<T>^ task, T def)
{
   try {
      return TaskHandler<T>([&]() {
         return concurrency::create_task(task).then([](T res) {
            return res;
            });
         }, def);
   }
   catch (...) {
      return def;
   }
}

bool ActionPass(Windows::Foundation::IAsyncAction^ action);

#pragma endregion

// Now it's more simple to execute async task
// @out: output variable
// @task: async task
template<typename T>
void ExecuteTask(T& out, Windows::Foundation::IAsyncOperation<T>^ task)
{
	out = TaskPass<T>(task, T());
};

// For specific return default value
// @out: output variable
// @task: async task
// @def: default value when fail
template<typename T>
void ExecuteTask(T& out, Windows::Foundation::IAsyncOperation<T>^ task, T def)
{
	out = TaskPass<T>(task, def);
};


// Async action such as 'Delete' file
// @action: async action
// return false when action failed
bool ExecuteTask(Windows::Foundation::IAsyncAction^ action);
