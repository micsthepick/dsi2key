#pragma once

#include "../gui/window.h"
#include "../../../common/udp.h"

namespace D2K {namespace GUI {namespace Connection {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	void SetVisible(bool visible);
	bool Update();
};
void ProcessCommandSettingsPacket(UDP::DS2KeyCommandSettingsPacket settings);
extern GUI::Connection::WindowClass g_window;

}}} // namespace D2K::GUI::Command
