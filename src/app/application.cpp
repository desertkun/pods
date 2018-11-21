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
#include "app/gs/intro.h"
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
	m_gamestate(nullptr)
{
	s_instance = this;

	device::System::init();
	device::Display::init();
	device::Input::init();
	device::Network::init();

	gs::Intro::switchTo();
}

void Application::switchTo(gs::GameState* gs)
{
	if (m_gamestate)
	{
		m_gamestate->release();
	}

	m_gamestate = gs;

	if (m_gamestate)
	{
		m_gamestate->init();
	}
}

void Application::messageReceived(uint8_t dataSize, uint8_t* data)
{
	uint8_t messageType = *data++;
	dataSize--;

	if (m_gamestate)
	{
		m_gamestate->messageReceived(messageType, dataSize, (void*)data);
	}
}

void Application::loop()
{
	uint32_t time = HAL_GetTick();

	while (true)
	{
		uint32_t passed = HAL_GetTick() - time;

		if (passed < 15)
			HAL_Delay(15 - passed);

		time = HAL_GetTick();

		uint32_t dt = 15;
		m_gamestate->update(dt);
	}

}

};
