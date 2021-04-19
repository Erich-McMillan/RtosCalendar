#pragma once

#include "lib/date_time_lib.h"

#define MAX_EVENT_NAME_LEN 64

typedef struct _ScheduledEvent_t {
   Date_t scheduledDay;
   Timeslot_t scheduledTime;
	 char eventName[MAX_EVENT_NAME_LEN];
   // other information, not relevant yet
} ScheduledEvent_t;
