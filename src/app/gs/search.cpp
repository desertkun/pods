/*
 * search.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */


#include "app/gs/search.h"
#include "app/gs/mainmenu.h"
#include "app/gs/game.h"
#include "app/application.h"
#include "device/display.h"
#include "device/network.h"
#include "device/system.h"

#include <cstring>

namespace gs
{

Search Search::s_instance = Search();

void Search::messageReceived(uint8_t messageType, uint8_t dataSize, void* data)
{
	switch (messageType)
	{
		case MSG_HELLO:
		{
			m_gotHello = true;
			break;
		}
		case MSG_MAP:
		{
			RECEIVE_MSG(m_mapMsg);
			m_gotMapMsg = true;
			break;
		}
	}
}

void Search::update(uint32_t dt)
{
	m_tim += dt;

	if (device::System::isHost())
	{
		if (m_gotHello)
		{
			m_gotHello = false;

			gs::Game* game = gs::Game::switchTo();

			game->serialize(&m_mapMsg);

			uint32_t cnt = 0;

			while (!device::Network::send(MSG_MAP, sizeof(m_mapMsg), &m_mapMsg))
			{
				if (cnt == 0)
				{
					device::Display::clear();
					device::Display::drawText(8, 27, "Synchronization...", device::Display::Font::f_5x7);
					device::Display::flush();
				}

				if (++cnt >= 5)
				{
					failed();
					return;
				}
				else
				{
					HAL_Delay(1000);
				}
			}

			game->started();
		}
	}
	else
	{
		if (m_gotMapMsg)
		{
			m_gotMapMsg = false;

			gs::Game* game = gs::Game::switchTo();
			game->deserialize(&m_mapMsg);
			game->started();

			return;
		}

		m_cnt += dt;

		if (m_cnt > 100)
		{
			m_cnt = 0;

			device::Network::sendNoAck(MSG_HELLO, 0, nullptr);
		}
	}

	if (m_tim > 5000)
	{
		m_tim = 0;

		failed();
	}
}

void Search::failed()
{
	device::Display::clear();
	device::Display::drawText(4, 27, "Failed to connect :(", device::Display::Font::f_5x7);
	device::Display::flush();

	HAL_Delay(2000);

	gs::MainMenu::switchTo();
}

void Search::release()
{
	//
}

void Search::init()
{
	m_cnt = 0;
	m_tim = 0;
	m_gotHello = false;
	m_gotMapMsg = false;

	device::Display::invertDisplay(false);
	device::Display::clear();
	device::Display::drawText(16, 27, "connecting...", device::Display::Font::f_7x10);
	device::Display::flush();
}

void Search::switchTo()
{
	app::Application::s_instance->switchTo(&s_instance);
}

}



