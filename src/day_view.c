#include <typedef.h>
#include <stdlib.h>
#include <string.h>
#include <lib/view_lib.h>
#include <obj/day_view.h>
#include <lib/graphics_lib.h>

#define NUM_TIMESLOTS 5u
#define TIMESLOT_HEIGHT ((DISPLAY_MAX_Y-HEADER_HEIGHT) / NUM_TIMESLOTS)
#define TIMESLOT_TIME_CHAR_SIZE 5u // characters are 5pixels tall
#define TIMESLOT_OFFSET 17

#define HEADER_BODY_SEP_Y_POS 10

#define EVENT_X_OFFSET 2
#define EVENT_Y_OFFSET 2
#define EVENT_WIDTH 18

#define NUM_EVENT_COLORS 6
RgbColor EVENT_BASE_COLORS[NUM_EVENT_COLORS] = {
		{ 102, 255, 178 },
		{ 102, 255, 255 },
		{ 102, 178, 255 },
		{ 102, 102, 255 },
		{ 178, 102, 255 },
		{ 255, 102, 255 }
};
RgbColor EVENT_SELECTED_COLORS[NUM_EVENT_COLORS] = {
		{ 204,  255, 229 },
		{ 204,  255, 255 },
		{ 204,  229, 255 },
		{ 204,  204,  255 },
		{ 255, 204,  255 },
		{ 255, 204,  255 }
};

DayView_t* DayViewSingleton = null;

typedef struct _TimeslotBox_t {
		Timeslot_t* timeslot;
		uint8_t x;
		uint8_t y;
		uint8_t height;
} TimeslotBox_t;

typedef struct _EventBox_t {
		ScheduledEvent_t* event;
		uint8_t x;
		uint8_t y;
		uint8_t width;
		uint8_t height;
		RgbColor color;
		RgbColor selectedColor;
		uint8_t selected;
		uint8_t visible;
		uint8_t eventOffset;
} EventBox_t;

void DrawHeader(Date_t* date) {
		char* headerStr = (char*) calloc(MAX_HEADER_CHAR_LEN, sizeof(char));
		sprintf(headerStr, "%.2d/%.2d/%.4d", date->day, date->month+1, date->year);
		DrawString(LEFT_CHAR_OFFSET_X_POS, TOP_CHAR_OFFSET_Y_POS, headerStr, TEXT_COLOR);
		DrawHLine(0, HEADER_HEIGHT, DISPLAY_MAX_X, LINE_COLOR);
		free(headerStr);
}

void DrawEvents(TimeslotBox_t* timeslots, uint8_t numTimeboxes, EventBox_t* events, uint8_t numEvents, uint8_t updatePlacement) {

		if (updatePlacement) {

				for (uint8_t idx = 0u; idx < numEvents; idx++) {
						uint8_t firstTimeslotJdx = 0xFF;
						events[idx].visible = 0;
						for (uint8_t jdx = 0u; jdx < numTimeboxes; jdx++) {
								if (DoTimeslotsOverlap(&(events[idx].event->scheduledTime), timeslots[jdx].timeslot))
								{
										firstTimeslotJdx = (firstTimeslotJdx == 0xFF) ? jdx : firstTimeslotJdx;
										events[idx].visible = 1;
								}
						}
						if (events[idx].visible) {
								events[idx].x = (timeslots[firstTimeslotJdx].x + TIMESLOT_OFFSET) + ((EVENT_WIDTH + EVENT_X_OFFSET) * events[idx].eventOffset);
								events[idx].y = timeslots[firstTimeslotJdx].y + EVENT_Y_OFFSET;
								uint8_t visibleDuration = events[idx].event->scheduledTime.durationMins - TimeslotStartSplitMins(&(events[idx].event->scheduledTime), timeslots[firstTimeslotJdx].timeslot);
								events[idx].height = (timeslots[firstTimeslotJdx].height * visibleDuration / timeslots[firstTimeslotJdx].timeslot->durationMins) - EVENT_Y_OFFSET * 2;
								events[idx].width = EVENT_WIDTH;
						}
				}
		}

		for (uint8_t idx = 0u; idx < numEvents; idx++) {
				if (events[idx].visible) {
						if (events[idx].selected) {
								FillRect(events[idx].x, events[idx].y, events[idx].width, events[idx].height, events[idx].selectedColor);
						}
						else {
								FillRect(events[idx].x, events[idx].y, events[idx].width, events[idx].height, events[idx].color);
						}

						DrawString(events[idx].x + LEFT_CHAR_OFFSET_X_POS, events[idx].y + TOP_CHAR_OFFSET_Y_POS, events[idx].event->eventName, TEXT_COLOR);
				}
		}

}

void DrawTimeslots(TimeslotBox_t* timeslots, uint8_t numTimeslots) {
		for (uint8_t timeslotId = 0u; timeslotId < numTimeslots; timeslotId++) {
				TimeslotBox_t currTimeslot = timeslots[timeslotId];
				char* timeStr = (char*) calloc(6u, sizeof(char));
				sprintf(timeStr, "%.2d:%.2d", currTimeslot.timeslot->hour, currTimeslot.timeslot->minute);
				DrawString(LEFT_CHAR_OFFSET_X_POS, currTimeslot.y + TOP_CHAR_OFFSET_Y_POS, timeStr, TEXT_COLOR);
				DrawHLine(currTimeslot.x, currTimeslot.y + currTimeslot.height, DISPLAY_MAX_X, LINE_COLOR);
				free(timeStr);
		}
}

void CalculateTimeslots(Timeslot_t *timeslots, uint8_t numTimeslots, Timeslot_t* startTimeslot)
{
		timeslots[0].hour = startTimeslot->hour;
		timeslots[0].minute = startTimeslot->minute;
		timeslots[0].durationMins = startTimeslot->durationMins;

		for (uint8_t idx = 1; idx < numTimeslots; idx++) {
				timeslots[idx].hour = timeslots[idx - 1].hour;
				timeslots[idx].minute = timeslots[idx - 1].minute;
				timeslots[idx].durationMins = timeslots[idx - 1].durationMins;
				AddMinsToTimeslot(&timeslots[idx], startTimeslot->durationMins);
		}
}

void DrawCurrentTime(TimeslotBox_t* timeboxes, uint8_t numTimeboxes)
{
		RgbColor currTimeColor = { 0, 255, 0 };
		CalendarTime_t* currTime = GetCurrentCalendarTime();
		Timeslot_t currTimeslot = { currTime->hour, currTime->minute, 1 };
		for (uint8_t idx = 0; idx < numTimeboxes; idx++)
		{
				if (DoTimeslotsOverlap(&currTimeslot, timeboxes[idx].timeslot))
				{
						uint16_t timediff = currTime->hour*MINUTES_PER_HOUR + currTime->minute - timeboxes[idx].timeslot->hour * MINUTES_PER_HOUR - timeboxes[idx].timeslot->minute;
						uint16_t percent = (100* timediff / timeboxes[idx].timeslot->durationMins);
						uint8_t currTimeY = timeboxes[idx].y + (timeboxes[idx].height * percent)/100;
						DrawHLine(timeboxes[idx].x, currTimeY, DISPLAY_MAX_X, currTimeColor);
				}
		}
}

void DrawTimeslotsAndEvents(Timeslot_t* currentTimeslot, EventBox_t* events, uint8_t numEvents, uint8_t updatePlacement) {
		// need to calculate the times of these other timeslots
		// need to shade the currently selected timeslot
		// build out the timeslots first
		Timeslot_t timeslots[NUM_TIMESLOTS] = {};
		CalculateTimeslots(timeslots, NUM_TIMESLOTS, currentTimeslot);
		TimeslotBox_t timeboxes[NUM_TIMESLOTS] = {
				{ &timeslots[0], 0u, HEADER_HEIGHT + (TIMESLOT_HEIGHT * (0)), TIMESLOT_HEIGHT},
				{ &timeslots[1], 0u, HEADER_HEIGHT + (TIMESLOT_HEIGHT * (1)), TIMESLOT_HEIGHT},
				{ &timeslots[2], 0u, HEADER_HEIGHT + (TIMESLOT_HEIGHT * (2)), TIMESLOT_HEIGHT},
				{ &timeslots[3], 0u, HEADER_HEIGHT + (TIMESLOT_HEIGHT * (3)), TIMESLOT_HEIGHT},
				{ &timeslots[4], 0u, HEADER_HEIGHT + (TIMESLOT_HEIGHT * (4)), TIMESLOT_HEIGHT}
		};
		DrawTimeslots(timeboxes, NUM_TIMESLOTS);
		DrawEvents(timeboxes, NUM_TIMESLOTS, events, numEvents, updatePlacement);
		DrawCurrentTime(timeboxes, NUM_TIMESLOTS);
}

void RecalculateEventPlacement(EventBox_t* eventPlacements, ScheduledEvent_t* events, uint8_t numEvents) {

		for (uint8_t idx =0; idx < numEvents; idx++)
		{
				eventPlacements[idx].event = &(events[idx]);
				eventPlacements[idx].color = EVENT_BASE_COLORS[idx%NUM_EVENT_COLORS];
				eventPlacements[idx].selectedColor = EVENT_SELECTED_COLORS[idx%NUM_EVENT_COLORS];
				eventPlacements[idx].selected = 0;
				eventPlacements[idx].visible = 0u;
				eventPlacements[idx].x = 0u;
				eventPlacements[idx].y = 0u;
				eventPlacements[idx].height = 0u;
				eventPlacements[idx].width = 0u;
				eventPlacements[idx].eventOffset = 0u;

				for (int jdx = idx-1; jdx >= 0 && idx > 0; jdx--) {
						// TODO: better algo will take advantage of empty space 
						if (DoTimeslotsOverlap(&(eventPlacements[idx].event->scheduledTime), &(eventPlacements[jdx].event->scheduledTime))) {
								eventPlacements[idx].eventOffset = (eventPlacements[idx].eventOffset <= eventPlacements[jdx].eventOffset) ? eventPlacements[jdx].eventOffset + 1 : eventPlacements[idx].eventOffset;
						}
				}
		}
}

void DayViewDraw(View_t* self)
{
		static EventBox_t eventPlacements[MAX_EVENTS_PER_DAY] = {};
		DayView_t* selfView = (DayView_t*)self;
		// TODO: should determine if partial redraw if possible (i.e. the selected time didn't change only the selected object)
		if (selfView->_newEventsAdded || selfView->_forceRedraw) {
				 RecalculateEventPlacement(eventPlacements, selfView->_events, selfView->_numEvents);
		}
		if (selfView->_selectedEvent >= 0) {
				for (uint8_t idx =0; idx < selfView->_numEvents; idx++) {
						eventPlacements[idx].selected = 0u;
				}
				eventPlacements[selfView->_selectedEvent].selected = 1;
		}
		uint8_t updatePlacements = (selfView->_newEventsAdded || selfView->_updatedTimeslot || selfView->_forceRedraw) ? 1 : 0;
		if (updatePlacements) {
				FillScreen(BACKGROUND_COLOR);
				DrawHeader(selfView->GetDate(selfView));
		}
		DrawTimeslotsAndEvents(selfView->GetCurrTimeslot(selfView), eventPlacements, selfView->_numEvents, updatePlacements);
		selfView->_newEventsAdded = 0;
		selfView->_updatedTimeslot = 0;
		selfView->_forceRedraw = 0;
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
		self->_updatedTimeslot = 1;
	  AddMinsToTimeslot(&self->_selectedTimeslot, TIMESPAN_LEN_IN_MINS);
}

void ImplSelectPrevTimeslot(DayView_t *self) {
		self->_updatedTimeslot = 1;
		SubMinsFromTimeslot(&self->_selectedTimeslot, TIMESPAN_LEN_IN_MINS);
}

void ImplSetCurrTimeslot(DayView_t *self, Timeslot_t span) {
   if (self == null) {
      return;
   }

	 self->_updatedTimeslot = 1;

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
   uint8_t startIndex = (self->_selectedEvent >= 0) ? self->_selectedEvent : 0u;
   for (int8_t idx=startIndex-1; idx >= 0; idx--) {
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
	 self->_newEventsAdded = 1;
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
