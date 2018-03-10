/*
 * input.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */


#include <device/input.h>

extern "C" {
	#include "stm32f1xx.h"
}

namespace device
{

Key::Key(GPIO_TypeDef* port, uint16_t pin) :
	m_justDown(false),
	m_justUp(false),
	m_state(KeyState::up),
	m_time(0),
	m_port(port),
	m_pin(pin)
{

}

void Key::update()
{
	switch (m_state)
	{
		case KeyState::down:
		{
			bool currentValue = HAL_GPIO_ReadPin(m_port, m_pin);

			if (!currentValue)
			{
				m_time = HAL_GetTick();
				m_state = KeyState::goingUp;
			}

			break;
		}
		case KeyState::up:
		{
			bool currentValue = HAL_GPIO_ReadPin(m_port, m_pin);

			if (currentValue)
			{
				m_time = HAL_GetTick();
				m_state = KeyState::goingDown;
			}

			break;
		}
		case KeyState::goingUp:
		{
			uint32_t now = HAL_GetTick();

			if (now - m_time >= 25)
			{
				bool currentValue = HAL_GPIO_ReadPin(m_port, m_pin);

				if (currentValue)
				{
					m_state = KeyState::down;
				}
				else
				{
					m_state = KeyState::up;
					m_justUp = true;
				}
			}

			break;
		}
		case KeyState::goingDown:
		{
			uint32_t now = HAL_GetTick();

			if (now - m_time >= 25)
			{
				bool currentValue = HAL_GPIO_ReadPin(m_port, m_pin);

				if (currentValue)
				{
					m_state = KeyState::up;
				}
				else
				{
					m_state = KeyState::down;
					m_justDown = true;
				}
			}

			break;
		}
	}
}

bool Key::isDown()
{
	update();

	return m_state == KeyState::down;
}


bool Key::isUp()
{
	update();

	return m_state == KeyState::up;
}

bool Key::isJustUp()
{
	update();

	if (m_justUp)
	{
		m_justUp = false;
		return true;
	}

	return false;
}

bool Key::isJustDown()
{
	update();

	if (m_justDown)
	{
		m_justDown = false;
		return true;
	}

	return false;
}

void Input::init()
{
	//
}

}
