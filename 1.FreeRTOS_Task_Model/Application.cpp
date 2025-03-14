/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : application.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 HS Osnabruck.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include <cstdio>
#include <cinttypes>
#include <new>


#include "cmsis_os.h"
#include "main.h"



#include "Application.h"

/* --Project includes. */
#include "ServerTask.h"
#include "ClientTask.h"
#include "ConwayTask.h"
#include "DebugTask.h"

osThreadDef(ServerTask, osPriorityNormal, 1, 1024);
osThreadId ServerTaskHandle;
osThreadDef(ClientTask1, osPriorityNormal, 1, 1024);
osThreadId ClientTaskHandle1;
osThreadDef(ClientTask2, osPriorityNormal, 1, 1024);
osThreadId ClientTaskHandle2;
osThreadDef(ConwayTask, osPriorityNormal, 1, 1024);
osThreadId ConwayTaskHandle;

// Mutex added
osMutexDef(I2CMutex);
osMutexId I2CMutexHandle;


Application::Application()
{
	if (NULL == (ServerTaskHandle = osThreadCreate(osThread(ServerTask),NULL)))
	{
		/* --Big problem. */
		printf("FATAL ERROR\n\r");
	}
	if (NULL == (ClientTaskHandle1 = osThreadCreate(osThread(ClientTask1),NULL)))
	{
			/* --Big problem. */
		printf("FATAL ERROR\n\r");
	}
	if (NULL == (ClientTaskHandle2 = osThreadCreate(osThread(ClientTask2),NULL)))
	{
			/* --Big problem. */
		printf("FATAL ERROR\n\r");
	}
	if (NULL == (ConwayTaskHandle = osThreadCreate(osThread(ConwayTask),NULL)))
	{

		printf("FATAL ERROR\n\r");
	}
	if(NULL == (I2CMutexHandle = osMutexCreate(osMutex(I2CMutex))))
	{
		printf("FATAL ERROR\n\r");
	}

}




static uint8_t appMem[sizeof(Application)];

extern "C"
{
static Application* appPointer = nullptr;
}

extern "C"
void startApplication()
{
    /* --Initialize the kernel. */
        osKernelInitialize ();

        appPointer = new(appMem) Application;
        /* --Run the kernel. */
        osKernelStart ();

}



