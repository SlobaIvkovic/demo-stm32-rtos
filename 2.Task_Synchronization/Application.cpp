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

#include <Ringbuffer.h>

#include <ConsumerProducer.h>
#include <DebugTask.h>
#include <TextGenerator.h>
#include <TermColor.h>


using Platform::BSP::cout;
using Platform::BSP::Manipulators::endl;



static const uint32_t bufferSize = (1 << 4) ;   // we want to have 16 elements in buffer.
static char mem[bufferSize];

/* ringbuffer stores data, producer and consumer need reference or pointer to ringbuffer. */
Ringbuffer<char> ringbuffer(bufferSize, '?', mem);


osThreadId ProducerTaskHandle;
osThreadId ConsumerTaskHandle1;
osThreadId ConsumerTaskHandle2;

osThreadDef(ProducerTask, osPriorityNormal, 1, 1024);
// 2 Instances
osThreadDef(ConsumerTask, osPriorityNormal, 2, 1024);


osSemaphoreId CountUpSemaphoreHandle;
osSemaphoreDef(CountUpSemaphore);
osSemaphoreId CountDownSemaphoreHandle;
osSemaphoreDef(CountDownSemaphore);

osMutexDef(BufferMutex);
osMutexId BufferMutexHandle;

osTimerDef(periodic, TextGenerator);
osTimerId periodicId;


osMessageQDef(MessageQueue, 5, char*);
osMessageQId MessageQueueId;

Application::Application()
{
 /* TODO: setup all tasks.
  * create semaphores and mutexe used by the tasks.
  */

	uint32_t i = 0;
	for(i = 0; i < bufferSize; i++)
	{
		mem[i++] = '?';
	}

    cout << Platform::BSP::TermColor::cls
         << Platform::BSP::TermColor::blue
         << "Starting Application"
         << Platform::BSP::TermColor::stdcol
         << endl;

    // pointer points to enumerator which has value blue
    Platform::BSP::TermColor::Color col;
    int colo = Platform::BSP::TermColor::green;
    col = Platform::BSP::TermColor::blue;

   if(NULL == ( periodicId = osTimerCreate(osTimer(periodic), osTimerPeriodic, (void *)0)))
   {
	   printf("Timer creation failed\n\r");
   }
    osTimerStart(periodicId, 5000);

    if(NULL == (MessageQueueId = osMessageCreate(osMessageQ(MessageQueue), NULL)))
    {
    	printf("QUEUE FAILED\n\r");
    }

    if(NULL == (BufferMutexHandle = osMutexCreate(osMutex(BufferMutex))))
    {
    	printf("FATAL ERROR\n\r");
    }
    else
    {
    	osMutexRelease(BufferMutexHandle);
    }

    if(NULL == (CountUpSemaphoreHandle = osSemaphoreCreate(osSemaphore(CountUpSemaphore), 0)))
    {
    	printf("FATAL ERROR\n\r");
    }
    if(NULL == (CountDownSemaphoreHandle = osSemaphoreCreate(osSemaphore(CountDownSemaphore), 15)))
    {
        	printf("FATAL ERROR\n\r");
    }

    //Create Producer task
    if (NULL == (ProducerTaskHandle = osThreadCreate(osThread(ProducerTask), (void *) &colo)))
    {
    	/* --Big problem. */
    	printf("FATAL ERROR\n\r");
    }
    if (NULL == (ConsumerTaskHandle1 = osThreadCreate(osThread(ConsumerTask),NULL)))
    {

    	printf("FATAL ERROR\n\r");
    }
    // Consumer task second instance
    if (NULL == (ConsumerTaskHandle2 = osThreadCreate(osThread(ConsumerTask),NULL)))
    {

    	    printf("FATAL ERROR\n\r");
    }
    //Create two consumer tasks
}

Application::~Application()
{

}



static uint8_t appMem[sizeof(Application)];


static Application* appPointer = nullptr;


extern "C"
void startApplication()
{
    /* --Initialize the kernel. */
        osKernelInitialize ();

        appPointer = new(appMem) Application;
        /* --Run the kernel. */
        osKernelStart ();

}



