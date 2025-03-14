#include <ignition.h>
#include "comfort.h"

#include <cstdio>

#include "cmsis_os.h"


/* --Globals. */
#define MBOX_SIZE	10
osMessageQDef(mbox,MBOX_SIZE,ComfortEvent);

/* --brightness values. */
#define BRIGHTNESS_ON	0xff
#define BRIGHTNESS_HALF	0x40
#define BRIGHTNESS_OFF	0x00

static ComfortState ambientDriverState=COMFORT_FAILURE;
static ComfortState ambientPassengerState=COMFORT_FAILURE;

/* --Door state. */
static DoorState doorState;

/* --Ignition state. */
static IgnitionState ignitionState=IGNITION_STATE_FAILURE;


void transitionAmbient(ComfortEvent ev) {
	/* --TODO: State machine transitions. */
}

Comfort::Comfort():RTOS::ListenerTask(nullptr,1024,"Comfort",osPriorityBelowNormal) {
	/* --Create the box. */
	_queue=osMessageCreate(osMessageQ(mbox),nullptr);
}

/* --Comfort-main. */
void Comfort::onEvent(osEvent msg) {
	ComfortEvent event=static_cast<ComfortEvent>(msg.value.v);

	/* --Check if ignition event. */
	if (event==COMFORT_EVENT_LOCK || event==COMFORT_EVENT_ACC || event==COMFORT_EVENT_ON || event==COMFORT_EVENT_START) {
		/* --Set ignition state. */
		switch (event) {
		case COMFORT_EVENT_LOCK: ignitionState=IGNITION_STATE_LOCK; break;
		case COMFORT_EVENT_ACC: ignitionState=IGNITION_STATE_ACC; break;
		case COMFORT_EVENT_ON: ignitionState=IGNITION_STATE_ON; break;
		case COMFORT_EVENT_START: ignitionState=IGNITION_STATE_START; break;
		default: ignitionState=IGNITION_STATE_FAILURE;
		}
	}

	/* --Transition only if ignition state is not failure state. */
	if (ignitionState!=IGNITION_STATE_FAILURE) {
		/* --Switch on the received event. */
		transitionAmbient(event);

		/* --States ok?. */
		if (COMFORT_FAILURE==ambientDriverState || COMFORT_FAILURE==ambientPassengerState)
			printf("AMBIENT STATE MACHINE ERROR: %u, %u\r\n",ambientDriverState,ambientPassengerState);
	}

}

void frmAmbient(uint8_t& driver, uint8_t& passenger) {
	/*--TODO: Fill data depending on current state. */
}

void frmDoorStatus(uint8_t& door) {
	/*--TODO: Fill data depending on current state. */
}


