/*
 * input.h
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#ifndef DEVICE_INPUT_H_
#define DEVICE_INPUT_H_

#include <stdint.h>
#include "stm32f1xx.h"
#include <map>

namespace device
{
	class Key
	{
	public:
		Key(GPIO_TypeDef* port, uint16_t pin);

		void update();
		bool getValue() const;

	private:
		bool m_value;

		GPIO_TypeDef* m_port;
		uint16_t m_pin;
	};

	class Input
	{
	public:
		typedef std::map<uint16_t, Key*> Keys;

	public:
		static void init();

	public:
		static void updateKey(uint16_t key);
		static void registerKey(uint16_t key_port, Key* key);

	private:
		static Keys m_keys;
	};

};

#endif /* DEVICE_INPUT_H_ */
