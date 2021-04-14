#pragma once

#include "lib/date_time_lib.h"


typedef struct _ScheduledEvent_t {
   Date_t scheduledDay;
   Timeslot_t scheduledTime;
   // other information, not relevant yet
} ScheduledEvent_t;
