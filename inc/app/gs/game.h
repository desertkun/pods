/*
 * game.h
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#ifndef APP_GS_GAME_H_
#define APP_GS_GAME_H_

#include "gamestate.h"
#include "app/messages.h"

#define MAP_WIDTH 16
#define MAP_HEIGHT 7

#define MAP_OFFSET_X 0
#define MAP_OFFSET_Y 8

namespace gs
{

enum class Tile
{
	empty = 0,
	wall,
	block,
	bonusBombs,
	bonusSpeed,
	bonusExplosion
};

class Game;

enum class PlayerState
{
	normal = 0,
	moving = 1
};

struct Player
{
	PlayerState state;
	uint8_t x, y, targetX, targetY, movingTimer, movedBy;
	int8_t offsetX, offsetY;
	Game* game;
	const unsigned char* sprite;

	void init(uint8_t x, uint8_t y, Game* game, const unsigned char* sprite);
	void update(uint32_t dt);
	void moveTo(uint8_t x, uint8_t y);
	void render();
};

class Game: public GameState
{
private:
	static Game s_instance;

public:
	static Game* switchTo();

public:
	virtual ID getID() const override { return ID::search; }
	virtual void update(uint32_t dt) override;
	virtual void release() override;
	virtual void init() override;
	virtual void messageReceived(uint8_t messageType, uint8_t dataSize, void* data) override;

public:
	void setDirty() { m_dirty = true; }

	void serialize(msg::Map* mapMsg);
	void deserialize(msg::Map* mapMsg);
	void started();

private:
	void sendMyPosition();
	void clearCell(uint8_t x, uint8_t y);
	void renderCell(uint8_t x, uint8_t y, const unsigned char* image);
	void getReady();
	void render();
	void generate();
	void build();
	void disconnect();

	const Player& getMyPlayer() const { return m_players[m_myPlayerId]; }
	const Player& getOtherPlayer() const { return m_players[1 - m_myPlayerId]; }

	Player& getMyPlayer() { return m_players[m_myPlayerId]; }
	Player& getOtherPlayer() { return m_players[1 - m_myPlayerId]; }

	void setTile(uint8_t x, uint8_t y, Tile tile);
	Tile getTile(uint8_t x, uint8_t y) const;

private:
	Tile m_tiles[MAP_WIDTH * MAP_HEIGHT];
	Player m_players[2];
	uint8_t m_myPlayerId;
	uint32_t m_seed;

	uint32_t m_refreshTimer;
	bool m_dirty;

	msg::Move m_moveMsg;
	bool m_gotMoveMsg;
};

};


#endif /* APP_GS_GAME_H_ */
