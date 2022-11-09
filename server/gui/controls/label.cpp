/*
Windows GUI label control
*/

#include <windows.h>
#include <commctrl.h>
#include "label.h"
#include "gui/gui.h"
#include "gui/resource.h"

namespace D2K {namespace GUI {

Label::Label(const std::string& text, int x, int y, int width, int height) : Object(x, y, width, height)
{
	Text = text.c_str();
}
Label::~Label() { }
bool Label::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		hwnd = CreateWindow (
					"STATIC", Text,
					WS_CHILD | WS_VISIBLE | BS_TEXT,
					X, Y, Width, Height,
					GetParentHWND(),
					(HMENU)ID,
					GetModuleHandle(NULL_VALUE),
					0);
		D2K::GUI::SetFont(hwnd);
		return true;
	}
	return false;
}
std::string Label::GetText()
{
	int length = GetWindowTextLength(hwnd);
	char *windowText = new char[length+1];
	GetWindowText(hwnd, windowText, length+1);
	windowText[length] = 0;
	Text = windowText;
	//delete windowText;
	return Text;
}
void Label::SetText(const std::string& text)
{
	Text = text.c_str();
	locked = true;
	SetWindowText(hwnd, Text);
	locked = false;
}
void Label::SetEnabled(bool enabled)
{
	EnableWindow(hwnd, enabled);
}
long Label::GetStyle()
{
	return GetWindowLong(hwnd, GWL_STYLE);
}
long Label::SetStyle(long style)
{
	return SetWindowLong(hwnd, GWL_STYLE, style);
}

}}//namespace D2K::GUI
