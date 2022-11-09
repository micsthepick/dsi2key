#include <string> // std::string
#include "keyboardWindow.h"
#include "configWindow.h"
#include "turboWindow.h"
#include "gui/gui.h"
#include "common/udp.h"
#include "common/key.h"
#include "config.h"
#include "core.h"

namespace D2K {namespace GUI {namespace Keyboard {

WindowClass g_window;

const int gamepad_buttons_count = 29;
const int keyboard_buttons_count = 103;
Button* gamepad_buttons[gamepad_buttons_count]{};
Button* keyboard_buttons[keyboard_buttons_count]{};

Edit* edit_entry{};
Label* label_entry{};

std::string Buffer{};
size_t BufferLen = 0;
void append(uint16_t KEY)
{
	ConfigWindow::current_pressed_key = KEY;
	if(KEY < 256)
	{
		char c = (char)KEY;
		switch(c)
		{
		// TODO::UPDATE COMMMENTED SECTION
		/*case ';':
		{
			ConfigWindow::current_pressed_key = 0xba;
			break;
		}
		case '\'':
		{
			ConfigWindow::current_pressed_key = 0xde;
			break;
		}
		case '.':
		{
			ConfigWindow::current_pressed_key = 0xbe;
			break;
		}
		case ',':
		{
			ConfigWindow::current_pressed_key = 0xbc;
			break;
		}
		case '=':
		{
			ConfigWindow::current_pressed_key = 0xbb;
			break;
		}*/
		case Key::DSKEY_BACK:
		{
			if(Buffer.length() > 0)
				Buffer.resize(Buffer.length() - 1);
			return;
		}
		}
		if(BufferLen > Buffer.size())
		{
			Buffer += c;
		}
	}
}

void ButtonCloseFunction()
{
	Keyboard::g_window.SetVisible(false);
}

void ButtonEnterFunction()
{
	append(Key::DSKEY_RETURN);
	ButtonCloseFunction();
}

// Keyboard Functions
void Button1Function()              { append(Key::DSKEY_1); }
void Button2Function()              { append(Key::DSKEY_2); }
void Button3Function()              { append(Key::DSKEY_3); }
void Button4Function()              { append(Key::DSKEY_4); }
void Button5Function()              { append(Key::DSKEY_5); }
void Button6Function()              { append(Key::DSKEY_6); }
void Button7Function()              { append(Key::DSKEY_7); }
void Button8Function()              { append(Key::DSKEY_8); }
void Button9Function()              { append(Key::DSKEY_9); }
void Button0Function()              { append(Key::DSKEY_0); }

void ButtonMinusFunction()          { append(Key::DSKEY_OEM_MINUS); }
void ButtonEqualsFunction()         { append(Key::DSKEY_OEM_PLUS); }

void ButtonEscapeFunction()         { append(Key::DSKEY_ESCAPE); }
void ButtonF1Function()             { append(Key::DSKEY_F1); }
void ButtonF2Function()             { append(Key::DSKEY_F2); }
void ButtonF3Function()             { append(Key::DSKEY_F3); }
void ButtonF4Function()             { append(Key::DSKEY_F4); }
void ButtonF5Function()             { append(Key::DSKEY_F5); }
void ButtonF6Function()             { append(Key::DSKEY_F6); }
void ButtonF7Function()             { append(Key::DSKEY_F7); }
void ButtonF8Function()             { append(Key::DSKEY_F8); }
void ButtonF9Function()             { append(Key::DSKEY_F9); }
void ButtonF10Function()            { append(Key::DSKEY_F10); }
void ButtonF11Function()            { append(Key::DSKEY_F11); }
void ButtonF12Function()            { append(Key::DSKEY_F12); }

void ButtonBackQuoteFunction()      { append(Key::DSKEY_OEM_3); }
void ButtonTabFunction()            { append(Key::DSKEY_TAB); }
void ButtonLeftBracketFunction()    { append(Key::DSKEY_OEM_4); }
void ButtonRightBracketFunction()   { append(Key::DSKEY_OEM_6); }
void ButtonBackSlashFunction()      { append(Key::DSKEY_OEM_5); }
void ButtonCapsLockFunction()       { append(Key::DSKEY_CAPITAL);}
void ButtonSemiColonFunction()      { append(Key::DSKEY_OEM_1); }
void ButtonQuoteFunction()          { append(Key::DSKEY_OEM_7); }
void ButtonLeftShiftFunction()      { append(Key::DSKEY_LSHIFT); }
void ButtonCommaFunction()          { append(Key::DSKEY_OEM_COMMA); }
void ButtonPeriodFunction()         { append(Key::DSKEY_OEM_PERIOD); }
void ButtonSlashFunction()          { append(Key::DSKEY_OEM_2); }
void ButtonRightShiftFunction()     { append(Key::DSKEY_RSHIFT); }
void ButtonLeftControlFunction()    { append(Key::DSKEY_LCONTROL); }
void ButtonLeftWinFunction()        { append(Key::DSKEY_LWIN); }
void ButtonLeftAltFunction()        { append(Key::DSKEY_LMENU); }
void ButtonSpaceFunction()          { append(Key::DSKEY_SPACE); }
void ButtonRightAltFunction()       { append(Key::DSKEY_RMENU); }
void ButtonRightWinFunction()       { append(Key::DSKEY_RWIN); }
void ButtonRightControlFunction()   { append(Key::DSKEY_RCONTROL); }

void ButtonAFunction()              { append(Key::DSKEY_A); }
void ButtonBFunction()              { append(Key::DSKEY_B); }
void ButtonCFunction()              { append(Key::DSKEY_C); }
void ButtonDFunction()              { append(Key::DSKEY_D); }
void ButtonEFunction()              { append(Key::DSKEY_E); }
void ButtonFFunction()              { append(Key::DSKEY_F); }
void ButtonGFunction()              { append(Key::DSKEY_G); }
void ButtonHFunction()              { append(Key::DSKEY_H); }
void ButtonIFunction()              { append(Key::DSKEY_I); }
void ButtonJFunction()              { append(Key::DSKEY_J); }
void ButtonKFunction()              { append(Key::DSKEY_K); }
void ButtonLFunction()              { append(Key::DSKEY_L); }
void ButtonMFunction()              { append(Key::DSKEY_M); }
void ButtonNFunction()              { append(Key::DSKEY_N); }
void ButtonOFunction()              { append(Key::DSKEY_O); }
void ButtonPFunction()              { append(Key::DSKEY_P); }
void ButtonQFunction()              { append(Key::DSKEY_Q); }
void ButtonRFunction()              { append(Key::DSKEY_R); }
void ButtonSFunction()              { append(Key::DSKEY_S); }
void ButtonTFunction()              { append(Key::DSKEY_T); }
void ButtonUFunction()              { append(Key::DSKEY_U); }
void ButtonVFunction()              { append(Key::DSKEY_V); }
void ButtonWFunction()              { append(Key::DSKEY_W); }
void ButtonXFunction()              { append(Key::DSKEY_X); }
void ButtonYFunction()              { append(Key::DSKEY_Y); }
void ButtonZFunction()              { append(Key::DSKEY_Z); }

void ButtonPrintScreenFunction()    { append(Key::DSKEY_PRINT); }
void ButtonScrollLockFunction()     { append(Key::DSKEY_SCROLL); }
void ButtonPauseBreakFunction()     { append(Key::DSKEY_PAUSE); }
void ButtonInsertFunction()         { append(Key::DSKEY_INSERT); }
void ButtonHomeFunction()           { append(Key::DSKEY_HOME); }
void ButtonPageUpFunction()         { append(Key::DSKEY_PRIOR); }
void ButtonNumLockFunction()        { append(Key::DSKEY_NUMLOCK); }
void ButtonNumPadSlashFunction()    { append(Key::DSKEY_DIVIDE); }
void ButtonNumPadAsteriskFunction() { append(Key::DSKEY_MULTIPLY); }
void ButtonDeleteFunction()         { append(Key::DSKEY_DELETE); } // TODO: currently unsupported
void ButtonNumPadMinusFunction()    { append(Key::DSKEY_SUBTRACT); }
void ButtonEndFunction()            { append(Key::DSKEY_END); }
void ButtonPageDownFunction()       { append(Key::DSKEY_NEXT); }
void ButtonNumPad7Function()        { append(Key::DSKEY_NUMPAD7); }
void ButtonNumPad8Function()        { append(Key::DSKEY_NUMPAD8); }
void ButtonNumPad9Function()        { append(Key::DSKEY_NUMPAD9); }
void ButtonNumPadPlusFunction()     { append(Key::DSKEY_ADD); }
void ButtonNumPad4Function()        { append(Key::DSKEY_NUMPAD4); }
void ButtonNumPad5Function()        { append(Key::DSKEY_NUMPAD5); }
void ButtonNumPad6Function()        { append(Key::DSKEY_NUMPAD6); }
void ButtonNumPad1Function()        { append(Key::DSKEY_NUMPAD1); }
void ButtonNumPad2Function()        { append(Key::DSKEY_NUMPAD2); }
void ButtonNumPad3Function()        { append(Key::DSKEY_NUMPAD3); }
void ButtonNumPadEnterFunction()    { append(Key::DSKEY_RETURN); } // TODO: currently unsupported
void ButtonNumPad0Function()        { append(Key::DSKEY_NUMPAD0); }
void ButtonNumPadPeriodFunction()   { append(Key::DSKEY_DECIMAL); }
void ButtonUpFunction()             { append(Key::DSKEY_UP); }
void ButtonDownFunction()           { append(Key::DSKEY_DOWN); }
void ButtonLeftFunction()           { append(Key::DSKEY_LEFT); }
void ButtonRightFunction()          { append(Key::DSKEY_RIGHT); }
void ButtonBackspaceFunction()      { append(Key::DSKEY_BACK); }

// Gamepad Functions
void ButtonGamepadAFunction()              { append(Key::JOY+0); }
void ButtonGamepadBFunction()              { append(Key::JOY+1); }
void ButtonGamepadXFunction()              { append(Key::JOY+2); }
void ButtonGamepadYFunction()              { append(Key::JOY+3); }

#if defined(_3DS)
void ButtonGamepadZLFunction()             { append(Key::JOY+4); }
void ButtonGamepadZRFunction()             { append(Key::JOY+5); }
#endif

void ButtonGamepadLFunction()              { append(Key::JOY_AXIS_Z_MINUS); }  // z
void ButtonGamepadRFunction()              { append(Key::JOY_AXIS_Z_PLUS); }   // z

void ButtonGamepadSelectFunction()         { append(Key::JOY+6); }
void ButtonGamepadStartFunction()          { append(Key::JOY+7); }

#if defined(_3DS)
void ButtonGamepadSliderVolumeFunction()   { append(Key::DSKEY_VOLUME_UP); }
void ButtonGamepadSlider3DFunction()       { append(Key::JOY+9); }
#elif defined(_NDS)
void ButtonGamepadBlueFunction()           { append(Key::JOY+4); }
void ButtonGamepadYellowFunction()         { append(Key::JOY+5); }
void ButtonGamepadRedFunction()            { append(Key::JOY+8); }
void ButtonGamepadGreenFunction()          { append(Key::JOY+9); }
#endif

void ButtonGamepadLidFunction()            { append(Key::JOY+10); }

void ButtonGamepadLeftFunction()           { append(Key::JOY_HAT_LEFT); }
void ButtonGamepadRightFunction()          { append(Key::JOY_HAT_RIGHT); }
void ButtonGamepadUpFunction()             { append(Key::JOY_HAT_UP); }
void ButtonGamepadDownFunction()           { append(Key::JOY_HAT_DOWN); }

void ButtonGamepadLeftCPadFunction()       { append(Key::JOY_AXIS_X_MINUS); }  // x
void ButtonGamepadRightCPadFunction()      { append(Key::JOY_AXIS_X_PLUS); }   // x
void ButtonGamepadUpCPadFunction()         { append(Key::JOY_AXIS_Y_MINUS); }  // y
void ButtonGamepadDownCPadFunction()       { append(Key::JOY_AXIS_Y_PLUS); }   // y

void ButtonGamepadLeftCStickFunction()     { append(Key::JOY_AXIS_RX_MINUS); } // rx
void ButtonGamepadRightCStickFunction()    { append(Key::JOY_AXIS_RX_PLUS); }  // rx
void ButtonGamepadUpCStickFunction()       { append(Key::JOY_AXIS_RY_MINUS); } // ry
void ButtonGamepadDownCStickFunction()     { append(Key::JOY_AXIS_RY_PLUS); }  // ry

void GamepadSetVisible(bool visible)
{
	for(int counter = 0; counter < gamepad_buttons_count; counter++)
	{
		if(gamepad_buttons[counter])
			gamepad_buttons[counter]->SetVisible(visible);
	}
}

void KeyboardSetVisible(bool visible)
{
	for(int counter = 0; counter < keyboard_buttons_count; counter++)
	{
		if(keyboard_buttons[counter])
			keyboard_buttons[counter]->SetVisible(visible);
	}
}

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	const int gap = 1;
	int x = gap;
	int y = gap;
	const int w = 15;
	const int h = 15;
	unsigned int buttons_counter = 0;
	const int keyboard_base_x = gap;
	const int keyboard_base_y = gap;
	const int gamepad_base_x = 8;
	const int gamepad_base_y = 30;
	
	const int print_scroll_pause_base_x = keyboard_base_x;
	const int print_scroll_pause_base_y = gap + (h + gap) * 6;
	const int ins_home_pgup_base_x      = keyboard_base_x;
	const int ins_home_pgup_base_y      = gap + (h + gap) * 8;
	const int up_down_left_right_base_x = keyboard_base_x;
	const int up_down_left_right_base_y = gap + (h + gap) * 10;

	AppendObject(new Button(m_screen, Rect(229, 1, 10, 10), "Close", ButtonCloseFunction));
	{
		buttons_counter = 0;
		// Keyboard
		{
		// Function row
		{
		x = keyboard_base_x;
		y = keyboard_base_y;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "Esc", ButtonEscapeFunction)); (x += w + 7 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F1",  ButtonF1Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F2",  ButtonF2Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F3",  ButtonF3Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F4",  ButtonF4Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F5",  ButtonF5Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F6",  ButtonF6Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F7",  ButtonF7Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F8",  ButtonF8Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F9",  ButtonF9Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F10", ButtonF10Function));    (x += w + 7 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F11", ButtonF11Function));    (x += w + 7 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "F12", ButtonF12Function));    (x += w + gap);
		}
		// number row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "`",  ButtonBackQuoteFunction)); (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "1",  Button1Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "2",  Button2Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "3",  Button3Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "4",  Button4Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "5",  Button5Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "6",  Button6Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "7",  Button7Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "8",  Button8Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "9",  Button9Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "0",  Button0Function));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "-",  ButtonMinusFunction));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   "=",  ButtonEqualsFunction));    (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "<-", ButtonBackspaceFunction)); (x += w*2 + gap);
		}
		// qwerty row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.6, h), "Tab", ButtonTabFunction));          (x += w*1.6 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "Q",   ButtonQFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "W",   ButtonWFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "E",   ButtonEFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "R",   ButtonRFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "T",   ButtonTFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "Y",   ButtonYFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "U",   ButtonUFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "I",   ButtonIFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "O",   ButtonOFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "P",   ButtonPFunction));            (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "[",   ButtonLeftBracketFunction));  (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "]",   ButtonRightBracketFunction)); (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.4, h), "\\",  ButtonBackSlashFunction));    (x += w*1.4 + gap);
		}
		// asdfgh row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.8, h),  "Caps",  ButtonCapsLockFunction));  (x += w*1.8 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "A",     ButtonAFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "S",     ButtonSFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "D",     ButtonDFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "F",     ButtonFFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "G",     ButtonGFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "H",     ButtonHFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "J",     ButtonJFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "K",     ButtonKFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "L",     ButtonLFunction));         (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      ";",     ButtonSemiColonFunction)); (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "'",     ButtonQuoteFunction));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2.3, h),  "Enter", ButtonEnterFunction));     (x += w*2.3 + gap);
		}
		// zxcvbn row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2.6, h),   "Shift", ButtonLeftShiftFunction));  (x += w*2.6 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "Z",     ButtonZFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "X",     ButtonXFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "C",     ButtonCFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "V",     ButtonVFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "B",     ButtonBFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "N",     ButtonNFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "M",     ButtonMFunction));          (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       ",",     ButtonCommaFunction));      (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       ".",     ButtonPeriodFunction));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),       "/",     ButtonSlashFunction));      (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2.5+1, h), "Shift", ButtonRightShiftFunction)); (x += w*2.5+1 + gap);
		}
		// ctrl row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.8, h), "Ctrl",  ButtonLeftControlFunction));  (x += w*1.8 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.5, h), "Win",   ButtonLeftWinFunction));      (x += w*1.5 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.5, h), "Alt",   ButtonLeftAltFunction));      (x += w*1.5 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*6, h),   "Space", ButtonSpaceFunction));        (x += w*6 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.5, h), "Alt",   ButtonRightAltFunction));     (x += w*1.5 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.5, h), "Win",   ButtonRightWinFunction));     (x += w*1.5 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*1.8, h), "Ctrl",  ButtonRightControlFunction)); (x += w*1.8 + gap);
		}
		}

		// printscroll pause
		{{
		x = print_scroll_pause_base_x;
		y = print_scroll_pause_base_y;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "PrSc", ButtonPrintScreenFunction)); (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "ScLk", ButtonScrollLockFunction));  (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "PsBk", ButtonPauseBreakFunction));  (x += w*2 + gap);
		}}

		// ins home pgup del end pgdown
		{
		// ins home pgup
		{
		x = ins_home_pgup_base_x;
		y = ins_home_pgup_base_y;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "Ins",  ButtonInsertFunction)); (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "Home", ButtonHomeFunction));   (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "PgUp", ButtonPageUpFunction)); (x += w*2 + gap);
		}
		// del end pgdown
		{
		x = ins_home_pgup_base_x;
		y += h + gap; // row 2
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "Del",  ButtonDeleteFunction));   (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "End",  ButtonEndFunction));      (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "PgDn", ButtonPageDownFunction)); (x += w*2 + gap);
		}
		}

		// numpad
		{
		const int numpad_base_x = gap + (w + gap) * 11;
		const int numpad_base_y = gap + (h + gap) * 7;
		// numlock row
		{
		x = numpad_base_x;
		y = numpad_base_y;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "Nu", ButtonNumLockFunction));        (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "/",  ButtonNumPadSlashFunction));    (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "*",  ButtonNumPadAsteriskFunction)); (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "-",  ButtonNumPadMinusFunction));    (x += w + gap);
		}
		// 7 8 9
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "7", ButtonNumPad7Function));    (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "8", ButtonNumPad8Function));    (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),     "9", ButtonNumPad9Function));    (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h * 2), "+", ButtonNumPadPlusFunction)); (x += w + gap);
		}
		// 4 5 6
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "4", ButtonNumPad4Function)); (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "5", ButtonNumPad5Function)); (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "6", ButtonNumPad6Function)); (x += w + gap);
		}
		// 1 2 3
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "1", ButtonNumPad1Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "2", ButtonNumPad2Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),      "3", ButtonNumPad3Function));     (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h * 2), "En", ButtonNumPadEnterFunction)); (x += w + gap);
		}
		// 0, .
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w*2, h), "0", ButtonNumPad0Function));      (x += w*2 + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h),   ".", ButtonNumPadPeriodFunction)); (x += w + gap);
		}
		}

		// up down left right
		{
		x = up_down_left_right_base_x + w + gap; // column 2
		y = up_down_left_right_base_y;           // row 1
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "^", ButtonUpFunction));    (x += w + gap);
		x = up_down_left_right_base_x;           // coloumn 1
		y = up_down_left_right_base_y + h + gap; // row 2
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "<", ButtonLeftFunction));  (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), "v", ButtonDownFunction));  (x += w + gap);
		AppendObject(keyboard_buttons[buttons_counter++] = new Button(m_screen, Rect(x, y, w, h), ">", ButtonRightFunction)); (x += w + gap);
		}
	}

	{
		//Gamepad
		static uint16_t lx, ly;
		buttons_counter = 0;
		lx = gamepad_base_x + 9;
		ly = gamepad_base_y + 15;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(lx,ly,10,10), "L", &ButtonGamepadLFunction));
	#ifdef _3DS
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(lx,ly+13,10,10), "ZL", &ButtonGamepadZLFunction));
	#endif
		
		static uint16_t rx, ry;
		rx = gamepad_base_x + 235;
		ry = gamepad_base_y + ly;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(rx,ry,10,10), "R", &ButtonGamepadRFunction));
	#ifdef _3DS
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(rx,ry+13,10,10), "ZR", &ButtonGamepadZRFunction));
	#endif

		static uint16_t dpad_x, dpad_y;
		dpad_x = gamepad_base_x + 9;
		dpad_y = gamepad_base_y + 80;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(dpad_x,      dpad_y + 15, 10, 10), "<", &ButtonGamepadLeftFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(dpad_x + 60, dpad_y + 15, 10, 10), ">", &ButtonGamepadRightFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(dpad_x + 30, dpad_y,      10, 10), "^", &ButtonGamepadUpFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(dpad_x + 30, dpad_y + 30, 10, 10), "v", &ButtonGamepadDownFunction));
	
	#ifdef _3DS
		static uint16_t cpad_x, cpad_y;
		cpad_x = gamepad_base_x + 30;
		cpad_y = gamepad_base_y + 15;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cpad_x,      cpad_y + 15, 10, 10), "CP <", &ButtonGamepadLeftCPadFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cpad_x + 60, cpad_y + 15, 10, 10), "CP >", &ButtonGamepadRightCPadFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cpad_x + 30, cpad_y,      10, 10), "CP ^", &ButtonGamepadUpCPadFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cpad_x + 30, cpad_y + 30, 10, 10), "CP v", &ButtonGamepadDownCPadFunction));
	
		static uint16_t cstick_x, cstick_y;
		cstick_x = gamepad_base_x + 135;
		cstick_y = gamepad_base_y + 15;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cstick_x,      cstick_y + 15, 10, 10), "CS <", &ButtonGamepadLeftCStickFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cstick_x + 60, cstick_y + 15, 10, 10), "CS >", &ButtonGamepadRightCStickFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cstick_x + 30, cstick_y,      10, 10), "CS ^", &ButtonGamepadUpCStickFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(cstick_x + 30, cstick_y + 30, 10, 10), "CS v", &ButtonGamepadDownCStickFunction));
	#endif
	
		static uint16_t abxy_x, abxy_y;
		abxy_x = gamepad_base_x + 175;
		abxy_y = gamepad_base_y + 80;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(abxy_x + 60, abxy_y + 15, 10, 10), "A", &ButtonGamepadAFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(abxy_x + 30, abxy_y + 30, 10, 10), "B", &ButtonGamepadBFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(abxy_x + 30, abxy_y,      10, 10), "X", &ButtonGamepadXFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(abxy_x     , abxy_y + 15, 10, 10), "Y", &ButtonGamepadYFunction));
	
		static uint16_t start_select_x, start_select_y;
		start_select_x = gamepad_base_x + 85;
		start_select_y = gamepad_base_y + 115;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(start_select_x + 60, start_select_y, 10, 10), "Start", &ButtonGamepadStartFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(start_select_x,     start_select_y, 10, 10), "Select", &ButtonGamepadSelectFunction));
	
	#if defined(_3DS)
		static uint16_t sliders_x, sliders_y;
		sliders_x = gamepad_base_x + 40;
		sliders_y = gamepad_base_y + 130;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(sliders_x,       sliders_y, 10, 10), "Volume", &ButtonGamepadSliderVolumeFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(sliders_x + 100, sliders_y, 10, 10), "3D Slider", &ButtonGamepadSlider3DFunction));
	#elif defined(_NDS)
		static uint16_t byrg_x, byrg_y;
		byrg_x = gamepad_base_x + 40;
		byrg_y = gamepad_base_y + 130;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(byrg_x,       byrg_y, 10, 10), "Blue", &ButtonGamepadBlueFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(byrg_x + 50,  byrg_y, 10, 10), "Yellow", &ButtonGamepadYellowFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(byrg_x + 100, byrg_y, 10, 10), "Red", &ButtonGamepadRedFunction));
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(byrg_x + 150, byrg_y, 10, 10), "Green", &ButtonGamepadGreenFunction));
	#endif

		static uint16_t lid_x, lid_y;
		lid_x = gamepad_base_x + 115;
		lid_y = gamepad_base_y + 145;
		AppendObject(gamepad_buttons[buttons_counter++] = new Button(m_screen, Rect(lid_x, lid_y, 10, 10), "Lid", &ButtonGamepadLidFunction));
	}
}
WindowClass::~WindowClass() { }

// configWindow function
std::string GetString(Label* label, Edit* edit, const std::string& text, int maxLength)
{
	Keyboard::label_entry->SetRect(label->GetRect());
	Keyboard::label_entry->SetText(label->GetText());

	Keyboard::edit_entry->SetRect(edit->GetRect());
	Keyboard::edit_entry->SetText(edit->GetText());

	Buffer = text;
	BufferLen = maxLength;

	ConfigWindow::g_window.SetVisible(false);               // Hide config window
	Keyboard::g_window.SetVisible(true);                    // Show keyboard

	Keyboard::g_window.Draw();                              // And actually draw it
	while(D2K::Loop()
	   && Keyboard::g_window.IsVisible())
	{
		UDP::Update(D2K::g_keys_held, Turbo::GetKeys(), nullptr,
		            &D2K::g_circle_position, &D2K::g_cstick_position,
		            &D2K::g_accel_status, &D2K::g_gyro_status,
		            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status,
		            ConfigWindow::current_pressed_key); // Update keys and press active gamepad key
		if(Keyboard::g_window.Update())                 // If pressed
		{
			ConfigWindow::current_pressed_key = Key::KEY_NONE;
			Keyboard::edit_entry->SetText(Buffer);  // Set text
		}
	}
	Keyboard::g_window.SetVisible(false);                   // Hide keyboard

	ConfigWindow::g_window.SetVisible(true);                // Go back to config window
	return Buffer;
}

uint16_t GetButtonOrKey(bool button_or_key)
{
	Buffer = "";
	BufferLen = 0;

	ConfigWindow::g_window.SetVisible(false); // Hide config window
	Keyboard::g_window.SetVisible(true);      // Show Keyboard and Gamepad Portion
	if(button_or_key)                         // If True/Gamepad
		KeyboardSetVisible(false);        // Hide Keyboard Portion
	else                                      // If False/Keyboard
		GamepadSetVisible(false);         // Hide Gamepad Portion

	Keyboard::g_window.Draw();                // And actually draw it
	while(D2K::Loop()
	   && Keyboard::g_window.IsVisible())
	{
		if(Keyboard::g_window.Update())   // If pressed
		{
			break;
		}
	}
	Keyboard::g_window.SetVisible(false);     // Hide keyboard

	ConfigWindow::g_window.SetVisible(true);  // Go back to config window
	return ConfigWindow::current_pressed_key;
}

}}} // namespace D2K::GUI::Keyboard
