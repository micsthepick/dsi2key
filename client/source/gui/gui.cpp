#ifdef _NDS
#include <nds/arm9/video.h> // SCREEN_WIDTH
#include <nds/dma.h>        // dmaFillHalfWords
#endif
#include <algorithm>        // std::min, std::max
#include <cstring>
#include "gui.h"
#include "png_format.h"

namespace D2K {namespace GUI {

#if defined(_NDS)
const uint16_t MAX_X = SCREEN_WIDTH;
const uint16_t MAX_Y = SCREEN_HEIGHT;
const uint8_t SCREEN_BYTES = 2;
#elif defined(_3DS)
const uint16_t MAX_X = _3DS_SCREEN_WIDTH;
const uint16_t MAX_Y = _3DS_SCREEN_HEIGHT;
const uint8_t SCREEN_BYTES = 3;
#endif
const uint8_t IMAGE_BYTES = 3;
uint16_t* g_screen[2];
bool Update = false;
uint16_t Color[colorMax];
uint8_t alpha_setting = 70;
void VoidFunction() { }

std::string background_filename{ };
void SetBackground(const std::string& text)
{
	background_filename = text;
}
std::string GetBackground()
{
	return background_filename;
}

uint8_t* GetScreenPointer(uint8_t screen, uint16_t x, uint16_t y)
{
	uint8_t* screen_pointer = (uint8_t*)GUI::g_screen[screen];
#if defined(_NDS)
	screen_pointer += (x + (y * SCREEN_WIDTH)) * SCREEN_BYTES;
#elif defined(_3DS)
	screen_pointer += (x * _3DS_SCREEN_HEIGHT + (_3DS_SCREEN_HEIGHT - 1 - y)) * SCREEN_BYTES;
#endif
	return screen_pointer;
}

void RGB15TORGB24(uint16_t color, uint8_t &red, uint8_t &green, uint8_t &blue)
{
	red   =  color & 0x001f        << 3;
	green = (color & 0x3E0)  >> 5  << 3;
	blue  = (color & 0x7C00) >> 10 << 3;
}
uint16_t RGB24TORGB15(uint8_t red, uint8_t green, uint8_t blue)
{
	return ARGB16(1, (red >> 3), (green >> 3), (blue >> 3));
}

char* background_image{ };
int background_image_size{ };
//TODO:handle deallocation of background_image
//TODO:seperate cropping function
//TODO:logs
bool LoadBackgroundImage()
{
	int png_width, png_height;
	char* png_image{ };
	if(LoadPngImage(GetBackground(), png_width, png_height, (unsigned char**)&png_image))
	{
		int background_height = MAX_Y;
		int background_width  = MAX_X;
		if(background_width > png_width)
			background_width = png_width;
		if(background_height > png_height)
			background_height = png_height;
		if(background_image == nullptr)
		{
			background_image_size = background_width * background_height * IMAGE_BYTES;
			background_image = (char*)malloc(background_image_size);
		}
		
		for(int x = 0; x < background_width; x++)
		{
			for(int y = 0; y < background_height; y++)
			{
				int background_memory_position = (x * MAX_Y + (MAX_Y - 1 - y)) * IMAGE_BYTES;
				int png_memory_position = (x + (y * png_width)) * IMAGE_BYTES;
				background_image[background_memory_position + 0] = png_image[png_memory_position + 0];
				background_image[background_memory_position + 1] = png_image[png_memory_position + 1];
				background_image[background_memory_position + 2] = png_image[png_memory_position + 2];
			}
		}
		free(png_image);
		return true;
	}
	else if(background_image == nullptr)
	{
		background_image_size = MAX_X * MAX_Y * IMAGE_BYTES;
		background_image = (char*)malloc(background_image_size);

		for(int x = 0; x < MAX_X; x++)
		{
			for(int y = 0; y < MAX_Y; y++)
			{
				int background_memory_position = (x * MAX_Y + (MAX_Y - 1 - y)) * IMAGE_BYTES;
				int png_memory_position = (x + (y * png_width)) * IMAGE_BYTES;
				uint8_t blue, green, red = {};
				RGB15TORGB24(Color[COLOR_BACKGROUND], red, green, blue);
				
				background_image[background_memory_position + 0] = red;
				background_image[background_memory_position + 1] = green;
				background_image[background_memory_position + 2] = blue;
			}
		}
	}
	return false;
}
bool DrawBackgroundImage(uint8_t screen, GUI::Rect rect, uint8_t color)
{
	if(background_image == nullptr)
		LoadBackgroundImage();

	if(background_image)
	{
		for(int x = rect.GetX(); x <= rect.GetX2(); x++)
		{
			for(int y = rect.GetY(); y <= rect.GetY2(); y++)
			{
				int background_memory_position = (x * MAX_Y + (MAX_Y - 1 - y)) * 3;

				SetPixel(screen, x, y, background_image[background_memory_position + 0], background_image[background_memory_position + 1], background_image[background_memory_position + 2]);
			}
		}
		return true;
	}
	else
	{
		for(int x = rect.GetX(); x <= rect.GetX2(); x++)
		{
			for(int y = rect.GetY(); y <= rect.GetY2(); y++)
			{
				SetPixel(screen, x, y, color);
			}
		}
		return false;
	}
}
bool DrawBackgroundImage(uint8_t screen, uint8_t color)
{
	if(background_image == nullptr)
		LoadBackgroundImage();

	if(background_image)
	{
		GUI::Rect rect = { 0, 0, MAX_X-1, MAX_Y-1 };
		DrawBackgroundImage(screen, rect, color);
		return true;
	}
	else
	{
//TODO: Fix when background fails to load
		for(int x = 0; x < MAX_X; x++)
		{
			for(int y = 0; y < MAX_Y; y++)
			{
				SetPixel(screen, x, y, color);
			}
		}
		return false;
	}
}

void SetUpdate(bool value)
{
	Update = value;
}
bool IsUpdated()
{
	return Update;
}

void GetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t& color)
{
	uint8_t* screen_pointer = GetScreenPointer(screen, x, y);
#if defined(_NDS)
	color = (uint16_t)screen_pointer[0];
#elif defined(_3DS)
	color = RGB24TORGB15(screen_pointer[0], screen_pointer[1], screen_pointer[2]);
#endif
}
void GetPixel(uint8_t screen, uint16_t x, uint16_t y, uint8_t& red, uint8_t& green, uint8_t& blue)
{
	uint8_t* screen_pointer = GetScreenPointer(screen, x, y);
#if defined(_NDS)
	RGB15TORGB24(((uint16_t*)screen_pointer)[0], red, green, blue);
#elif defined(_3DS)
	blue  = screen_pointer[0];
	green = screen_pointer[1];
	red   = screen_pointer[2];
#endif
}

uint8_t AlphaBlend2(uint8_t color_1, uint8_t color_2, uint8_t alpha)
{
	if (color_1 >= color_2)
		return (color_1-color_2) * (alpha*0.01) + color_2;
	else
		return (color_2-color_1) * ((100-alpha)*0.01) + color_1;
}
uint8_t AlphaBlend(uint8_t color_1, uint8_t color_2, uint8_t alpha)
{
	return (alpha  * color_1 + (100 - alpha) * color_2) / 100;
}
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t color)
{
	if((x < MAX_X) // if we're drawing on screen
	&& (y < MAX_Y))
	{
#if defined(_NDS)
		uint16_t* screen_pointer = (uint16_t*)GetScreenPointer(screen, x, y);

		screen_pointer[0] = color;
#elif defined(_3DS)
		uint8_t blue, green, red = {};
		RGB15TORGB24(color, red, green, blue);
		
		SetPixel(screen, x, y, red, green, blue);
#endif
	}
}
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if((x < MAX_X) // if we're drawing on screen
	&& (y < MAX_Y))
	{
#if defined(_3DS)
		uint8_t* screen_pointer = GetScreenPointer(screen, x, y);

		screen_pointer[0] = blue;
		screen_pointer[1] = green;
		screen_pointer[2] = red;
#elif defined(_NDS)
		SetPixel(screen, x, y, RGB24TORGB15(red, green, blue));
#endif
	}
}
//TODO: add errors or fatals if the boundary checks fail
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t color, uint8_t alpha)
{
	if((x < MAX_X) // if we're drawing on screen
	&& (y < MAX_Y))
	{
		if(alpha >= 100 || background_image == nullptr)
			SetPixel(screen, x, y, color);
		else if(alpha == 0)
			return;

		uint8_t color_blue, color_green, color_red = {};
		RGB15TORGB24(color,
		             color_red,
		             color_green,
		             color_blue);

		uint8_t screen_pointer_blue, screen_pointer_green, screen_pointer_red = {};
		GetPixel(screen, x, y,
		         screen_pointer_red,
		         screen_pointer_green,
		         screen_pointer_blue);

		SetPixel(screen, x, y,
		         AlphaBlend(color_red,   screen_pointer_red  , alpha),
		         AlphaBlend(color_green, screen_pointer_green, alpha),
		         AlphaBlend(color_blue,  screen_pointer_blue , alpha));
	}
}

void ClearScreen(uint8_t screen, uint16_t color)
{
	DrawBackgroundImage(screen, color);
}
//TODO: add errors or fatals if the boundary checks fail
void DrawFastHorizontleLine(uint8_t screen, uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
	if(x >= MAX_X
	|| y >= MAX_Y)
		return;
	if(x + w >= MAX_X)
	{
		if(MAX_X - x > 0)
			w = MAX_X - x;
		else
			return;
	}
	if(w == 0)
		return;
#if defined(_NDS)
	uint16_t* screen_pointer = (uint16_t*)GetScreenPointer(screen, x, y);
	dmaFillHalfWords(color, &screen_pointer[0], w * SCREEN_BYTES);
#elif defined(_3DS)
	for(int i = 0; i < w; i++)
	{
		SetPixel(screen, x + i, y, color);
	}
#endif
}
void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c)
{
	for(int y = rect.GetY(); y <= rect.GetY2(); y++)
	{
		SetPixel(screen, rect.GetX(), y, c);
		SetPixel(screen, rect.GetX2(), y, c);
	}

	DrawFastHorizontleLine(screen, rect.GetX(), rect.GetY(), rect.GetW(), c);
	DrawFastHorizontleLine(screen, rect.GetX(), rect.GetY2(), rect.GetW(), c);
}
void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c)
{
	for(int y = rect.GetY(); y <= rect.GetY2(); y++)
	{
#if defined(_NDS)
		DrawFastHorizontleLine(screen, rect.GetX(), y, rect.GetW(), c);
#elif defined(_3DS)
		for(int x = rect.GetX(); x <= rect.GetX2(); x++)
		{
			SetPixel(screen, x, y, c, alpha_setting);
		}
#endif
	}
}
void DrawLine(uint8_t screen, std::string text, uint16_t x, uint16_t y, uint16_t c)
{
	if (text == "X")
	{
		SetPixel(screen, x, y, c);
	}
	else if(text == "XX")
	{
		DrawFastHorizontleLine(screen, x, y, 2, c);
	}
	else if(text == " XX")
	{
		DrawFastHorizontleLine(screen, 1 + x, y, 2, c);
	}
	else if(text == "  XX")
	{
		DrawFastHorizontleLine(screen, 2 + x, y, 2, c);
	}
	else if(text == "   XX")
	{
		DrawFastHorizontleLine(screen, 3 + x, y, 2, c);
	}
	else if(text == "XXX")
	{
		DrawFastHorizontleLine(screen, x, y, 3, c);
	}
	else if(text == " XXX")
	{
		DrawFastHorizontleLine(screen, 1 + x, y, 3, c);
	}
	else if(text == "  XXX")
	{
		DrawFastHorizontleLine(screen, 2 + x, y, 3, c);
	}
	else if(text == "XXXX")
	{
		DrawFastHorizontleLine(screen, x, y, 4, c);
	}
	else if(text == " XXXX")
	{
		DrawFastHorizontleLine(screen, 1 + x, y, 4, c);
	}
	else if(text == "XXXXX")
	{
		DrawFastHorizontleLine(screen, x, y, 5, c);
	}
	else
	{
		for(unsigned int i = 0; i < text.length(); i++)
		{
			if(text.at(i) != ' ')
				SetPixel(screen, x + i, y, c);
		}
	}
}
void DrawLetter(uint8_t screen, char letter, uint16_t x, uint16_t y, uint16_t c)
{
	switch(letter)
	{
	case 'A':
	case 'a':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'B':
	case 'b':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "XXXX",  	x, y+4, c);
		break;
	}
	case 'C':
	case 'c':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X",		x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'D':
	case 'd':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X   X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "XXXX",	x, y+4, c);
		break;
	}
	case 'E':
	case 'e':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "XXXXX",	x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case 'F':
	case 'f':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "XXX",		x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "X",		x, y+4, c);
		break;
	}
	case 'G':
	case 'g':
	{
		DrawLine(screen, " XXXX",	x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "X  XX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'H':
	case 'h':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'I':
	case 'i':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "  X",		x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case 'J':
	case 'j':
	{
		DrawLine(screen, "    X",	x, y+0, c);
		DrawLine(screen, "    X",	x, y+1, c);
		DrawLine(screen, "    X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'K':
	case 'k':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X  X",	x, y+1, c);
		DrawLine(screen, "XXX",		x, y+2, c);
		DrawLine(screen, "X  X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'L':
	case 'l':
	{
		DrawLine(screen, "X",		x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "X",		x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case 'M':
	case 'm':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "XX XX",	x, y+1, c);
		DrawLine(screen, "X X X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'N':
	case 'n':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "XX  X",	x, y+1, c);
		DrawLine(screen, "X X X",	x, y+2, c);
		DrawLine(screen, "X  XX",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'O':
	case 'o':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X   X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'P':
	case 'p':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "X",		x, y+4, c);
		break;
	}
	case 'Q':
	case 'q':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X X X",	x, y+2, c);
		DrawLine(screen, "X  X",	x, y+3, c);
		DrawLine(screen, " XX X",	x, y+4, c);
		break;
	}
	case 'R':
	case 'r':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'S':
	case 's':
	{
		DrawLine(screen, " XXXX",	x, y+0, c);
		DrawLine(screen, "X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "    X",	x, y+3, c);
		DrawLine(screen, "XXXX",	x, y+4, c);
		break;
	}
	case 'T':
	case 't':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "  X",		x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "  X",		x, y+4, c);
		break;
	}
	case 'U':
	case 'u':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "X   X",		x, y+2, c);
		DrawLine(screen, "X   X",		x, y+3, c);
		DrawLine(screen, " XXX",		x, y+4, c);
		break;
	}
	case 'V':
	case 'v':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, " X X",		x, y+2, c);
		DrawLine(screen, " X X",		x, y+3, c);
		DrawLine(screen, "  X",		x, y+4, c);
		break;
	}
	case 'W':
	case 'w':
	{
		DrawLine(screen, "X X X",	x, y+0, c);
		DrawLine(screen, "X X X",		x, y+1, c);
		DrawLine(screen, "X X X",		x, y+2, c);
		DrawLine(screen, "X X X",		x, y+3, c);
		DrawLine(screen, " X X",		x, y+4, c);
		break;
	}
	case 'X':
	case 'x':
	{
		DrawLine(screen, "X   X",		x, y+0, c);
		DrawLine(screen, " X X",		x, y+1, c);
		DrawLine(screen, "  X",			x, y+2, c);
		DrawLine(screen, " X X",		x, y+3, c);
		DrawLine(screen, "X   X",		x, y+4, c);
		break;
	}
	case 'Y':
	case 'y':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, " X X",	x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "  X",		x, y+4, c);
		break;
	}
	case 'Z':
	case 'z':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "   X",	x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, " X",		x, y+3, c);
		DrawLine(screen, "XXXXX",		x, y+4, c);
		break;
	}
	case '0':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X  XX",	x, y+1, c);
		DrawLine(screen, "X X X",		x, y+2, c);
		DrawLine(screen, "XX  X",		x, y+3, c);
		DrawLine(screen, " XXX",		x, y+4, c);
		break;
	}
	case '1':
	{
		DrawLine(screen, "  X",		x, y+0, c);
		DrawLine(screen, " XX",		x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case '2':
	{
		DrawLine(screen, " XXX",		x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "  XX",		x, y+2, c);
		DrawLine(screen, " X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case '3':
	{
		DrawLine(screen, " XXX",		x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "  XX",		x, y+2, c);
		DrawLine(screen, "X   X",		x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '4':
	{
		DrawLine(screen, "X   X",		x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "XXXXX",		x, y+2, c);
		DrawLine(screen, "    X",		x, y+3, c);
		DrawLine(screen, "    X",	x, y+4, c);
		break;
	}
	case '5':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "    X",	x, y+3, c);
		DrawLine(screen, "XXXX",	x, y+4, c);
		break;
	}
	case '6':
	{
		DrawLine(screen, " XXXX",	x, y+0, c);
		DrawLine(screen, "X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '7':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "    X",	x, y+1, c);
		DrawLine(screen, "   X",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '8':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '9':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, " XXXX",	x, y+2, c);
		DrawLine(screen, "    X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '.':
	{
		DrawLine(screen, "   X",	x, y+3, c);
		break;
	}
	case ',':
	{
		DrawLine(screen, "   X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '?':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "  XX",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case ':':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+3, c);
		break;
	}
	case '<':
	{
		DrawLine(screen, "  X",	x, y+0, c);
		DrawLine(screen, " X",	x, y+1, c);
		DrawLine(screen, "X",	x, y+2, c);
		DrawLine(screen, " X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '>':
	{
		DrawLine(screen, "X",	x, y+0, c);
		DrawLine(screen, " X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+2, c);
		DrawLine(screen, " X",	x, y+3, c);
		DrawLine(screen, "X",	x, y+4, c);
		break;
	}
	case '*':
	{
		DrawLine(screen, "  X",	x, y+0, c);
		DrawLine(screen, "X X X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "X X X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '+':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+3, c);
		break;
	}
	case '-':
	{
		DrawLine(screen, " XXX",	x, y+2, c);
		break;
	}
	case '=':
	{
		DrawLine(screen, " XXX",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '\'':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+3, c);
		break;
	}
	case '`':
	{
		DrawLine(screen, " X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+2, c);
		break;
	}
	case '"':
	{
		DrawLine(screen, " X X",	x, y+1, c);
		DrawLine(screen, " X X",	x, y+2, c);
		DrawLine(screen, " X X",	x, y+3, c);
		break;
	}
	case ';':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+3, c);
		DrawLine(screen, " X",	x, y+4, c);
		break;
	}
	case '^':
	{
		DrawLine(screen, "  X",	x, y+0, c);
		DrawLine(screen, " X X",	x, y+1, c);
		DrawLine(screen, "X   X",	x, y+2, c);
		break;
	}
	case '/':
	{
		DrawLine(screen, "    X",x, y+0, c);
		DrawLine(screen, "   X", x, y+1, c);
		DrawLine(screen, "  X",  x, y+2, c);
		DrawLine(screen, " X",   x, y+3, c);
		DrawLine(screen, "X",    x, y+4, c);
		break;
	}
	case '\\':
	{
		DrawLine(screen, "X",    x, y+0, c);
		DrawLine(screen, " X",   x, y+1, c);
		DrawLine(screen, "  X",  x, y+2, c);
		DrawLine(screen, "   X", x, y+3, c);
		DrawLine(screen, "    X",x, y+4, c);
		break;
	}
	case '[':
	{
		DrawLine(screen, "XX",x, y+0, c);
		DrawLine(screen, "X", x, y+1, c);
		DrawLine(screen, "X", x, y+2, c);
		DrawLine(screen, "X", x, y+3, c);
		DrawLine(screen, "XX",x, y+4, c);
		break;
	}
	case ']':
	{
		DrawLine(screen, "XX",x, y+0, c);
		DrawLine(screen, " X",x, y+1, c);
		DrawLine(screen, " X",x, y+2, c);
		DrawLine(screen, " X",x, y+3, c);
		DrawLine(screen, "XX",x, y+4, c);
		break;
	}
	}
}
void DrawString(uint8_t screen, std::string text, uint16_t x, uint16_t y, uint16_t c)
{
	for(unsigned int i = 0; i < text.length(); i++)
	{
		DrawLetter(screen, text.at(i), x + (i * 6), y, c);
	}
}

}}//namespace D2K::GUI
