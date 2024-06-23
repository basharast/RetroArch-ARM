// UWP STORAGE MANAGER
// Copyright (c) 2023 Bashar Astifan.
// Email: bashar@astifan.online
// Telegram: @basharastifan
// GitHub: https://github.com/basharast/UWP2Win32

// This code must keep support for lower builds (15063+)
// Try always to find possible way to keep that support

#pragma once

#include "../verbosity.h"
#include "../libretro-common/include/queues/task_queue.h"

#define UWPSMT = 4; // Change it based on your needs

// Link your debug function below
#define UWP_ERROR_LOG(t,...)   do {  } while (false)
#define UWP_WARN_LOG(t,...)    do {  } while (false)
#define UWP_NOTICE_LOG(t,...)  do {  } while (false)
#define UWP_INFO_LOG(t,...)    do {  } while (false)
#define UWP_DEBUG_LOG(t,...)   do {  } while (false)
#define UWP_VERBOSE_LOG(t,...) do {  } while (false)
