/*
 * ServerTask.cpp
 *
 *  Created on: 23.06.2016
 *      Author: Wuebbelmann
 */

//#include <inttypes.h>
#include <cstdio>
#include "ServerTask.h"
#include <Application.h>    // needed for events
#include "stm32facade.h"
#include <pca9531.h>        // port expander, LED

#include "cmsis_os.h"

using namespace Platform::BSP;
using namespace std;

extern osThreadId ServerTaskHandle;

//Mutex added
extern osMutexId I2CMutexHandle;


void ServerTask(const void * argument)
{
	PCA9531 LED(STM32Facade::BaseBoard18,STM32Facade::instance(STM32Facade::I2C_Peripheral::UseI2C1));
	unsigned char toggle1;
	unsigned char toggle2;

	 osEvent evt;
	 for(;;)
	 {

		evt = osSignalWait(0x10, osWaitForever); // signal 2
		printf("W2 s2 %x f2 %lx\r\n", evt.status, evt.value.signals);
		osMutexWait(I2CMutexHandle, osWaitForever);
		if(evt.value.signals & 0x10)
		{
				 // drive leds for client 2
			if(!toggle2)
			{
				LED.select((int)(0b00000010));
				toggle2 = 1;
			}
			else
			{
				LED.select((int)(0b00000010),LED.OFF);
				toggle2 = 0;
			}
		}

		// LED1 drive
		if(!toggle1)
		{
			LED.select(int(0b00000001));
			toggle1 = 1;

		}
		else
		{
			LED.select((int)(0b00000001), LED.OFF);
			toggle1 = 0;
		}
		osMutexRelease(I2CMutexHandle);

	 }




	/********************************************

Test code
*******************************************/



}
