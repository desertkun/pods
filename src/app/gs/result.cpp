/*
 * intro.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#include "app/gs/result.h"
#include "app/gs/game.h"
#include "app/gs/mainmenu.h"
#include "app/application.h"
#include "device/display.h"
#include "device/sound.h"
#include "device/network.h"
#include "device/system.h"
#include "app/melodies.h"

#include <cstring>

extern const unsigned char photo_bmp [];
extern const unsigned char desert_bmp [];

namespace gs
{

Result Result::s_instance = Result();

void Result::messageReceived(uint8_t messageType, uint8_t dataSize, void* data)
{
	switch (messageType)
	{
		case MSG_MAP:
		{
			RECEIVE_MSG(m_mapMsg);
			m_gotMapMsg = true;
			break;
		}
	}
}

void Result::update(uint32_t dt)
{
	if (m_gotMapMsg)
	{
		m_gotMapMsg = false;

		gs::Game* game = gs::Game::switchTo();
		game->deserialize(&m_mapMsg);

		device::Display::drawRefreshSequence();

		game->started();

		return;
	}

	m_timer += dt;

	if (m_timer > 2000)
	{
		m_timer = 0;

		if (device::System::isHost())
		{
			generate();
		}
	}
}

void Result::disconnect()
{
	device::Display::clear();
	device::Display::drawText(4, 27, "Disconnected :(", device::Display::Font::f_5x7);
	device::Display::flush();

	HAL_Delay(2000);

	gs::MainMenu::switchTo();
}

void Result::generate()
{
	gs::Game* game = gs::Game::switchTo();

	game->serialize(&m_mapMsg);

	while (!device::Network::send(MSG_MAP, sizeof(m_mapMsg), &m_mapMsg))
	{
		disconnect();
	}

	device::Display::drawRefreshSequence();

	game->started();
}

void Result::release()
{
	//
}

void Result::init()
{
	m_timer = 0;

	device::Display::invertDisplay(false);

	device::Display::drawRefreshSequence();

	device::Sound::playMelody(&Melodies::Startup[0]);

	device::Display::setModeSet();

	const uint8_t* winner = device::System::isHost() == m_won ? desert_bmp : photo_bmp;

	device::Display::clear();
	device::Display::drawText(m_won ? 0 : 64, 27, m_won ? "YOU WON!" : "YOU LOST", device::Display::Font::f_7x10);
	device::Display::drawImage(m_won ? 64 : 0, 0, 64, 64, winner);
	device::Display::flush();
}

void Result::switchTo(bool won)
{
	s_instance.m_won = won;
	app::Application::s_instance->switchTo(&s_instance);
}

};


