/*
 * game.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */


#include "app/gs/game.h"
#include "app/gs/mainmenu.h"
#include "app/application.h"
#include "device/display.h"
#include "device/network.h"
#include "device/system.h"
#include "app/messages.h"

#include <cstring>
#include <stdlib.h>

extern const unsigned char tile_wall_bmp [];
extern const unsigned char tile_block_bmp [];

namespace gs
{

Game Game::s_instance = Game();

void Game::messageReceived(uint8_t messageType, uint8_t dataSize, void* data)
{
	switch (messageType)
	{
		case MSG_HELLO:
		{
			break;
		}
	}
}

void Game::update(uint32_t dt)
{
	//
}

void Game::release()
{
	//
}

void Game::render()
{
	Tile* tiles = m_tiles;

	for (uint8_t j = 0; j < MAP_HEIGHT; j++)
	{
		uint8_t jk = MAP_OFFSET_Y + j * 8;

		for (uint8_t i = 0; i < MAP_WIDTH; i++)
		{
			Tile tile = *tiles++;
			const unsigned char* tile_image;

			switch (tile)
			{
				case Tile::wall:
				{
					tile_image = tile_wall_bmp;
					break;
				}

				case Tile::block:
				{
					tile_image = tile_block_bmp;
					break;
				}

				case Tile::empty:
				default:
				{
					continue;
				}
			}

			device::Display::drawImage(
				MAP_OFFSET_X + i * 8, jk, 8, 8, tile_image);
		}
	}

	device::Display::flush();
}

void shuffle(uint8_t *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          uint8_t t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

void Game::generate()
{
	uint8_t pool[MAP_WIDTH * MAP_HEIGHT];

	Tile* tiles = m_tiles;
	uint8_t cnt = 0;

	for (uint8_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (uint8_t i = 0; i < MAP_WIDTH; i++)
		{
			Tile tile = *tiles++;

			if (tile == Tile::empty)
			{
				if (j < 2 && i < 2)
				{
					continue;
				}

				if (j >= MAP_HEIGHT - 2 && i >= MAP_WIDTH - 2)
				{
					continue;
				}

				pool[cnt++] = (j << 4) | i;
			}
		}
	}

	srand(HAL_GetTick());

	shuffle(pool, cnt);

	// only half the cells should be filled
	cnt = cnt >> 1;

	for (uint8_t i = 0; i < cnt; i++)
	{
		uint8_t v = pool[i];
		uint8_t x = v & 0xF;
		uint8_t y = v >> 4;
		setTile(x, y, Tile::block);
	}
}

void Game::serialize(msg::Map* mapMsg)
{
	uint8_t data_bit = 0;
	uint8_t* data = mapMsg->data;

	Tile* tiles = m_tiles;

	for (uint8_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (uint8_t i = 0; i < MAP_WIDTH; i++)
		{
			Tile tile = *tiles++;

			if (tile == Tile::wall)
			{
				continue;
			}

			uint8_t b = tile == Tile::block ? 0x01 : 0x00;

			if (b)
			{
				*data = *data | (1 << data_bit);
			}

			if (++data_bit >= 8)
			{
				data++;
				data_bit = 0;
			}
		}
	}
}

void Game::deserialize(msg::Map* mapMsg)
{
	uint8_t data_bit = 0;
	uint8_t* data = mapMsg->data;
	uint8_t v = *data;

	Tile* tiles = m_tiles;

	for (uint8_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (uint8_t i = 0; i < MAP_WIDTH; i++)
		{
			Tile tile = *tiles++;

			if (tile == Tile::wall)
			{
				continue;
			}

			if (v & 0x1)
			{
				setTile(i, j, Tile::block);
			}

			v = v >> 1;

			if (++data_bit >= 8)
			{
				data++;
				v = *data;
				data_bit = 0;
			}
		}
	}
}

void Game::init()
{
	memset(m_tiles, 0x00, MAP_WIDTH * MAP_HEIGHT);

	for (uint8_t j = 1; j < 6; j += 2)
	{
		for (uint8_t i = 0; i < 16; i += 2)
		{
			setTile(i, j, Tile::wall);
		}
	}

	if (device::System::isHost())
	{
		generate();
	}

}

void Game::setTile(uint8_t x, uint8_t y, Tile tile)
{
	m_tiles[y * MAP_WIDTH + x] = tile;
}

Tile Game::getTile(uint8_t x, uint8_t y) const
{
	return m_tiles[y * MAP_WIDTH + x];
}

Game* Game::switchTo()
{
	app::Application::s_instance->switchTo(&s_instance);
	return &s_instance;
}

void Game::started()
{
	device::Display::invertDisplay(false);
	device::Display::clear();

	render();
}

}






