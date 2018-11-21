/*
 * mainmenu.h
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#ifndef APP_GS_MAINMENU_H_
#define APP_GS_MAINMENU_H_

#include "gamestate.h"

namespace gs
{

class MainMenu: public GameState
{
private:
	static MainMenu s_instance;

public:
	static void switchTo();

public:
	virtual ID getID() const override { return ID::mainmenu; }
	virtual void update(uint32_t dt) override;
	virtual void release() override;
	virtual void init() override;

private:
	uint32_t m_cnt;
};

};

#endif /* APP_GS_MAINMENU_H_ */
