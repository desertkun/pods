/*
 * game.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */


#include "app/gs/game.h"
#include "app/gs/mainmenu.h"
#include "app/gs/result.h"
#include "app/application.h"
#include "device/display.h"
#include "device/network.h"
#include "device/system.h"
#include "app/messages.h"

#include <cstring>
#include <stdlib.h>

extern const unsigned char tile_wall_bmp [];
extern const unsigned char tile_block_bmp [];
extern const unsigned char tile_bomb_1_bmp [];
extern const unsigned char tile_bomb_2_bmp [];
extern const unsigned char tile_bomb_3_bmp [];
extern const unsigned char tile_bomb_4_bmp [];
extern const unsigned char tile_character_a_bmp [];
extern const unsigned char tile_character_b_bmp [];
extern const unsigned char tile_explosion_1_bmp [];
extern const unsigned char tile_explosion_2_bmp [];
extern const unsigned char tile_star_1_bmp [];
extern const unsigned char tile_star_2_bmp [];
extern const unsigned char tile_empty_bmp [];


namespace gs
{

Game Game::s_instance = Game();

int8_t sgn(int8_t val) {
    return (int8_t(0) < val) - (val < int8_t(0));
}

const uint32_t Bomb::TIME = 4000;
const uint32_t Bomb::EXPLOSION_TIME = 600;


void Bomb::place(uint8_t x, uint8_t y, uint8_t power)
{
	this->x = x;
	this->y = y;
	this->power = power;

	timer = 0;
	state = BombState::active;
	_render = true;

	Game::SetDirty();
}

void Bomb::removeExplosions()
{
	state = BombState::normal;
	timer = 0;

	Game& game = Game::s_instance;

	game.setTile(x, y, Tile::empty);

	// left
	for (uint8_t _x = x - 1, p = power; _x >= 0 && p > 0; _x--, p--)
	{
		if (game.getTile(_x, y) != Tile::explosion)
			break;

		game.setTile(_x, y, Tile::empty);
	}

	// right
	for (uint8_t _x = x + 1, p = power; _x <= MAP_WIDTH - 1 && p > 0; _x++, p--)
	{
		if (game.getTile(_x, y) != Tile::explosion)
			break;

		game.setTile(_x, y, Tile::empty);
	}

	// up
	for (uint8_t _y = y - 1, p = power; _y >= 0 && p > 0; _y--, p--)
	{
		if (game.getTile(x, _y) != Tile::explosion)
			break;

		game.setTile(x, _y, Tile::empty);
	}

	// down
	for (uint8_t _y = y + 1, p = power; _y <= MAP_HEIGHT - 1 && p > 0; _y++, p--)
	{
		if (game.getTile(x, _y) != Tile::explosion)
			break;

		game.setTile(x, _y, Tile::empty);
	}

	Game::SetDirty();
}

void Bomb::explode()
{
	Game& game = Game::s_instance;

	game.setTile(x, y, Tile::explosion);

	// left
	for (uint8_t _x = x - 1, p = power; _x >= 0 && p > 0; _x--, p--)
	{
		if (game.checkTile(_x, y))
			break;

		game.setTile(_x, y, Tile::explosion);
	}

	// right
	for (uint8_t _x = x + 1, p = power; _x <= MAP_WIDTH - 1 && p > 0; _x++, p--)
	{
		if (game.checkTile(_x, y))
			break;

		game.setTile(_x, y, Tile::explosion);
	}

	// up
	for (uint8_t _y = y - 1, p = power; _y >= 0 && p > 0; _y--, p--)
	{
		if (game.checkTile(x, _y))
			break;

		game.setTile(x, _y, Tile::explosion);
	}

	// down
	for (uint8_t _y = y + 1, p = power; _y <= MAP_HEIGHT - 1 && p > 0; _y++, p--)
	{
		if (game.checkTile(x, _y))
			break;

		game.setTile(x, _y, Tile::explosion);
	}

	state = BombState::explosion;
	timer = 0;

	Game::SetDirty();
}

void Bomb::update(uint32_t dt)
{
	switch (state)
	{
		case BombState::active:
		{
			timer += dt;

			bool now = timer % 500 > 250;

			if (now != _render)
			{
				_render = now;
				Game::SetDirty();
			}

			if (timer > Bomb::TIME)
			{
				explode();
			}

			break;
		}
		case BombState::explosion:
		{
			timer += dt;

			if (timer > Bomb::EXPLOSION_TIME)
			{
				removeExplosions();
			}

			Game::SetDirty();

			break;
		}
		default:
		{
			break;
		}
	}

}

void Bomb::render()
{
	if (state != BombState::active)
		return;

	const uint8_t* sprite;

	if (timer < 1000)
	{
		sprite = tile_bomb_1_bmp;
	}
	else if (timer < 2000)
	{
		sprite = tile_bomb_2_bmp;
	}
	else if (timer < 3000)
	{
		sprite = tile_bomb_3_bmp;
	}
	else
	{
		sprite = tile_bomb_4_bmp;
	}

	if (!_render)
		return;

	device::Display::drawImage(
		MAP_OFFSET_X + x * 8,
		MAP_OFFSET_Y + y * 8,
		8, 8, sprite);
}

///////////

void Player::moved()
{
	Game& game = Game::s_instance;

	Tile at = game.getTile(x, y);

	if (at == Tile::powerup)
	{
		game.removePowerUp();
		game.setTile(x, y, Tile::empty);

		bombPower++;
	}
}

void Player::update(uint32_t dt)
{
	bomb.update(dt);

	switch (state)
	{
		case PlayerState::moving:
		{
			movingTimer += dt;

			if (movingTimer >= 16)
			{
				movingTimer = 0;

				int8_t signX = sgn((int8_t)targetX - (int8_t)x),
					   signY = sgn((int8_t)targetY - (int8_t)y);

				offsetX += signX;
				offsetY += signY;
				movedBy += 1;

				Game::SetDirty();

				if (movedBy >= 8)
				{
					x = targetX;
					y = targetY;
					offsetX = 0;
					offsetY = 0;
					movedBy = 0;
					movingTimer = 0;
					state = PlayerState::normal;

					moved();
				}
			}


			break;
		}
		default:
		{
			break;
		}
	}
}

void Player::moveTo(uint8_t x, uint8_t y)
{
	targetX = x;
	targetY = y;
	offsetX = 0;
	offsetY = 0;
	movedBy = 0;
	movingTimer = 0;

	state = PlayerState::moving;
}

void Player::init(uint8_t x, uint8_t y, const unsigned char* sprite)
{
	this->x = x;
	this->y = y;
	this->sprite = sprite;

	bombPower = 1;
	offsetX = 0;
	offsetY = 0;
	movingTimer = 0;
	state = PlayerState::normal;
}

void Player::render()
{
	device::Display::drawImage(
		MAP_OFFSET_X + x * 8 + offsetX,
		MAP_OFFSET_Y + y * 8 + offsetY,
		8, 8, sprite);

	bomb.render();
}

/////////


bool Game::checkTile(uint8_t x, uint8_t y)
{
	Tile tile = getTile(x, y);

	if (device::System::isHost())
	{
		for (uint8_t i = 0; i < 2; i++)
		{
			Player& player = m_players[i];

			if (player.x == x && player.y == y)
			{
				m_resultMsg.won = m_myPlayerId == i;

				if (!device::Network::send(MSG_RESULT, sizeof(m_resultMsg), &m_resultMsg))
				{
					disconnect();
					return true;
				}

				Result::switchTo(m_myPlayerId != i);
				return true;
			}
		}
	}

	if (tile == Tile::wall)
		return true;

	if (tile == Tile::block)
	{
		setTile(x, y, Tile::explosion);
		return true;
	}

	if (tile == Tile::blockWithPowerup)
	{
		setTile(x, y, Tile::powerup);
		addPowerUp();
		return true;
	}

	return false;
}

void Game::messageReceived(uint8_t messageType, uint8_t dataSize, void* data)
{
	switch (messageType)
	{
		case MSG_MOVE:
		{
			RECEIVE_MSG(m_moveMsg);
			m_gotMoveMsg = true;
			break;
		}
		case MSG_BOMB:
		{
			RECEIVE_MSG(m_bombMsg);
			m_gotBombMsg = true;
			break;
		}
		case MSG_RESULT:
		{
			RECEIVE_MSG(m_resultMsg);
			m_gotResultMsg = true;
			break;
		}
	}
}

bool Game::checkLocation(uint8_t x, uint8_t y) const
{
	Tile at = getTile(x, y);

	switch (at)
	{
		case Tile::powerup:
		case Tile::empty:
		{
			break;
		}
		default:
		{
			return false;
		}
	}

	const Player& me = getMyPlayer();
	const Player& other = getOtherPlayer();

	if (other.x == x && other.y == y)
		return false;

	if (me.bomb.state == BombState::active && me.bomb.x == x && me.bomb.y == y)
		return false;

	if (other.bomb.state == BombState::active && other.bomb.x == x && other.bomb.y == y)
		return false;

	return true;
}

void Game::update(uint32_t dt)
{
	if (m_refreshTimer >= 16)
	{
		m_refreshTimer = 0;

		if (m_dirty || m_activePowerUps > 0)
		{
			m_dirty = false;
			render();
		}
	}
	else
	{
		m_refreshTimer += dt;
	}

	Player& me = getMyPlayer();
	Player& other = getOtherPlayer();

	me.update(dt);
	other.update(dt);

	app::Application* app = app::Application::s_instance;

	if (me.state == PlayerState::normal)
	{
		if (app->getKeyLeft().isDown())
		{
			if (me.x > 0 && checkLocation(me.x - 1, me.y))
			{
				// we can move left
				me.moveTo(me.x - 1, me.y);
				sendMyPosition();
			}
		}
		else if (app->getKeyRight().isDown())
		{
			if (me.x < MAP_WIDTH - 1 && checkLocation(me.x + 1, me.y))
			{
				// we can move right
				me.moveTo(me.x + 1, me.y);
				sendMyPosition();
			}
		}
		else if (app->getKeyUp().isDown())
		{
			if (me.y > 0 && checkLocation(me.x, me.y - 1))
			{
				// we can move up
				me.moveTo(me.x, me.y - 1);
				sendMyPosition();
			}
		}
		else if (app->getKeyDown().isDown())
		{
			if (me.y < MAP_HEIGHT - 1 && checkLocation(me.x, me.y + 1))
			{
				// we can move down
				me.moveTo(me.x, me.y + 1);
				sendMyPosition();
			}
		}
	}

	if (me.bomb.state == BombState::normal && app->getKeyA().isJustDown())
	{
		me.bomb.place(me.x, me.y, me.bombPower);

		m_bombMsg.x = me.x;
		m_bombMsg.y = me.y;
		m_bombMsg.power = me.bombPower;

		if (!device::Network::send(MSG_BOMB, sizeof(m_bombMsg), &m_bombMsg))
		{
			disconnect();
		}
	}

	if (m_gotMoveMsg)
	{
		m_gotMoveMsg = false;

		other.x = m_moveMsg.x;
		other.y = m_moveMsg.y;
		other.moveTo(m_moveMsg.targetX, m_moveMsg.targetY);
	}

	if (m_gotBombMsg)
	{
		m_gotBombMsg = false;

		other.bomb.place(m_bombMsg.x, m_bombMsg.y, m_bombMsg.power);
	}

	if (m_gotResultMsg)
	{
		m_gotResultMsg = false;

		Result::switchTo(m_resultMsg.won);
	}
}

void Game::disconnect()
{
	device::Display::clear();
	device::Display::drawText(4, 27, "Disconnected :(", device::Display::Font::f_5x7);
	device::Display::flush();

	HAL_Delay(2000);

	gs::MainMenu::switchTo();
}

void Game::sendMyPosition()
{
	Player& me = getMyPlayer();

	m_moveMsg.targetX = me.targetX;
	m_moveMsg.targetY = me.targetY;
	m_moveMsg.x = me.x;
	m_moveMsg.y = me.y;

	if (!device::Network::send(MSG_MOVE, sizeof(m_moveMsg), &m_moveMsg))
	{
		disconnect();
	}
}


void Game::release()
{
	//
}

void Game::clearCell(uint8_t x, uint8_t y)
{
	device::Display::drawImage(
		MAP_OFFSET_X + x * 8, MAP_OFFSET_Y + y * 8,
		8, 8, tile_empty_bmp);
}

void Game::renderCell(uint8_t x, uint8_t y, const unsigned char* image)
{
	device::Display::drawImage(
		MAP_OFFSET_X + x * 8, MAP_OFFSET_Y + y * 8,
		8, 8, image);
}

void Game::render()
{
	device::Display::clear();

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

				case Tile::blockWithPowerup:
				case Tile::block:
				{
					tile_image = tile_block_bmp;
					break;
				}
				case Tile::explosion:
				{
					tile_image = rand() % 2 == 0 ? tile_explosion_1_bmp : tile_explosion_2_bmp;
					break;
				}
				case Tile::powerup:
				{
					tile_image = HAL_GetTick() % 100 > 50 ? tile_star_1_bmp : tile_star_2_bmp;
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

	m_players[0].render();
	m_players[1].render();

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
	m_seed = HAL_GetTick();
}

void Game::build()
{
	uint8_t pool[MAP_WIDTH * MAP_HEIGHT];

	Tile* tiles = m_tiles;
	uint8_t cnt = 0;

	for (uint8_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (uint8_t i = 0; i < MAP_WIDTH; i++)
		{
			Tile tile = *tiles++;

			if (tile != Tile::wall)
			{
				setTile(i, j, Tile::empty);

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

	srand(m_seed);

	shuffle(pool, cnt);

	// only half the cells should be filled
	cnt = cnt >> 1;

	for (uint8_t i = 0; i < cnt; i++)
	{
		uint8_t v = pool[i];
		uint8_t x = v & 0xF;
		uint8_t y = v >> 4;
		setTile(x, y, (i > cnt - 6) ? Tile::blockWithPowerup : Tile::block);
	}

	// free up space for players

	setTile(0, 0, Tile::empty);
	setTile(1, 0, Tile::empty);
	setTile(1, 1, Tile::empty);

	setTile(MAP_WIDTH - 1, MAP_HEIGHT - 1, Tile::empty);
	setTile(MAP_WIDTH - 1, MAP_HEIGHT - 2, Tile::empty);
	setTile(MAP_WIDTH - 2, MAP_HEIGHT - 1, Tile::empty);

	// setup players
	m_players[0].init(0, 0, tile_character_a_bmp);
	m_players[1].init(MAP_WIDTH - 1, MAP_HEIGHT - 1, tile_character_b_bmp);
}

void Game::serialize(msg::Map* mapMsg)
{
	mapMsg->seed = m_seed;
}

void Game::deserialize(msg::Map* mapMsg)
{
	m_seed = mapMsg->seed;
}

void Game::init()
{
	m_gotMoveMsg = false;
	m_gotBombMsg = false;
	m_gotResultMsg = false;
	m_activePowerUps = 0;

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

		m_myPlayerId = 0;
	}
	else
	{
		m_myPlayerId = 1;
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

void Game::getReady()
{
	device::Display::invertDisplay(false);

	device::Display::clear();
	device::Display::drawText(29, 27, "GET READY!", device::Display::Font::f_7x10);
	device::Display::flush();

	HAL_Delay(3000);

	device::Display::drawRefreshSequence();
}

void Game::started()
{
	build();
	getReady();
	render();
}

}






