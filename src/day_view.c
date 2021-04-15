#include <typedef.h>
#include <stdlib.h>
#include <string.h>
#include <lib/view_lib.h>
#include <obj/day_view.h>
#include <lib/graphics_lib.h>

#define HEADER_HEIGHT 28u
#define TIMESLOT_HEIGHT 20u // 5 timeslots
#define TIMESLOT_TIME_CHAR_SIZE 5u // characters are 5pixels tall
#define EVENT_WIDTH 20u // fit 5 events into width

#define MAX_HEADER_LEN 20
#define HEADER_X_POS 5
#define HEADER_Y_POS 5
#define HEADER_BODY_SEP_Y_POS 10

DayView_t* DayViewSingleton = null;

void DrawHeader(Date_t* date) {
		RgbColor c = { 255, 0, 0 };
		char* headerStr = (char*) calloc(MAX_HEADER_LEN, sizeof(char));
		sprintf(headerStr, "%.2d/%.2d/%.4d", date->day, date->month, date->year);
		DrawString(HEADER_X_POS, HEADER_Y_POS, headerStr, c);

		DrawHLine(0, HEADER_BODY_SEP_Y_POS, DISPLAY_MAX_X, c);
}

ScheduledEvent_t* FindCurrentlyVisibleEvents() {
		// return new list of events ordered by time, then by preexisting order
		return null;
}

void DrawEvents() {

}

void DrawTimeslotsAndEvents(Timeslot_t* currentTimeslot, ScheduledEvent_t* events) {
		// probably want to draw 5-6 timeslots
		// need to calculate the times of these other timeslots
		// need to shade the currently selected timeslot
		DrawEvents();
}

void DayViewDraw(View_t* self)
{
		DayView_t* selfView = (DayView_t*)self;
   DrawHeader(selfView->GetDate(selfView)); // can probably be moved to a common lib
	 // DrawCurrentTime();
   DrawTimeslotsAndEvents(selfView->GetCurrTimeslot(selfView), selfView->_events);
   
}

void ImplSetDate(DayView_t *self, Date_t* date) {
   self->_date.year = date->year;
   self->_date.month = date->month;
   self->_date.day = date->day;
}

Date_t* ImplGetDate(DayView_t *self) {
   return &self->_date;
}

void ImplSelectNextTimeslot(DayView_t *self) {
   // this implementation MUST be moved to date_time_lib as a 'add time to timeslot' or 'add time to time' function
   // here for unit testing until date_time_lib is implemented
   // AddTimeToSlot(self->_selectedTimeslot, {0u, TIMESPAN_LEN_IN_MINS});

   uint8_t newMins = (self->_selectedTimeslot.minute + TIMESPAN_LEN_IN_MINS) % MINUTES_PER_HOUR;
   uint8_t nextHour = self->_selectedTimeslot.minute <= newMins;
   self->_selectedTimeslot.minute = newMins;

   if (self->_selectedTimeslot.minute <= newMins)
   {
      self->_selectedTimeslot.hour = (self->_selectedTimeslot.hour + 1u) % HOURS_PER_DAY;
   }
}

void ImplSelectPrevTimeslot(DayView_t *self) {
   // this implementation MUST be moved to date_time_lib as a 'sub time to timeslot' or 'sub time to time' function
   // here for unit testing until date_time_lib is implemented
   // SubTimeToSlot(self->_selectedTimeslot, {0u, TIMESPAN_LEN_IN_MINS});

   uint8_t newMins = (self->_selectedTimeslot.minute + MINUTES_PER_HOUR - (TIMESPAN_LEN_IN_MINS)) % MINUTES_PER_HOUR;
   uint8_t nextHour = self->_selectedTimeslot.minute >= newMins;
   self->_selectedTimeslot.minute = newMins;

   if (self->_selectedTimeslot.minute >= newMins)
   {
      self->_selectedTimeslot.hour = (self->_selectedTimeslot.hour == 0u) ?
         HOURS_PER_DAY-1u :
         (self->_selectedTimeslot.hour - 1u) % HOURS_PER_DAY;
   }
}

void ImplSetCurrTimeslot(DayView_t *self, Timeslot_t span) {
   if (self == null) {
      return;
   }

   self->_selectedTimeslot.hour = span.hour;
   self->_selectedTimeslot.minute = (span.minute / (TIMESPAN_LEN_IN_MINS)) * TIMESPAN_LEN_IN_MINS;
}

Timeslot_t* ImplGetCurrTimeslot(DayView_t *self) {
   return &self->_selectedTimeslot;
}

void ImplSelectNextEvent(DayView_t *self) {
   uint8_t startIndex = (self->_selectedEvent >= 0) ? self->_selectedEvent+1u : 0u;
   for (uint8_t idx=startIndex; idx < self->_numEvents; idx++) {
      uint8_t inCurrTimeslot = DoTimeslotsOverlap(&(self->_events[idx].scheduledTime), self->GetCurrTimeslot(self));
      if (inCurrTimeslot) {
         self->_selectedEvent = idx;
         return;
      }
   }
   // loop back to the beginning, might end up back at currently selected event, if no other events in this timeslot
   for (uint8_t idx=0u; idx < startIndex; idx++) {
      uint8_t inCurrTimeslot = DoTimeslotsOverlap(&(self->_events[idx].scheduledTime), self->GetCurrTimeslot(self));
      if (inCurrTimeslot) {
         self->_selectedEvent = idx;
         return;
      }
   }
   self->_selectedEvent = -1;
   return;
}

void ImplSelectPrevEvent(DayView_t *self) {
   uint8_t startIndex = (self->_selectedEvent >= 0) ? self->_selectedEvent+1u : 0u;
   for (int8_t idx=startIndex; idx > 0u; idx--) {
      uint8_t inCurrTimeslot = DoTimeslotsOverlap(&(self->_events[idx].scheduledTime), self->GetCurrTimeslot(self));
      if (inCurrTimeslot) {
         self->_selectedEvent = idx;
         return;
      }
   }
   // loop back to the beginning, might end up back at currently selected event, if no other events in this timeslot
   for (int8_t idx=self->_numEvents-1; idx >= startIndex; idx--) {
      uint8_t inCurrTimeslot = DoTimeslotsOverlap(&(self->_events[idx].scheduledTime), self->GetCurrTimeslot(self));
      if (inCurrTimeslot) {
         self->_selectedEvent = idx;
         return;
      }
   }
   self->_selectedEvent = -1;
   return;
}

void ImplSetEvents(DayView_t *self, ScheduledEvent_t* events, uint8_t numEvents) {
   self->_events = events;
   self->_numEvents = numEvents;
}

ScheduledEvent_t* ImplGetSelectedEvent(DayView_t *self)
{
   if (self->_selectedEvent >= 0)
   {
      return &(self->_events[self->_selectedEvent]);
   }
   return null;
}

void InitDayView(DayView_t* view)
{
   if (view != null)
   {
      memset(view, 0u, sizeof(*view));

      DayViewSingleton->_super.Draw = DayViewDraw;
      DayViewSingleton->_selectedTimeslot.hour=8u;
      DayViewSingleton->_selectedTimeslot.minute=0u;
      DayViewSingleton->_selectedTimeslot.durationMins=TIMESPAN_LEN_IN_MINS;
      DayViewSingleton->_selectedEvent = -1;
      DayViewSingleton->SetDate = ImplSetDate;
      DayViewSingleton->GetDate = ImplGetDate;
      DayViewSingleton->SelectNextTimeslot = ImplSelectNextTimeslot;
      DayViewSingleton->SelectPrevTimeslot = ImplSelectPrevTimeslot;
      DayViewSingleton->SetCurrTimeslot = ImplSetCurrTimeslot;
      DayViewSingleton->GetCurrTimeslot = ImplGetCurrTimeslot;
      DayViewSingleton->SelectNextEvent = ImplSelectNextEvent;
      DayViewSingleton->SelectPrevEvent = ImplSelectPrevEvent;
      DayViewSingleton->SetEvents = ImplSetEvents;
      DayViewSingleton->GetSelectedEvent = ImplGetSelectedEvent;
   }
}

View_t* GetDayView() {
   if (DayViewSingleton == null)
   {
      DayViewSingleton = (DayView_t*) calloc(1u, sizeof(*DayViewSingleton));
      InitDayView(DayViewSingleton);
   }

   return (View_t*) DayViewSingleton;
}
