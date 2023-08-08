// https://www.youtube.com/watch?v=FCymm6PBtOs
#include <assert.h>

#include "fsm.h"

// typedef Active;
typedef struct TimeBomb TimeBomb;

enum TimeBombSignal {
	SIGNAL_RESERVED = SIGNAL_USER,
	SIGNAL_TB_1,
	SIGNAL_TB_2
};

typedef struct TimeBomb {
	Active super;
	// TimeEvent timeEvent;
	uint32_t blink_counter;
} TimeBomb;

Status TimeBomb_initial(TimeBomb* const me, Event const* const e);
Status TimeBomb_wait4button(TimeBomb* const me, Event const* const e);
Status TimeBomb_blink(TimeBomb* const me, Event const* const e);
Status TimeBomb_pause(TimeBomb* const me, Event const* const e);
Status TimeBomb_boom(TimeBomb* const me, Event const* const e);
Status TimeBomb_armed(TimeBomb* const me, Event const* const e);
Status TimeBomb_defused(TimeBomb* const me, Event const* const e);

void TimeBomb_ctor(TimeBomb* const me) {
	Active_ctor(&me->super, (StateHandler)&TimeBomb_initial);
	// TimeEvent_ctor();
}

Status TimeBomb_initial(TimeBomb* const me, Event const* const e) {
return TRAN(TimeBomb_wait4button);
}

Status TimeBomb_wait4button(TimeBomb* const me, Event const* const e) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	switch(e->signal) {
		case SIGNAL_ENTRY: {
			status = STATUS_HANDLED;
			break;
		}
		case SIGNAL_EXIT: {
			status = STATUS_HANDLED;
			break;
		}
		case SIGNAL_TB_1: {
			status = TRAN(TimeBomb_defused);
			break;
		}
		default:
			status = SUPER(TimeBomb_armed);
			break;
	}
	return status;
}

Status TimeBomb_blink(TimeBomb* const me, Event const* const e) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	switch (e->signal) {
		default:
			status = SUPER(TimeBomb_armed);
			break;
	}
	return status;
}

Status TimeBomb_pause(TimeBomb* const me, Event const* const e) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	switch (e->signal) {
		default:
			status = SUPER(TimeBomb_armed);
			break;
	}
	return status;
}

Status TimeBomb_boom(TimeBomb* const me, Event const* const e) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	switch (e->signal) {
		default:
			status = SUPER(TimeBomb_armed);
			break;
	}
	return status;
}

Status TimeBomb_armed(TimeBomb* const me, Event const* const e) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	switch (e->signal) {
		default:
			status = SUPER(Hsm_top);
			break;
	}
	return status;
}

Status TimeBomb_defused(TimeBomb* const me, Event const* const e) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	switch (e->signal) {
		default:
			status = SUPER(Hsm_top);
			break;
	}
	return status;
}

static TimeBomb timeBomb;
Active* activeObjectTimeBomb = &timeBomb.super;

int main() {
	int8_t error;
	TimeBomb_ctor(&timeBomb);
	//Active_start(activeObjectTimeBomb, 0);
	Active_eventLoop(activeObjectTimeBomb);
	return 0;
}