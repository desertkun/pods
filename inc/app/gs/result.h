
#ifndef APP_GS_RESULT_H_
#define APP_GS_RESULT_H_

#include "gamestate.h"
#include "app/messages.h"

namespace gs
{

class Result: public GameState
{
private:
	static Result s_instance;

public:
	static void switchTo(bool won);

public:
	virtual ID getID() const override { return ID::result; }
	virtual void update(uint32_t dt) override;
	virtual void release() override;
	virtual void init() override;
	virtual void messageReceived(uint8_t messageType, uint8_t dataSize, void* data) override;

private:
	void disconnect();
	void generate();

private:
	bool m_won;
	msg::Map m_mapMsg;
	bool m_gotMapMsg;
	uint32_t m_timer;
};

};


#endif /* APP_GS_RESULT_H_ */
