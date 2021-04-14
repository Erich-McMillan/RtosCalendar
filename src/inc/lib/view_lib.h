#pragma once

#include <obj/view.h>

#define TIMESLOTS_IN_DAY 24; // divide day into 15min slots
#define MAX_EVENTS_PER_TIMESLOT 4;

// to be implmented per device, so that each device can guarantee consistency of appearence
View_t* GetYearView();

View_t* GetMonthView();

View_t* GetDayView();

View_t* GetSchedulerView();
