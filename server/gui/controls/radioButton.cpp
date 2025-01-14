/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include "gui/gui.h"
#include "gui/resource.h"
#include "radioButton.h"

namespace D2K {namespace GUI {

RadioButton::RadioButton(const std::string& text, int x, int y, int width, int height) : CheckButton(text, x, y, width, height) { }
RadioButton::~RadioButton() { }
bool RadioButton::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		hwnd = CreateWindow (
					"BUTTON", Text,
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
					X, Y, Width, Height,
					GetParentHWND(),
					(HMENU)ID,
					GetModuleHandle(NULL_VALUE),
					0);
		SetFont(hwnd);
		return true;
	}
	return false;
}

}}//namespace D2K::GUI
