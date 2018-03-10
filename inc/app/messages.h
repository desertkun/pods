/*
 * messages.h
 *
 *  Created on: Feb 27, 2018
 *      Author: desertkun
 */

#ifndef APP_MESSAGES_H_
#define APP_MESSAGES_H_

#include <stdint.h>

#define MSG_TEST (0)

namespace msg
{
	struct __attribute__ ((packed)) Test
	{
		uint32_t a;
	};

};


#endif /* APP_MESSAGES_H_ */
