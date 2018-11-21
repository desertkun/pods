/*
 * intro.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#include "app/gs/intro.h"
#include "app/gs/mainmenu.h"
#include "app/application.h"
#include "device/display.h"

extern const unsigned char heart_bmp[];
extern const unsigned char heart_inside_bmp[];
extern const unsigned char photo_bmp [];

namespace gs
{

Intro Intro::s_instance = Intro();

void Intro::update(uint32_t dt)
{
	//
}

void Intro::release()
{
	//
}

static void refresh()
{
	device::Display::clear();

	for (int i = 0; i <= 64; i++)
	{
		device::Display::drawLine(0, i, i * 2 - 1, 0);
		device::Display::drawLine(127, 64 - i, 127 - i * 2 - 1, 63);

		if (i % 4 == 0)
			device::Display::flush();
	}

	device::Display::setModeReset();

	for (int i = 0; i <= 64; i++)
	{
		device::Display::drawLine(0, i, i * 2 - 1, 0);
		device::Display::drawLine(127, 64 - i, 127 - i * 2 - 1, 63);

		if (i % 4 == 0)
			device::Display::flush();
	}

	device::Display::setModeSet();
	device::Display::clear();
	device::Display::flush();
}

void Intro::init()
{
	device::Display::invertDisplay(false);

	refresh();

	device::Display::clear();
	device::Display::drawImage(60, 8, 16, 16, heart_bmp);
	device::Display::drawText(32, 32, "dedicated to", device::Display::Font::f_5x7);
	device::Display::drawText(12, 42, "Ekaterina Glukhaia", device::Display::Font::f_5x7);

	device::Display::flush();

	device::Display::setModeInvert();

	for (int i = 0; i < 4; i++)
	{
		HAL_Delay(500);

		device::Display::drawImage(60, 8, 16, 16, heart_inside_bmp);
		device::Display::flush();
	}

	device::Display::setModeSet();

	device::Display::clear();
	device::Display::drawImage(32, 0, 64, 64, photo_bmp);
	device::Display::flush();

	HAL_Delay(2000);

	refresh();

	gs::MainMenu::switchTo();

}

void Intro::switchTo()
{
	app::Application::s_instance->switchTo(&s_instance);
}

};


