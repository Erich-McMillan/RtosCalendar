#pragma once

#define HOURS_PER_DAY 24u
#define MINUTES_PER_HOUR 60u

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

typedef struct _Date_t {
   uint8_t day;
   Month month;
   uint16_t year;
} Date_t;

typedef struct _Timespan_t { 
   Date_t start;
   Date_t end;
} Timespan_t;

typedef struct _Timeslot_t {
   uint8_t hour;
   uint8_t minute;
   uint16_t durationMins;
} Timeslot_t;

uint8_t DoTimeslotsOverlap(Timeslot_t* a, Timeslot_t* b);

uint16_t TimeslotStartSplitMins(Timeslot_t* a, Timeslot_t* b);