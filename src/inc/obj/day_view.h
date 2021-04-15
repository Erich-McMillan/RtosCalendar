#pragma once

#include "tm4c_defines.h"
#include <obj/view.h>
#include <lib/date_time_lib.h>
#include <obj/scheduled_event.h>

typedef struct _DayView_t DayView_t;

typedef void (*SetDate) (DayView_t *self, Date_t* date);
typedef Date_t* (*GetDate) (DayView_t *self);
typedef void (*SelectNextTimeslot) (DayView_t *self);
typedef void (*SelectPrevTimeslot) (DayView_t *self);
typedef void (*SetCurrTimeslot) (DayView_t *self, Timeslot_t span);
typedef Timeslot_t* (*GetCurrTimeslot) (DayView_t *self);
typedef void (*SelectNextEvent) (DayView_t *self);
typedef void (*SelectPrevEvent) (DayView_t *self);
typedef ScheduledEvent_t* (*GetSelectedEvent) (DayView_t *self);
typedef void (*SetDayViewEvents) (DayView_t *self, ScheduledEvent_t* events, uint8_t numEvents);

typedef struct _DayView_t {
   View_t _super;
   Date_t _date;
   Timeslot_t _selectedTimeslot;
   uint8_t _numEvents;
   int8_t _selectedEvent;
   ScheduledEvent_t* _events;
   SetDate SetDate;
   GetDate GetDate;
   SelectNextTimeslot SelectNextTimeslot;
   SelectPrevTimeslot SelectPrevTimeslot;
   SetCurrTimeslot SetCurrTimeslot;
   GetCurrTimeslot GetCurrTimeslot;
   SelectNextEvent SelectNextEvent;
   SelectPrevEvent SelectPrevEvent;
   SetDayViewEvents SetEvents;
   GetSelectedEvent GetSelectedEvent;
} DayView_t;

void InitDayView(DayView_t* view);
