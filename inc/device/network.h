/*
 * network.h
 *
 *  Created on: Feb 27, 2018
 *      Author: desertkun
 */

#ifndef DEVICE_NETWORK_H_
#define DEVICE_NETWORK_H_

#include <stdint.h>

extern "C"
{
	#include "nrf24l01.h"
}

namespace device
{

	class Network
	{
	public:
		static void init();

	public:
		static void interrupt();
		static bool send(uint8_t command, uint8_t dataSize, const void* data);
		static void sendNoAck(uint8_t command, uint8_t dataSize, const void* data);

	private:

		static nrf24l01 s_nrf;
		static uint8_t s_tx_buffer[64];
		static uint8_t s_rx_buffer[64];

		static uint8_t s_0_address[5];
		static uint8_t s_1_address[5];
	};

};

#endif /* DEVICE_NETWORK_H_ */
