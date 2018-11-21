/*
 * search.h
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#ifndef APP_GS_SEARCH_H_
#define APP_GS_SEARCH_H_

#include "gamestate.h"
#include "app/messages.h"

namespace gs
{

class Search: public GameState
{
private:
	static Search s_instance;

public:
	static void switchTo();

public:
	virtual ID getID() const override { return ID::search; }
	virtual void update(uint32_t dt) override;
	virtual void release() override;
	virtual void init() override;
	virtual void messageReceived(uint8_t messageType, uint8_t dataSize, void* data) override;

private:
	void failed();

private:
	uint32_t m_cnt;
	uint32_t m_tim;
	bool m_gotHello;
	msg::Map m_mapMsg;
	bool m_gotMapMsg;
};

};

#endif /* APP_GS_SEARCH_H_ */
