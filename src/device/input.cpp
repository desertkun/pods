/*
 * input.cpp
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */


#include <device/input.h>
#include "stm32f1xx.h"

namespace device
{

Key::Key(GPIO_TypeDef* port, uint16_t pin) :
	m_port(port),
	m_pin(pin)
{

}

void Key::update()
{
	m_value = HAL_GPIO_ReadPin(m_port, m_pin);
}

bool Key::getValue() const
{
	return m_value;
}

Input::Keys Input::m_keys = Keys();

void Input::updateKey(uint16_t key)
{
	Keys::const_iterator it = m_keys.find(key);

	if (it == m_keys.end())
		return;

	it->second->update();
}

void Input::registerKey(uint16_t key_port, Key* key)
{
	m_keys.insert(std::pair<uint16_t, Key*>(key_port, key));
}

void Input::init()
{
	//
}

}
