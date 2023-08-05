#pragma once

#include "../gui/window.h"

namespace D2K {namespace GUI {namespace Bar {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	bool Update();
};
extern void SetText(const std::string& Text);
extern GUI::Bar::WindowClass g_window;

}}} // namespace D2K::GUI::Main
