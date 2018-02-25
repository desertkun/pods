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
		static void init();

	public:
		static void clear();
		static void flush();
		static void fill(uint8_t with);

		static void drawLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
	};

};

#endif /* DEVICE_DISPLAY_H_ */
