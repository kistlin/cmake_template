// https://www.youtube.com/watch?v=FCymm6PBtOs
#include "hsm.h"

#include <stdio.h>

// typedef Active;
typedef struct TimeBomb TimeBomb;

enum Ascii {
	a = 'a',
	b = 'b',
	d = 'd',
	p = 'p',
	r = 'r'
};

enum TimeBombSignal {
	SIGNAL_RESERVED = EVENT_USER,
	SIGNAL_ARMED = a,
	SIGNAL_BUTTON = b,
	SIGNAL_DISARMED = d,
	SIGNAL_PAUSE = p,
	SIGNAL_RESUME = r,
};

typedef struct TimeBomb {
	Active super;
	// TimeEvent timeEvent;
	uint32_t blink_counter;
} TimeBomb;

Message* TimeBomb_getEvent();
Status TimeBomb_initial(TimeBomb* const me, Message const* const message);
Status TimeBomb_wait4button(TimeBomb* const me, Message const* const message);
Status TimeBomb_blink(TimeBomb* const me, Message const* const message);
Status TimeBomb_pause(TimeBomb* const me, Message const* const message);
Status TimeBomb_boom(TimeBomb* const me, Message const* const message);
Status TimeBomb_armed(TimeBomb* const me, Message const* const message);
Status TimeBomb_disarmed(TimeBomb* const me, Message const* const message);

Message TimeBomb_event;

void TimeBomb_ctor(TimeBomb* const me) {
	Active_ctor(&me->super, (EventHandler)&TimeBomb_initial);
	// TimeEvent_ctor();
}

Message* TimeBomb_getEvent() {
	uint16_t input = 0;
	uint16_t tmp = 0;
	//while (tmp = getchar()) {
	while (tmp = getc(stdin)) {
		if (tmp == '\n' || tmp == '\r') {
			continue;
		}
		else {
			input = tmp;
			break;
		}
	}
	fprintf(stderr, "TimeBomb_getEvent input=%c (0x%02X)\n", input, input);
	TimeBomb_event.event = input;
	return &TimeBomb_event;
}

Status TimeBomb_initial(TimeBomb* const me, Message const* const message) {
	fprintf(stderr, "TimeBomb_initial\n");
	return TRANSITION(TimeBomb_disarmed);
}

Status TimeBomb_wait4button(TimeBomb* const me, Message const* const message) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	fprintf(stderr, "TimeBomb_wait4button (e->event = %d)\n", message->event);
	switch (message->event) {
	case EVENT_ENTRY: {
		status = STATUS_HANDLED;
		break;
	}
	case EVENT_EXIT: {
		status = STATUS_HANDLED;
		break;
	}
	case SIGNAL_DISARMED: {
		status = TRANSITION(TimeBomb_disarmed);
		break;
	}
	default:
		status = SUPER(TimeBomb_armed);
		break;
	}
	return status;
}

Status TimeBomb_blink(TimeBomb* const me, Message const* const message) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	fprintf(stderr, "TimeBomb_blink (e->event = %d)\n", message->event);
	switch (message->event) {
	default:
		status = SUPER(TimeBomb_armed);
		break;
	}
	return status;
}

Status TimeBomb_pause(TimeBomb* const me, Message const* const message) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	fprintf(stderr, "TimeBomb_pause (e->event = %d)\n", message->event);
	switch (message->event) {
	default:
		status = SUPER(TimeBomb_armed);
		break;
	}
	return status;
}

Status TimeBomb_boom(TimeBomb* const me, Message const* const message) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	fprintf(stderr, "TimeBomb_boom (e->event = %d)\n", message->event);
	switch (message->event) {
	default:
		status = SUPER(TimeBomb_armed);
		break;
	}
	return status;
}

Status TimeBomb_armed(TimeBomb* const me, Message const* const message) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	fprintf(stderr, "TimeBomb_armed (e->event = %d)\n", message->event);
	switch (message->event) {
	case EVENT_ENTRY:
		status = TRANSITION(TimeBomb_wait4button);
		break;
	case EVENT_EXIT: {
		status = STATUS_IGNORED;
		break;
	}
	case SIGNAL_DISARMED:
		status = TRANSITION(TimeBomb_disarmed);
		break;
	default:
		status = SUPER(Hsm_top);
		break;
	}
	return status;
}

Status TimeBomb_disarmed(TimeBomb* const me, Message const* const message) {
	Status status;  // not initialized because warnings are turned on if uninitialized
	fprintf(stderr, "TimeBomb_disarmed (e->event = %d)\n", message->event);
	switch (message->event) {
	case EVENT_ENTRY:
		status = STATUS_IGNORED;
		break;
	case EVENT_EXIT:
		status = STATUS_IGNORED;
		break;
	case SIGNAL_ARMED:
		status = TRANSITION(TimeBomb_armed);
		break;
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
	// Active_start(activeObjectTimeBomb, 0);
	Active_eventLoop(activeObjectTimeBomb, &TimeBomb_getEvent);
	return 0;
}
