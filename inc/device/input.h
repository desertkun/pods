/*
 * input.h
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#ifndef DEVICE_INPUT_H_
#define DEVICE_INPUT_H_

#include <stdint.h>
#include <map>

extern "C" {
	#include "stm32f1xx.h"
}

namespace device
{
	enum class KeyState
	{
		goingDown,
		goingUp,
		down,
		up
	};

	class Key
	{
	public:
		Key(GPIO_TypeDef* port, uint16_t pin);

		bool isDown();
		bool isUp();
		bool isJustDown();
		bool isJustUp();

	private:
		void update();

	private:
		volatile bool m_justDown;
		volatile bool m_justUp;
		volatile KeyState m_state;

		uint32_t m_time;

		GPIO_TypeDef* m_port;
		uint16_t m_pin;
	};

	class Input
	{
	public:
		static void init();
	};

};

#endif /* DEVICE_INPUT_H_ */
