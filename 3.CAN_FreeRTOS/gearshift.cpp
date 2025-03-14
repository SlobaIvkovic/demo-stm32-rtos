#include <ignition.h>
#include "gearshift.h"

#include <cstdio>

#include "cmsis_os.h"


/* --Globals. */
#define MBOX_SIZE	10
osMessageQDef(mbox,MBOX_SIZE,GearshiftEvent);

/* --brightness values. */
#define BRIGHTNESS_ON	0xff
#define BRIGHTNESS_HALF	0x40
#define BRIGHTNESS_OFF	0x00

static GearshiftState gearState=GEARSHIFT_P;

/* --Ignition state. */
static IgnitionState ignitionState=IGNITION_STATE_FAILURE;


void transitionGearshift(enum GearshiftEvent ev) {
	/* --TODO: State machine transitions. */

	if(ev == GEARSHIFT_EVENT_UP)
	{
		switch(gearState)
		{
			case GEARSHIFT_P:
				gearState = GEARSHIFT_R;
				break;
			case GEARSHIFT_R:
				gearState = GEARSHIFT_N;
				break;
			case GEARSHIFT_N:
				gearState = GEARSHIFT_D;
				break;
			case GEARSHIFT_D:
				gearState = GEARSHIFT_E;

			case GEARSHIFT_E:
				break;
			default:
				gearState = GEARSHIFT_FAILURE;
				break;
		}
	}
	else if(ev == GEARSHIFT_EVENT_DOWN)
	{
		switch(gearState)
		{
			case GEARSHIFT_E:
				gearState = GEARSHIFT_D;
				break;
			case GEARSHIFT_D:
				gearState = GEARSHIFT_N;
				break;
			case GEARSHIFT_N:
				gearState = GEARSHIFT_R;
				break;
			case GEARSHIFT_R:
				gearState = GEARSHIFT_P;

			case GEARSHIFT_P:
				break;
			default:
				gearState = GEARSHIFT_FAILURE;
				break;
		}
	}

}

GearShifter::GearShifter():RTOS::ListenerTask(nullptr,1024,"GearShifter",osPriorityBelowNormal) {
	/* --Create the box. */
	_queue=osMessageCreate(osMessageQ(mbox),nullptr);
}

/* --GearShifter-main. */
void GearShifter::onEvent(osEvent msg) {

	GearshiftEvent event=static_cast<GearshiftEvent>(msg.value.v);

	/* --Check if ignition event. */
	if (event==GEARSHIFT_EVENT_LOCK || event==GEARSHIFT_EVENT_ACC || event==GEARSHIFT_EVENT_ON || event==GEARSHIFT_EVENT_START) {
		/* --Set ignition state. */
		switch (event) {
		case GEARSHIFT_EVENT_LOCK: ignitionState=IGNITION_STATE_LOCK; break;
		case GEARSHIFT_EVENT_ACC: ignitionState=IGNITION_STATE_ACC; break;
		case GEARSHIFT_EVENT_ON: ignitionState=IGNITION_STATE_ON; break;
		case GEARSHIFT_EVENT_START: ignitionState=IGNITION_STATE_START; break;
		default: ignitionState=IGNITION_STATE_FAILURE;
		}
	}

	/* --Transition only if ignition state is not failure state. */
	if (ignitionState!=IGNITION_STATE_FAILURE) {
		/* --Switch on the received event. */
		transitionGearshift(event);
		/* --States ok?. */
		if (GEARSHIFT_FAILURE==gearState)
			printf("GEARSHIFT STATE MACHINE ERROR: \r\n");
	}
}

void frmReversing(uint8_t& reverse) {
	/*--TODO: Fill data depending on current state. */
	if(gearState == GEARSHIFT_R)
	{
		if(ignitionState != IGNITION_STATE_LOCK)
		{
			reverse = BRIGHTNESS_ON;
		}
		else
		{
			reverse = BRIGHTNESS_OFF;
		}
	}
	else
	{
		reverse = BRIGHTNESS_OFF;
	}

}

void frmShifterPosition(uint8_t& position) {
	/*--TODO: Fill data depending on current state. */
	position = gearState;
}




