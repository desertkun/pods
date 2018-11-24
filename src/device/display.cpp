
#include <device/display.h>

extern "C"
{
	#include "sh1106.h"
	#include "font7x10.h"
	#include "font5x7.h"
	#include "bitmap_tile.h"
}

namespace device
{

void Display::drawRefreshSequence()
{
	device::Display::clear();

	for (int i = 0; i <= 64; i++)
	{
		device::Display::drawLine(0, i, i * 2 - 1, 0);
		device::Display::drawLine(127, 64 - i, 127 - i * 2 - 1, 63);

		device::Display::flush();
	}

	device::Display::setModeReset();

	for (int i = 0; i <= 64; i++)
	{
		device::Display::drawLine(0, i, i * 2 - 1, 0);
		device::Display::drawLine(127, 64 - i, 127 - i * 2 - 1, 63);

		device::Display::flush();
	}

	device::Display::setModeSet();
	device::Display::clear();
	device::Display::flush();
}

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

void Display::setModeSet()
{
	LCD_PixelMode = LCD_PSET;
}

void Display::setModeReset()
{
	LCD_PixelMode = LCD_PRES;
}

void Display::setModeInvert()
{
	LCD_PixelMode = LCD_PINV;
}

void Display::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	LCD_Line(x1, y1, x2, y2);
}

void Display::invertDisplay(bool invert)
{
	SH1106_SetInvert((uint8_t)invert);
}

void Display::drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	LCD_FillRect(x, y, w, h);
}

void Display::drawImage(uint8_t X, uint8_t Y, uint8_t W, uint8_t H, const uint8_t* image)
{
	LCD_DrawBitmap(X, Y, W, H, image);
}

void Display::drawImageFullscreen(const uint8_t* image)
{
	LCD_DrawBitmapFullscreen(image);
}


void Display::drawText(uint8_t X, uint8_t Y, const char *str, Display::Font font)
{
	const Font_TypeDef *f;

	switch (font)
	{
		case Display::Font::f_7x10:
		{
			f = &Font7x10;
			break;
		}
		case Display::Font::f_5x7:
		default:
		{
			f = &Font5x7;
			break;
		}
	}

	LCD_PutStr(X, Y, str, f);
}

void Display::init()
{
	SH1106_Init();
	SH1106_Flush();

	SH1106_Orientation(LCD_ORIENT_NORMAL);
	SH1106_Contrast(127);
}

};
