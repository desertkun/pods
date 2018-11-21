

#ifndef APP_GAMESTATE_H_
#define APP_GAMESTATE_H_

#include <stdint.h>

namespace gs
{

enum class ID
{
	intro,
	mainmenu,
	search,
	game
};

class GameState
{
public:
	virtual ID getID() const = 0;
	virtual void update(uint32_t dt) = 0;
	virtual void release() = 0;
	virtual void init() = 0;
	virtual void messageReceived(uint8_t messageType, uint8_t dataSize, void* data) {};
};

};

#endif
