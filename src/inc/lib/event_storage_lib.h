#pragma once

#include <typedef.h>
#include <obj/scheduled_event.h>
#include <lib/date_time_lib.h>

void LoadEventsForDay(Date_t* day, out ScheduledEvent_t** events, out uint8_t* numEvents); 
void SaveEvent(ScheduledEvent_t* event);
void DeleteEvent(ScheduledEvent_t* event);