/*
 * mainmenu.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#include "app/gs/mainmenu.h"
#include "app/application.h"
#include "app/gs/search.h"
#include "device/display.h"

extern const unsigned char logo_bmp [];
extern const unsigned char button_a_bmp [];

namespace gs
{

MainMenu MainMenu::s_instance = MainMenu();

void MainMenu::update(uint32_t dt)
{
	m_cnt += dt;

	device::Display::clear();
	device::Display::drawImage(8, 8 + (m_cnt > 1800 ? ((m_cnt % 50) > 25 ? 2 : 0) : 0), 112, 16, logo_bmp);



	if (m_cnt % 500 < 250)
	{
		device::Display::drawImage(32, 34, 16, 16, button_a_bmp);
	}

	device::Display::drawText(56, 38, "start", device::Display::Font::f_5x7);
	device::Display::flush();

	if (m_cnt > 2000)
	{
		m_cnt = 0;
	}

	if (app::Application::s_instance->getKeyA().isJustDown())
	{
		gs::Search::switchTo();
	}
}

void MainMenu::release()
{
	//
}

void MainMenu::init()
{
	m_cnt = 0;

	device::Display::invertDisplay(false);
	device::Display::clear();
	device::Display::drawImage(8, 8, 112, 16, logo_bmp);
	device::Display::drawImage(32, 34, 16, 16, button_a_bmp);
	device::Display::drawText(56, 38, "start", device::Display::Font::f_5x7);
	device::Display::flush();
}

void MainMenu::switchTo()
{
	app::Application::s_instance->switchTo(&s_instance);
}

}
