/*
 * application.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#include "app/application.h"
#include "device/device.h"
#include "main.h"

namespace app
{

Application::Application() :
	m_keyA(CTL_A_GPIO_Port, CTL_A_Pin),
	m_keyB(CTL_B_GPIO_Port, CTL_B_Pin),
	m_keyUp(CTL_UP_GPIO_Port, CTL_UP_Pin),
	m_keyDown(CTL_DOWN_GPIO_Port, CTL_DOWN_Pin),
	m_keyLeft(CTL_LEFT_GPIO_Port, CTL_LEFT_Pin),
	m_keyRight(CTL_RIGHT_GPIO_Port, CTL_RIGHT_Pin)
{
	device::System::init();
	device::Display::init();
	device::Input::init();

	device::Input::registerKey(GPIO_PIN_7, &m_keyA);
	device::Input::registerKey(GPIO_PIN_6, &m_keyB);
	device::Input::registerKey(GPIO_PIN_5, &m_keyUp);
	device::Input::registerKey(GPIO_PIN_4, &m_keyDown);
	device::Input::registerKey(GPIO_PIN_3, &m_keyLeft);
	device::Input::registerKey(GPIO_PIN_2, &m_keyRight);
}

void Application::loop()
{
	uint8_t a = 0;
	uint8_t dir = 0;

	for(;;)
	{
		if (getKeyA().getValue())
			continue;

		device::Display::clear();

		device::Display::drawLine(a, 0, a, 63);
		device::Display::drawLine(0, a >> 1, 127, a >> 1);

		device::Display::flush();

		if (dir == 0)
		{
			a++;

			if ( a >= 127)
			{
				dir = 1;

				device::Display::fill(0xFF);
				device::Display::flush();
			}
		}
		else
		{
			a--;

			if ( a <= 0)
			{
				dir = 0;

				device::Display::fill(0xFF);
				device::Display::flush();
			}
		}
	}
}

};
