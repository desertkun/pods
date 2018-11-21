/*
 * intro.h
 *
 *  Created on: Mar 11, 2018
 *      Author: desertkun
 */

#ifndef APP_GS_INTRO_H_
#define APP_GS_INTRO_H_

#include "gamestate.h"

namespace gs
{

class Intro: public GameState
{
private:
	static Intro s_instance;

public:
	static void switchTo();

public:
	virtual ID getID() const override { return ID::intro; }
	virtual void update(uint32_t dt) override;
	virtual void release() override;
	virtual void init() override;
};

};


#endif /* APP_GS_INTRO_H_ */
