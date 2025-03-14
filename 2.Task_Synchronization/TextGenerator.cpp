/*
 * TextGenerator.cpp
 *
 *  Created on: 29.06.2016
 *      Author: Wuebbelmann
 */

#include <TextGenerator.h>
#include "cmsis_os.h"
#include<cstdio>


const char text[] =
    "Habe nun, ach! Philosophie,\r\n"
    "Juristerei und Medizin,\r\n"
    "Und leider auch Theologie\r\n"
    "Durchaus studiert, mit heissem Bemuehn.\r\n"
    "Da steh ich nun, ich armer Tor!\r\n"
    "Und bin so klug als wie zuvor;\r\n"
    "Heisse Magister, heisse Doktor gar\r\n"
    "Und ziehe schon an die zehen Jahr\r\n"
    "Herauf, herab und quer und krumm\r\n"
    "Meine Schueler an der Nase herum -\r\n"
    "Und sehe, dass wir nichts wissen koennen!\r\n";


/* TODO: implement textgenenerator task.
 * Implementation can be done in C style or by using an OOP approach.
 */


extern osMessageQId MessageQueueId;

void TextGenerator(const void * arg)
{
	int s;
	s = osMessagePut(MessageQueueId, (uint32_t)text, osWaitForever);
//	printf("Periodic Task status =  %x \n\r", s);
}
