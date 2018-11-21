/*
 * application.h
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#ifndef APP_APPLICATION_H_
#define APP_APPLICATION_H_

#include "device/input.h"
#include "app/gs/gamestate.h"

namespace app
{

	class Application
	{
	public:
		static Application* s_instance;

	public:
		Application();

	public:
		void switchTo(gs::GameState* gs);
		void loop();
		void messageReceived(uint8_t dataSize, uint8_t* data);

		device::Key& getKeyA() { return m_keyA; }
		device::Key& getKeyB() { return m_keyB; }
		device::Key& getKeyUp() { return m_keyUp; }
		device::Key& getKeyDown() { return m_keyDown; }
		device::Key& getKeyLeft() { return m_keyLeft; }
		device::Key& getKeyRight() { return m_keyRight; }

	private:
		device::Key m_keyA;
		device::Key m_keyB;

		device::Key m_keyUp;
		device::Key m_keyDown;
		device::Key m_keyLeft;
		device::Key m_keyRight;

		gs::GameState* m_gamestate;
	};

};


#endif /* APP_APPLICATION_H_ */
