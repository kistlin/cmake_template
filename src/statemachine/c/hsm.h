#ifndef HSM_H
#define HSM_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

typedef uint16_t Event;

enum EventsReserved {
	EVENT_INIT,
	EVENT_ENTRY,
	EVENT_EXIT,
	EVENT_USER
};

typedef struct {
	Event event;
} Message;

typedef struct Hsm Hsm;

typedef enum {
	STATUS_TRANSITION,
	STATUS_HANDLED,
	STATUS_IGNORED,
	STATUS_INIT,
	STATUS_SUPER,
	STATUS_LAST
} Status;

typedef Status (*EventHandler)(Hsm* const me, Message const* const message);
typedef Message* (*EventFunction)();

#define TRANSITION(target_) (((Hsm*)me)->state = (EventHandler)(target_), STATUS_TRANSITION)
void FsmTransition(Hsm* const me, Message const* const message);

#define SUPER(super_) (((Hsm*)me)->stateSuper = (EventHandler)(super_), STATUS_SUPER)

typedef struct Hsm {
	EventHandler state;
	EventHandler stateSuper;
	char const* name;
} Hsm;

void Hsm_ctor(Hsm* const me, EventHandler initial);
void Hsm_init(Hsm* const me, Message const* const message);
void Hsm_dispatch(Hsm* const me, Message const* const message);
Status Hsm_top(Hsm* const me, Message const* const message);

typedef struct Active Active;

// typedef void (*DispatchHandler)(Active* const me, Event const* const e);

struct Active {
	int8_t thread;
	Hsm super;
	// DispatchHandler dispatch;
};

void Active_ctor(Active* const me, EventHandler initial);
void Active_start(Active* const me, uint8_t misc);
void Active_post(Active* const me, Message const* const message);

static Message const eventEntry = {EVENT_ENTRY};
static Message const eventExit = {EVENT_EXIT};

void FsmTransition(Hsm* const me, Message const* const message) {
	// FsmDispatch(me, &exitEvt);   /* exit the source */
	// me->state = target; /*   change current state */
	// FsmDispatch(me, &entryEvt); /* enter the target */
}

// typedef struct Hsm Hsm;
// typedef Msg const *(*EvtHndlr)(Hsm*, Msg const*);
//
// typedef struct State State;
// struct State {
// 	State *super;                                  /* pointer to superstate */
// 	EvtHndlr hndlr;                             /* state's handler function */
// 	char const *name;
// };
//
// void StateCtor(State *me, char const *name, State *super, EvtHndlr hndlr);
// #define StateOnEvent(me_, ctx_, msg_) \
//     (*(me_)->hndlr)((ctx_), (msg_))
//
// struct Hsm {                       /* Hierarchical State Machine base class */
// 	char const *name;                             /* pointer to static name */
// 	State *curr;                                           /* current state */
// 	State *next;                  /* next state (non 0 if transition taken) */
// 	State *source;                   /* source state during last transition */
// 	State top;                                     /* top-most state object */
// };

void Hsm_ctor(Hsm* const me, EventHandler initial) {
	me->state = initial;
}

void Hsm_init(Hsm* const me, Message const* const message) {
	fprintf(stderr, "Hsm_init\n");
	assert(me->state != (EventHandler)0);
	(*me->state)(me, message);
	(*me->state)(me, &eventEntry);
}

void Hsm_dispatch(Hsm* const me, Message const* const message) {
	Status state;
	EventHandler state_previous = me->state;
	fprintf(stderr, "Hsm_dispatch\n");

	// assert((me->state != (StateHandler)0) && (e->signal < SIGNAL_MAX));
	state = (*me->state)(me, message);

	while (state == STATUS_SUPER) {
		fprintf(stderr, "state == STATUS_SUPER\n");
		state = (*me->stateSuper)(me, message);
	}

	// TODO: implement nested entry/exit action

	if (state == STATUS_TRANSITION) {
		fprintf(stderr, "state == STATUS_TRANSITION\n");
		assert(me->state != (EventHandler)0);
		(*state_previous)(me, &eventExit);
		(*me->state)(me, &eventEntry);
	}
}

Status Hsm_top(Hsm* const me, Message const* const message) {
	fprintf(stderr, "Hsm_top\n");
	return STATUS_IGNORED;
}

void Active_ctor(Active* const me, EventHandler initial) {
	fprintf(stderr, "Active_ctor\n");
	Hsm_ctor(&me->super, initial);
}

static void Active_eventLoop(void* pData, EventFunction eventFunction) {
	Active* me = (Active*)pData;
	fprintf(stderr, "Active_eventLoop\n");

	Hsm_init(&me->super, (Message*)0);

	while (1) {
		Message* e = 0;
		int8_t error = 0;
		e = (*eventFunction)();
		assert(error == 0);

		Hsm_dispatch(&me->super, e);
		sleep(1);
	}
}

#endif  // HSM_H
