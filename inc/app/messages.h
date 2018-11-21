/*
 * messages.h
 *
 *  Created on: Feb 27, 2018
 *      Author: desertkun
 */

#ifndef APP_MESSAGES_H_
#define APP_MESSAGES_H_

#include <stdint.h>

#define MSG_HELLO (0)
#define MSG_MAP (1)

#define RECEIVE_MSG(x) std::memcpy(&x, data, sizeof(x));

namespace msg
{
	struct __attribute__ ((packed)) Map
	{
		uint8_t data[11];
	};

};


#endif /* APP_MESSAGES_H_ */
