/*
 * ConsumerProducer.cpp
 *
 *  Created on: 14.09.2015
 *      Author: Wuebbelmann
 */


#include <ConsumerProducer.h>

#include <TermColor.h>

// added
#include "cmsis_os.h"
#include "Ringbuffer.h"
extern const char text[];
/* TODO: implement consumer and producer task.
 * Implementation can be done in C style or by using an OOP approach.
 */

extern const char txt[];

extern osSemaphoreId CountUpSemaphoreHandle;
extern osSemaphoreId CountDownSemaphoreHandle;

extern Ringbuffer<char> ringbuffer;

using Platform::BSP::cout;
using Platform::BSP::Manipulators::endl;

extern osMutexId BufferMutexHandle;

extern osMessageQId MessageQueueId;

void ProducerTask(const void * arg)
{
	char * ptr;
	osEvent evt;


	int clr = Platform::BSP::TermColor::blue;

	int* inpt;
	inpt = (int*)arg;


	Platform::BSP::TermColor::Color colo;


	colo = (Platform::BSP::TermColor::Color)(*inpt);

	cout << Platform::BSP::TermColor::cls
	         << colo
	         << "Starting Producer"

	         << endl;

	int counter = 0;
	int rcv = 0;
	for(;;)
	{

		// Receive message from the text generator
		evt = osMessageGet(MessageQueueId, 10);
		if (evt.status == osEventMessage)
		{
			 ptr = (char*)evt.value.p;
			 // if message is received again set counter to 0
			 cout << "Message Received"<< endl;
			 cout << ptr[0]<<ptr[1]<<ptr[2]<<endl;
			 counter = 0;
			 rcv = 1;
		}
		// semaphore take
		if(rcv == 1)
		{
			osSemaphoreWait(CountDownSemaphoreHandle, osWaitForever); //decrease tokens

			osMutexWait(BufferMutexHandle, osWaitForever);
			ringbuffer<< ptr[counter];
			cout<<"Pr "<<ptr[counter]<<endl;
			counter++;
			osMutexRelease(BufferMutexHandle);

			osSemaphoreRelease(CountUpSemaphoreHandle);
		}
	}

}

void ConsumerTask(const void * arg)
{
	char toPrint;

	for(;;)
	{


		osSemaphoreWait(CountUpSemaphoreHandle, osWaitForever); //decrease tokens

		osMutexWait(BufferMutexHandle, osWaitForever);
		ringbuffer>>toPrint;
		cout<<"Con "<<toPrint<<endl;
		osMutexRelease(BufferMutexHandle);

		osSemaphoreRelease(CountDownSemaphoreHandle);


	}

}
