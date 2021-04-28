#pragma once

#include "typedef.h"
#include "tm4c_defines.h"
#include <obj/view.h>
#include <obj/scheduled_event.h>
#include <lib/date_time_lib.h>

typedef struct _SchedulerView_t SchedulerView_t;

typedef enum _SchedulerViewState {
		EDITING=0,
		SAVED,
		DELETED
} SchedulerViewState;

typedef ScheduledEvent_t* (*CopyScheduledEvent) (SchedulerView_t *self);
typedef void (*SetEventBaseInfo) (SchedulerView_t *self, Date_t* eventDate, Timeslot_t* timeslot);
typedef void (*SetExistingEventInfo) (SchedulerView_t *self, ScheduledEvent_t* event);
typedef SchedulerViewState (*GetSchedulerViewState) (SchedulerView_t *self);
typedef void (*SelectCurrElement) (SchedulerView_t *self);
typedef void (*UnselectCurrElement) (SchedulerView_t *self);
typedef void (*MoveFocusUp) (SchedulerView_t *self);
typedef void (*MoveFocusDown) (SchedulerView_t *self);
typedef void (*MoveFocusLeft) (SchedulerView_t *self);
typedef void (*MoveFocusRight) (SchedulerView_t *self);

typedef struct _SchedulerView_t {
		View_t _super;
		ScheduledEvent_t _event;
		uint8_t _prevObjId;
		uint8_t _currObjId;
		uint8_t _focused;
		SchedulerViewState _state;
		SetEventBaseInfo SetEventBaseInfo;
		SetExistingEventInfo SetExistingEventInfo;
		CopyScheduledEvent CopyScheduledEvent;
		GetSchedulerViewState GetSchedulerViewState;
		SelectCurrElement SelectCurrElement;
		UnselectCurrElement UnselectCurrElement;
		MoveFocusUp MoveFocusUp;
		MoveFocusDown MoveFocusDown;
		MoveFocusLeft MoveFocusLeft;
		MoveFocusRight MoveFocusRight;
} SchedulerView_t;

void InitSchedulerView(SchedulerView_t* view);
