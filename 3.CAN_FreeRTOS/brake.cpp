#include <ignition.h>
#include "brake.h"

#include <cstdio>

#include "cmsis_os.h"


/* --Globals. */
#define MBOX_SIZE	10
osMessageQDef(mbox,MBOX_SIZE,AntiLockBrakeEvent);

/* --brightness values. */
#define BRIGHTNESS_ON	0xff
#define BRIGHTNESS_HALF	0x40
#define BRIGHTNESS_OFF	0x00

/* --Anti-lock brake state. */
static AntiLockBrakeState brakeState=BRAKE_RELEASE;
/* --Ignition state. */
static IgnitionState ignitionState=IGNITION_STATE_FAILURE;


void transitionAntiLockBrake(enum AntiLockBrakeEvent ev) {
	/* --TODO: State machine transitions. */

	if (ev == BRAKE_EVENT_PRESS)
	{
		brakeState = BRAKE_PRESS;
	}

	else if(ev == BRAKE_EVENT_RELEASE)
	{
		brakeState = BRAKE_RELEASE;
	}
}

AntiLockBrake::AntiLockBrake():RTOS::ListenerTask(nullptr,1024,"AntiLockBrake",osPriorityBelowNormal) {
	/* --Create the box. */
	_queue=osMessageCreate(osMessageQ(mbox),nullptr);
}

/* --ABS-main. */
void AntiLockBrake::onEvent(osEvent msg) {
	AntiLockBrakeEvent event=static_cast<AntiLockBrakeEvent>(msg.value.v);

	/* --Check if ignition event. */
	if (event==BRAKE_EVENT_LOCK || event==BRAKE_EVENT_ACC || event==BRAKE_EVENT_ON || event==BRAKE_EVENT_START) {
		/* --Set ignition state. */
		switch (event) {
		case BRAKE_EVENT_LOCK: ignitionState=IGNITION_STATE_LOCK; break;
		case BRAKE_EVENT_ACC: ignitionState=IGNITION_STATE_ACC; break;
		case BRAKE_EVENT_ON: ignitionState=IGNITION_STATE_ON; break;
		case BRAKE_EVENT_START: ignitionState=IGNITION_STATE_START; break;

		case BRAKE_EVENT_PRESS: brakeState = BRAKE_PRESS; break;
		case BRAKE_EVENT_RELEASE: brakeState = BRAKE_RELEASE; break;

		default: ignitionState=IGNITION_STATE_FAILURE;
		}
	}

	/* --Transition only if ignition state is not failure state. */
	if (ignitionState!=IGNITION_STATE_FAILURE) {
		/* --Switch on the received event. */
		transitionAntiLockBrake(event);
		/* --States ok?. */
		if (BRAKE_FAILURE==brakeState)
			printf("ANTI-LOCK BRAKE STATE MACHINE ERROR:\r\n");
	}
}

void frmBrake(uint8_t& left, uint8_t& right) {
	/*--TODO: Fill data depending on current state. */
	if(ignitionState != IGNITION_STATE_LOCK)
	{
		if(brakeState == BRAKE_PRESS)
		{
			left = BRIGHTNESS_ON;
			right = BRIGHTNESS_ON;
		}
		else
		{
			left = BRIGHTNESS_OFF;
			right = BRIGHTNESS_OFF;
		}
	}
	else
	{
		left = BRIGHTNESS_OFF;
		right = BRIGHTNESS_OFF;
	}

}
