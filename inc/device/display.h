/*
 * display.h
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#ifndef DEVICE_DISPLAY_H_
#define DEVICE_DISPLAY_H_

#include <stdint.h>

namespace device
{
	class Display
	{
	public:
		enum class Font
		{
			f_7x10,
			f_5x7
		};

	public:
		static void init();

	public:
		static void clear();
		static void flush();
		static void fill(uint8_t with);
		static void invertDisplay(bool invert);

		static void setModeSet();
		static void setModeReset();
		static void setModeInvert();

		static void drawRefreshSequence();
		static void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
		static void drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
		static void drawImage(uint8_t X, uint8_t Y, uint8_t W, uint8_t H, const uint8_t* image);
		static void drawImageFullscreen(const uint8_t* image);
		static void drawText(uint8_t X, uint8_t Y, const char *str, Font font);
	};
};

#endif /* DEVICE_DISPLAY_H_ */
