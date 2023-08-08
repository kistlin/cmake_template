#ifndef FSM_H
#define FSM_H

#include <assert.h>
#include <stdint.h>

typedef uint16_t Signal;

enum ReservedSignals {
	SIGNAL_INIT,
	SIGNAL_ENTRY,
	SIGNAL_EXIT,
	SIGNAL_USER
};

typedef struct {
	Signal signal;
} Event;

typedef struct Hsm Hsm;

typedef enum {
	STATUS_TRANSITION,
	STATUS_HANDLED,
	STATUS_IGNORED,
	STATUS_INIT,
	STATUS_SUPER,
	STATUS_LAST
} Status;

typedef Status (*StateHandler)(Hsm* const me, Event const* const e);

#define TRAN(target_) (((Hsm*)me)->state = (StateHandler)(target_), STATUS_TRANSITION)

#define SUPER(super_) (((Hsm*)me)->stateSuper = (StateHandler)(super_), STATUS_SUPER)

typedef struct Hsm {
	StateHandler state;
	StateHandler stateSuper;
} Hsm;

void Hsm_ctor(Hsm* const me, StateHandler initial);
void Hsm_init(Hsm* const me, Event const* const e);
void Hsm_dispatch(Hsm* const me, Event const* const e);
Status Hsm_top(Hsm* const me, Event const* const e);

typedef struct Active Active;

//typedef void (*DispatchHandler)(Active* const me, Event const* const e);

struct Active {
	int8_t thread;
	Hsm super;
	//DispatchHandler dispatch;
};

void Active_ctor(Active* const me, StateHandler initial);
void Active_start(Active* const me, uint8_t misc);
void Active_post(Active* const me, Event const* const e);

static Event const eventEntry = {SIGNAL_ENTRY};
static Event const eventExit = {SIGNAL_EXIT};

void Hsm_ctor(Hsm* const me, StateHandler initial) {
	me->state = initial;
}

void Hsm_init(Hsm* const me, Event const* const e) {
	assert(me->state != (StateHandler)0);
	(*me->state)(me, e);
	(*me->state)(me, &eventEntry);
}

void Hsm_dispatch(Hsm* const me, Event const* const e) {
	Status state;
	StateHandler state_previous = me->state;

	//assert((me->state != (StateHandler)0) && (e->signal < SIGNAL_MAX));
	state = (*me->state)(me, e);

	while (state == STATUS_SUPER) {
		state = (*me->stateSuper)(me, e);
	}

	// TODO: implement nested entry/exit action

	if (state == STATUS_TRANSITION) {
		assert(me->state != (StateHandler)0);
		(*state_previous)(me, &eventExit);
		(*me->state)(me, &eventEntry);
	}
}

Status Hsm_top(Hsm* const me, Event const* const e) {
	return STATUS_IGNORED;
}

void Active_ctor(Active* const me, StateHandler initial) {
	Hsm_ctor(&me->super, initial);
}

static void Active_eventLoop(void* pData) {
	Active* me = (Active*)pData;

	Hsm_init(&me->super, (Event*)0);

	while(1) {
		Event* e;
		int8_t error;
		//e = GetEvent();
		assert(error == 0);

		Hsm_dispatch(&me->super, e);
	}
}

#endif  // FSM_H
