/*
 * network.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: desertkun
 */

#include <device/network.h>
#include <device/system.h>
#include "app/application.h"

#include <cstring>

extern "C"
{
	#include "main.h"
	#include "spi.h"

	void nrf_packet_received_callback(nrf24l01* dev, uint8_t* data)
	{
		uint8_t dataSize = *data++;
		app::Application::s_instance->messageReceived(dataSize, data);
	}

}

namespace device
{

nrf24l01 Network::s_nrf = nrf24l01();
uint8_t Network::s_tx_buffer[64] = {};
uint8_t Network::s_rx_buffer[64] = {};

uint8_t Network::s_0_address[5] = {};
uint8_t Network::s_1_address[5] = {};

void Network::interrupt()
{
	nrf_irq_handler(&s_nrf);
}

bool Network::send(uint8_t command, uint8_t dataSize, const void* data)
{
	s_tx_buffer[0] = dataSize;
	s_tx_buffer[1] = command;

	if (dataSize > 0)
		std::memcpy((void*)&s_tx_buffer[2], data, dataSize);

	return nrf_send_packet(&s_nrf, s_tx_buffer) == NRF_OK;
}

void Network::sendNoAck(uint8_t command, uint8_t dataSize, const void* data)
{
	s_tx_buffer[0] = dataSize;
	s_tx_buffer[1] = command;

	if (dataSize > 0)
		std::memcpy((void*)&s_tx_buffer[2], data, dataSize);

	nrf_send_packet_noack(&s_nrf, s_tx_buffer);
}

void Network::init()
{
	const uint8_t addr_a[4] = {0xC2, 0xC2, 0xC2, 0x5D};
	const uint8_t addr_b[4] = {0xC2, 0xC2, 0xC2, 0xE7};

	if (System::isHost())
	{
		std::memcpy((void*)s_0_address, (const void*)addr_a, 4);
		std::memcpy((void*)s_1_address, (const void*)addr_b, 4);
	}
	else
	{
		std::memcpy((void*)s_0_address, (const void*)addr_b, 4);
		std::memcpy((void*)s_1_address, (const void*)addr_a, 4);
	}

	{
		nrf24l01_config config;
		config.data_rate        = NRF_DATA_RATE_1MBPS;
		config.tx_power         = NRF_TX_PWR_0dBm;
		config.crc_width        = NRF_CRC_WIDTH_2B;
		config.addr_width       = NRF_ADDR_WIDTH_4;
		config.payload_length   = 16;    // maximum is 32 bytes
		config.retransmit_count = 15;   // maximum is 15 times
		config.retransmit_delay = 0x0F; // 4000us, LSB:250us

		config.rx_address = s_0_address;
		config.tx_address = s_1_address;

		config.rf_channel = 39;
		config.rx_buffer = s_rx_buffer;

		config.spi         = &hspi1;
		config.spi_timeout = 10; // milliseconds
		config.ce_port     = NRF_CE_GPIO_Port;
		config.ce_pin      = NRF_CE_Pin;
		config.csn_port    = NRF_CS_GPIO_Port;
		config.csn_pin     = NRF_CS_Pin;
		config.irq_port    = NRF_IRQ_GPIO_Port;
		config.irq_pin     = NRF_IRQ_Pin;

		nrf_init(&s_nrf, &config);
	}
}

};


