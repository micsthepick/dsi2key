#include <string> // std::string
#include "button.h"
#include "gui.h"

namespace D2K {namespace GUI {

Button::Button(uint8_t screen, GUI::Rect rect, const std::string& text, void (*function)()) : Label(screen, rect, text)
{
	GUI::Rect thisRect = GetRect();
	int width = text.length() * 6 + 4; // Calculate width of "const std::string& text"

	if(thisRect.GetW() < width)        // If button isn't big enough
		thisRect.SetW(width);      // Adjust it to text's length
	SetRect(thisRect);                 // Then reassign the Rect

	Button::Function = function;
}

Button::~Button() { }

bool Button::Draw()
{
	if(!Object::Draw())
		return false;

	if(IsVisible())
	{
		Clear(Color[COLOR_BUTTON_BACKGROUND]);

		if(GetStatus() == OBJECT_STATUS::HELD) // If we're active/clicked
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_OUTLINE_ACTIVE]);
		else                                   // If we're idle
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_OUTLINE]);

		DrawString(GetScreen(), GetText(), TTF::FONT_SIZE_REGULAR, TTF::FONT_REGULAR,
			GetRect().GetX() + 3, GetRect().GetY() + 3, Color[COLOR_BUTTON_TEXT]);
	}

	return true;
}

}} // namespace D2K::GUI
