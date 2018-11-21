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
	void serialize(msg::Map* mapMsg);
	void deserialize(msg::Map* mapMsg);
	void started();

private:
	void render();
	void generate();

	void setTile(uint8_t x, uint8_t y, Tile tile);
	Tile getTile(uint8_t x, uint8_t y) const;

private:
	Tile m_tiles[MAP_WIDTH * MAP_HEIGHT];
};

};


#endif /* APP_GS_GAME_H_ */
