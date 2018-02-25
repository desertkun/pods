
#include <device/display.h>

extern "C"
{
	#include <sh1106.h>
	#include "font7x10.h"
	#include "font5x7.h"
	#include "bitmap_tile.h"
}

namespace device
{


void Display::clear()
{
	SH1106_Fill(0x00);
}

void Display::fill(uint8_t with)
{
	SH1106_Fill(with);
}

void Display::flush()
{
	SH1106_Flush();
}

void Display::drawLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	LCD_Line(x, y, w, h);
}

void Display::init()
{
	SH1106_Init();
	SH1106_Flush();

	SH1106_Orientation(LCD_ORIENT_NORMAL);
	SH1106_Contrast(127);
}

};
