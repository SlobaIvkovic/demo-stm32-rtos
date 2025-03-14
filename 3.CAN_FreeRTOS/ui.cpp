#include "ui.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "cmsis_os.h"

/* --BSP includes. */
#include "TermColor.h"
using Terminal=Platform::BSP::TermColor;


#include "appl.h"
#include "canid.h"


#ifdef ENABLE_COCKPIT
#include "cockpit.h"
#endif
#ifdef ENABLE_COMFORT
#include "comfort.h"
#endif
#ifdef ENABLE_GEARBOX
#include "gearshift.h"
#endif
#ifdef ENABLE_ABS
#include "brake.h"
#endif

static const char* LEGO_PRETTY_NAME[]={"SUPER CAR 8070","4x4 CRAWLER","VW T1 BULLY","MINI COOPER"};

static const char* usage=
		"[!] CLEAR SCREEN\r\n";



/* --UI Task. */
void preamble() {
	/* --Some information. */
	Terminal::getInstance().setCol(Terminal::Color::green);
	printf("*** LEGO CAR %s ECU CONTROL***.\r\n%s\r\n",LEGO_PRETTY_NAME[MY_LEGO_CAR],usage);
}

/* --Handle the input by user or drunken monkey. */
void processInput(char k) {
	switch (k) {
	case '!':
		Terminal::getInstance().setAtt(Terminal::Attribute::home);
		Terminal::getInstance().setAtt(Terminal::Attribute::cls);
		break;
	case 'B':
		antiLockBrake->send(BRAKE_EVENT_PRESS);
		break;
	case 'R':
		antiLockBrake->send(BRAKE_EVENT_RELEASE);
		break;
	case 'S':
		gearShifter->send(GEARSHIFT_EVENT_UP);
		break;
	case 'D':
		gearShifter->send(GEARSHIFT_EVENT_DOWN);
		break;
	default:
		printf("INVALID INPUT %c\r\n",k);
	}
}

/* -- UI-Task. */
UI::UI():RTOS::SimpleTask(1024,"UI",osPriorityBelowNormal) {}

/* --UI-main. */
bool UI::m_task() {
	/* --Clean screen. */
	Terminal::getInstance().setAtt(Terminal::Attribute::home);
	Terminal::getInstance().setAtt(Terminal::Attribute::cls);
	while (true) {
		/* --Dump some initial information. */
		preamble();
		/* --Wait for user input. */
		char input;
		scanf("%c",&input);
		processInput(input);

	}
}



