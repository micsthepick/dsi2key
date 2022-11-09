#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"

namespace D2K {namespace GUI {

class Window : public Object
{
public:
	Window();
	~Window();
	void Append(Object *object, void (*function)(HWND, UINT, WPARAM, LPARAM));
	void Append(Object *object);
	bool Setup(HINSTANCE hThisInstance, const std::string& ClassName, const std::string& Title, int Width, int Height);
	void SetVisible(int visible);
	void SetVisible(bool visible = true);
	void CreateMainMenu();
	void SetText(const std::string & text);

private:
	std::string Text;
};

}}//namespace D2K::GUI
