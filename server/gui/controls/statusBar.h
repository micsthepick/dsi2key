#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {namespace GUI {

class StatusBar : public Label
{
public:
	StatusBar(const std::string& text, int x, int y, int width, int height);
	~StatusBar();
	bool Attach(Object *parentObject);
	void SetText(const std::string& Text);
};

}}//namespace D2K::GUI
