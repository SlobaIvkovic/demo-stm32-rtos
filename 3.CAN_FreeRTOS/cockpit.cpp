#include <ignition.h>
#include "cockpit.h"

#include <cstdio>

#include "cmsis_os.h"



/* --Globals. */
#define MBOX_SIZE	10
osMessageQDef(mbox,MBOX_SIZE,CockpitEvent);

/* --brightness values. */
#define BRIGHTNESS_ON	0xff
#define BRIGHTNESS_HALF	0x40
#define BRIGHTNESS_OFF	0x00

/* --States. */
static BeamState beamState=BEAM_FAILURE;
static FlashState flashState=FLASH_FAILURE;

/* --Left indicator state (parallel state). */
static IndicatorState leftIndicator=INDICATOR_FAILURE;
/* --Right indicator state (parallel state). */
static IndicatorState rightIndicator=INDICATOR_FAILURE;
/* --Ignition state. */
static IgnitionState ignitionState=IGNITION_STATE_FAILURE;


void transitionIndicator(CockpitEvent event) {
	/* --TODO: State machine transitions. */
}

void transitionBeam(CockpitEvent event) {
	/* --TODO: State machine transitions. */
}

Cockpit::Cockpit():RTOS::ListenerTask(nullptr,1024,"Cockpit",osPriorityBelowNormal) {
	/* --Create the box. */
	_queue=osMessageCreate(osMessageQ(mbox),nullptr);
}

/* --Cockpit-main. */
void Cockpit::onEvent(osEvent msg) {

	CockpitEvent event=static_cast<CockpitEvent>(msg.value.v);

	/* --Check if ignition event. */
	if (event==COCKPIT_EVENT_LOCK || event==COCKPIT_EVENT_ACC || event==COCKPIT_EVENT_ON || event==COCKPIT_EVENT_START) {
		/* --Set ignition state. */
		switch (event) {
		case COCKPIT_EVENT_LOCK: ignitionState=IGNITION_STATE_LOCK; break;
		case COCKPIT_EVENT_ACC: ignitionState=IGNITION_STATE_ACC; break;
		case COCKPIT_EVENT_ON: ignitionState=IGNITION_STATE_ON; break;
		case COCKPIT_EVENT_START: ignitionState=IGNITION_STATE_START; break;
		default: ignitionState=IGNITION_STATE_FAILURE;
		}
	}

	/* --Transition only if ignition state is not failure state. */
	if (ignitionState!=IGNITION_STATE_FAILURE) {

		/* --Switch on the received event. */
		transitionIndicator(event);
		/* --States ok?. */
		if (INDICATOR_FAILURE==leftIndicator || INDICATOR_FAILURE==rightIndicator)
			printf("INDICATOR STATE MACHINE ERROR: %d, %d\r\n",leftIndicator,rightIndicator);

		transitionBeam(event);
		if (BEAM_FAILURE==beamState || FLASH_FAILURE==flashState)
			printf("BEAM STATE MACHINE ERROR: %d, %d\r\n",beamState,flashState);

	}
}

/* ZUGRIFF SCHÜTZEN??? */

void frmIndicator(uint8_t& frontLeft, uint8_t& frontRight, uint8_t& rearLeft, uint8_t& rearRight) {
	/*--TODO: Fill data depending on current state. */
}

void frmHighBeam(uint8_t& frontLeft, uint8_t& frontRight) {
	/*--TODO: Fill data depending on current state. */
}

void frmLowBeam(uint8_t& frontLeft, uint8_t& frontRight, uint8_t& rearLeft, uint8_t& rearRight) {
	/*--TODO: Fill data depending on current state. */
}


