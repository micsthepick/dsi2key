#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "checkButton.h"

namespace D2K {namespace GUI {

class RadioButton : public CheckButton
{
public:
	RadioButton(const std::string& text, int x, int y, int width, int height);
	~RadioButton();
	bool Attach(Object *parentObject);
};

}}//namespace D2K::GUI
