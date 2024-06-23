// UWP UI HELPER
// Copyright (c) 2023 Bashar Astifan.
// Email: bashar@astifan.online
// Telegram: @basharastifan
// GitHub: https://github.com/basharast/UWP2Win32

// Functions:
// SendKeyToTextEdit(vKey, flags, state) [state->KEY_DOWN or KEY_UP]
// ShowInputKeyboard()
// HideInputKeyboard()
// IsCapsLockOn()
// IsShiftOnHold()
// IsCtrlOnHold()

#include "StorageExtensions.h"

// Input Handler
void ShowInputKeyboard();
void HideInputKeyboard();


// Keys Status
bool IsCapsLockOn();
bool IsShiftOnHold();
bool IsCtrlOnHold();

// Notifications
void ShowToastNotification(std::string title, std::string message);
