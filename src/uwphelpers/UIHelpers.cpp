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

#include "UIHelpers.h"

using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;

#pragma region Input Keyboard
void ShowInputKeyboard() {
	InputPane::GetForCurrentView()->TryShow();
}

void HideInputKeyboard() {
	InputPane::GetForCurrentView()->TryHide();
}
#pragma endregion

#pragma region Keys Status
bool IsCapsLockOn() {
	auto capsLockState = CoreApplication::MainView->CoreWindow->GetKeyState(VirtualKey::CapitalLock);
	return (capsLockState == CoreVirtualKeyStates::Locked);
}
bool IsShiftOnHold() {
	auto shiftState = CoreApplication::MainView->CoreWindow->GetKeyState(VirtualKey::Shift);
	return (shiftState == CoreVirtualKeyStates::Down);
}
bool IsCtrlOnHold() {
	auto ctrlState = CoreApplication::MainView->CoreWindow->GetKeyState(VirtualKey::Control);
	return (ctrlState == CoreVirtualKeyStates::Down);
}
#pragma endregion

#pragma region Notifications
void ShowToastNotification(std::string title, std::string message) {
	ToastNotifier^ toastNotifier = ToastNotificationManager::CreateToastNotifier();
	XmlDocument^ toastXml = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastText02);
	XmlNodeList^ toastNodeList = toastXml->GetElementsByTagName("text");
	toastNodeList->Item(0)->AppendChild(toastXml->CreateTextNode(convert(title)));
	toastNodeList->Item(1)->AppendChild(toastXml->CreateTextNode(convert(message)));
	IXmlNode^ toastNode = toastXml->SelectSingleNode("/toast");
	XmlElement^ audio = toastXml->CreateElement("audio");
	audio->SetAttribute("src", "ms-winsoundevent:Notification.SMS");
	ToastNotification^ toast = ref new ToastNotification(toastXml);
	toastNotifier->Show(toast);
}
#pragma endregion


