#pragma once

#define HOURS_PER_DAY 24u
#define MINUTES_PER_HOUR 60u
#define NUM_DAYS_IN_WEEK 7u

typedef enum _Month {
   January = 0u,
   February,
   March,
   April,
   May,
   June,
   July,
   August,
   September,
   October,
   November,
   December
} Month;

typedef enum _Weekday {
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
} Weekday;

typedef struct _Date_t {
   uint8_t day;
   Month month;
   uint16_t year;
} Date_t;

typedef struct _CalendarTime_t {
		uint8_t hour;
		uint8_t minute;
} CalendarTime_t;

typedef struct _Timespan_t { 
   Date_t start;
   Date_t end;
} Timespan_t;

typedef struct _Timeslot_t {
   uint8_t hour;
   uint8_t minute;
   uint16_t durationMins;
} Timeslot_t;

uint8_t GetNumDaysInMonth(Month month, uint16_t year);

Weekday GetDayOfWeek(Date_t* Date);

uint8_t DoTimeslotsOverlap(Timeslot_t* a, Timeslot_t* b);

uint16_t TimeslotStartSplitMins(Timeslot_t* a, Timeslot_t* b);

Weekday GetDayOfWeek(Date_t* Date);

void AddMinsToTimeslot(Timeslot_t* a, uint16_t minsToAdd);

void SubMinsFromTimeslot(Timeslot_t* a, uint16_t minsToSub);

CalendarTime_t* GetCurrentCalendarTime();
