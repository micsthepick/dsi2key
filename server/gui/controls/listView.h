#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {namespace GUI {
class ListView : public Label
{
public:
	ListView(const std::string& text, int x, int y, int width, int height);
	~ListView();
	unsigned GetColumnCount();
	void SetHeaderVisible(bool visible);
	void InsertColumn(const std::string& text, int i);
	std::string GetText(int row, int column);
	void SetText(const std::string& text, int row, int column);
	void SetDoubleClick(void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
	void AutoSizeColumns();
	unsigned GetSelection();
	void SetSelection(unsigned row);
	void Append(const std::string& text);
	void Append(const std::string& text, const std::string& text2);
	bool Attach(Object *parentObject);
};

}}//namespace D2K::GUI
