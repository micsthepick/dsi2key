#pragma once

#include <windows.h>
#include <cstring>
#include <string>//std::string
#include "object.h"

namespace D2K {namespace GUI {

class Label : public Object
{
public:
	Label(const std::string& text, int x, int y, int width, int height);
	virtual ~Label();
	bool Attach(Object *parentObject);
	std::string GetText();
	void SetText(const std::string& text);
	void SetEnabled(bool enabled);
	long GetStyle();
	long SetStyle(long style);
protected:
	const char* Text;
};

}}//namespace D2K::GUI
