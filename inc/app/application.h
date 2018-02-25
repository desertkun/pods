/*
 * application.h
 *
 *  Created on: Feb 25, 2018
 *      Author: desertkun
 */

#ifndef APP_APPLICATION_H_
#define APP_APPLICATION_H_

#include "device/input.h"

namespace app
{

	class Application
	{
	public:
		Application();

	public:
		void loop();

		const device::Key& getKeyA() const { return m_keyA; }
		const device::Key& getKeyB() const { return m_keyB; }
		const device::Key& getKeyUp() const { return m_keyUp; }
		const device::Key& getKeyDown() const { return m_keyDown; }
		const device::Key& getKeyLeft() const { return m_keyLeft; }
		const device::Key& getKeyRight() const { return m_keyRight; }

	private:
		device::Key m_keyA;
		device::Key m_keyB;

		device::Key m_keyUp;
		device::Key m_keyDown;
		device::Key m_keyLeft;
		device::Key m_keyRight;
	};

};


#endif /* APP_APPLICATION_H_ */
