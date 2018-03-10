/*
 * application.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#include "app/application.h"
#include "app/messages.h"
#include "device/device.h"
#include "device/network.h"
#include "main.h"

#include <cstring>

namespace app
{

Application* Application::s_instance = nullptr;

Application::Application() :
	m_keyA(CTL_A_GPIO_Port, CTL_A_Pin),
	m_keyB(CTL_B_GPIO_Port, CTL_B_Pin),
	m_keyUp(CTL_UP_GPIO_Port, CTL_UP_Pin),
	m_keyDown(CTL_DOWN_GPIO_Port, CTL_DOWN_Pin),
	m_keyLeft(CTL_LEFT_GPIO_Port, CTL_LEFT_Pin),
	m_keyRight(CTL_RIGHT_GPIO_Port, CTL_RIGHT_Pin),
	enabled(false)
{
	s_instance = this;

	device::System::init();
	device::Display::init();
	device::Input::init();
	device::Network::init();
}

void Application::messageReceived(uint8_t dataSize, uint8_t* data)
{
	uint8_t messageType = *data++;
	dataSize--;

	switch (messageType)
	{
		case MSG_TEST:
		{
			msg::Test test;
			std::memcpy((void*)&test, (const void*)data, sizeof(test));

			enabled = test.a;
		}
	}
}

void Application::loop()
{
	uint8_t a = 0;
	uint8_t dir = 0;

	enabled = true;

	for(;;)
	{
		if (getKeyA().isJustDown())
		{
			enabled = !enabled;

			msg::Test test;
			test.a = enabled ? 1 : 0;

			device::Network::send(MSG_TEST, sizeof(test), (uint8_t*)&test);
		}

		if (!enabled)
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
