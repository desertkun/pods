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
#include "device/sound.h"
#include "app/melodies.h"

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

void Intro::init()
{
	device::Display::invertDisplay(false);

	device::Display::drawRefreshSequence();

	device::Sound::playMelody(&Melodies::Startup[0]);

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

	device::Display::drawRefreshSequence();

	gs::MainMenu::switchTo();

}

void Intro::switchTo()
{
	app::Application::s_instance->switchTo(&s_instance);
}

};


