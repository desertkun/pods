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

static const device::Tone tones_SMB[] = {
		{2637,18}, // E7 x2
		{   0, 9}, // x3
		{2637, 9}, // E7
		{   0, 9}, // x3
		{2093, 9}, // C7
		{2637, 9}, // E7
		{   0, 9}, // x3
		{3136, 9}, // G7
		{   0,27}, // x3
		{1586, 9}, // G6
		{   0,27}, // x3

		{2093, 9}, // C7
		{   0,18}, // x2
		{1586, 9}, // G6
		{   0,18}, // x2
		{1319, 9}, // E6
		{   0,18}, // x2
		{1760, 9}, // A6
		{   0, 9}, // x1
		{1976, 9}, // B6
		{   0, 9}, // x1
		{1865, 9}, // AS6
		{1760, 9}, // A6
		{   0, 9}, // x1

		{1586,12}, // G6
		{2637,12}, // E7
		{3136,12}, // G7
		{3520, 9}, // A7
		{   0, 9}, // x1
		{2794, 9}, // F7
		{3136, 9}, // G7
		{   0, 9}, // x1
		{2637, 9}, // E7
		{   0, 9}, // x1
		{2093, 9}, // C7
		{2349, 9}, // D7
		{1976, 9}, // B6
		{   0,18}, // x2

		{2093, 9}, // C7
		{   0,18}, // x2
		{1586, 9}, // G6
		{   0,18}, // x2
		{1319, 9}, // E6
		{   0,18}, // x2
		{1760, 9}, // A6
		{   0, 9}, // x1
		{1976, 9}, // B6
		{   0, 9}, // x1
		{1865, 9}, // AS6
		{1760, 9}, // A6
		{   0, 9}, // x1

		{1586,12}, // G6
		{2637,12}, // E7
		{3136,12}, // G7
		{3520, 9}, // A7
		{   0, 9}, // x1
		{2794, 9}, // F7
		{3136, 9}, // G7
		{   0, 9}, // x1
		{2637, 9}, // E7
		{   0, 9}, // x1
		{2093, 9}, // C7
		{2349, 9}, // D7
		{1976, 9}, // B6

		{   0, 0}
};

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
	device::Sound::init();
	device::Network::init();


	device::Sound::playMelody(&tones_SMB[0]);

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
