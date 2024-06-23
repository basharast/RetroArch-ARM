// UWP UI HELPER
// Copyright (c) 2023 Bashar Astifan.
// Email: bashar@astifan.online
// Telegram: @basharastifan
// GitHub: https://github.com/basharast/UWP2Win32

// Functions:
// ShowInputKeyboard()
// HideInputKeyboard()
// IsCapsLockOn()
// IsShiftOnHold()
// IsCtrlOnHold()

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


