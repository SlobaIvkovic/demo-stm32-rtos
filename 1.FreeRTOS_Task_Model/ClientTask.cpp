/*
 * ClientTask.cpp
 *
 *  Created on: 23.06.2016
 *      Author: Wuebbelmann
 */

#include "ClientTask.h"
#include "cmsis_os.h"

#include <cinttypes>

#include <cstdio>

extern osThreadId ServerTaskHandle;
extern osThreadId ClientTaksHandle1;


void ClientTask1(const void * argument)
{
	int32_t rt;
	int32_t toSend = 0x01;
	for(;;)
	{
		osDelay(250);
		if((int32_t)0x80000000 == (rt = osSignalSet(ServerTaskHandle, toSend)))
		{
			printf("Client1 send error\r\n");
		}

	}

}

void ClientTask2(const void * argument)
{
	int32_t rt;
	int32_t toSend = 0x10;
	for(;;)
	{
		osDelay(1000);

		if((int32_t)0x80000000 == (rt = osSignalSet(ServerTaskHandle, toSend)))
		{
			printf("Client2 send error\r\n");
		}


	}
}
