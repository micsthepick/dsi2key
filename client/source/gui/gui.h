#pragma once

#include <cstdint>
#include <string> // std::string
#include "common/screen_size.h"
#include "rect.h"
#include "ttf_format.h"

namespace D2K {namespace GUI {

#if defined(__3DS__)
#define ARGB16(a, r, g, b) (((a) << 15) | (r) | ((g) << 5) | ((b) << 10))
#endif

extern uint16_t* g_screen[2];

void VoidFunction();
void SetBackground(const std::string& text);
std::string GetBackground();
void SetFont(uint8_t font_face, const std::string& text);
std::string GetFont(uint8_t font_face);
bool DrawBackgroundImage(uint8_t screen, uint8_t c);
bool DrawBackgroundImage(uint8_t screen, GUI::Rect rect, uint8_t c);
bool LoadButtonImage();
bool DrawButtonImage(uint8_t screen, const std::string& letter, uint16_t x, uint16_t y);
void SetUpdate(bool value);
bool IsUpdated();
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t color);
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t color, uint8_t alpha);
void ClearScreen(uint8_t screen, uint16_t color);
void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawLine(uint8_t screen, const std::string& text, uint16_t x, uint16_t y, uint16_t c);
void DrawLetter(uint8_t screen, char letter, uint16_t x, uint16_t y, uint16_t c);
void DrawString(uint8_t screen, const std::string& text, uint8_t font_size, uint8_t face_type, uint16_t x, uint16_t y, uint16_t c);
enum { 	COLOR_BACKGROUND,
		COLOR_LABEL_TEXT, COLOR_LABEL_BACKGROUND,
		COLOR_BUTTON_TEXT, COLOR_BUTTON_BACKGROUND, COLOR_BUTTON_OUTLINE, COLOR_BUTTON_OUTLINE_ACTIVE,
		COLOR_EDIT_TEXT, COLOR_EDIT_BACKGROUND, COLOR_EDIT_OUTLINE, COLOR_EDIT_OUTLINE_ACTIVE,
		colorMax};
extern uint16_t Color[colorMax];

}} // namespace D2K::GUI
