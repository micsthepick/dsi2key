/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include "gui/gui.h"
#include "gui/resource.h"
#include "trayIcon.h"

namespace D2K {namespace GUI {

TrayIcon::TrayIcon(const std::string& text) : Label(text, 0, 0, 0, 0)
{
	ZeroMemory(&niData, sizeof(NOTIFYICONDATA));
	niData.cbSize = NOTIFYICONDATA_V1_SIZE;
	niData.uID = ID;//tray icon identifier
	niData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	SetText(text);
	niData.hIcon = LoadIcon(GUI::hThisInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	niData.uCallbackMessage = WM_TRAYMSG;
}
TrayIcon::~TrayIcon()
{
	Delete();
}
bool TrayIcon::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		niData.hWnd = GetParentHWND();
		Shell_NotifyIcon(NIM_ADD, &niData);
		return true;
	}
	return false;
}
void TrayIcon::SetText(const std::string& text)
{
	this->Text = text.c_str();
	ZeroMemory(&niData.szTip, 64);
	strncpy(niData.szTip, this->Text, 63);

	return;
}
void TrayIcon::Delete()
{
	Shell_NotifyIcon(NIM_DELETE, &niData);
}

}}//namespace D2K::GUI
