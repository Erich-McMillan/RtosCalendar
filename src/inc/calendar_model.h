//
// Author: Erich McMillan
// License: MIT
//
// Purpose: Controller for model of calendar

#include <inc/calendar_typedef.h>

#define CALENDAR_EVENT_MESSAGE_SIZE 64u
#define CALENDAR_MAX_EVENTS_PER_DAY 8u
#define CALENDAR_DAYS_IN_WEEK 7u
#define CALENDAR_WEEKS_IN_MONTH 5u

typedef enum ClockHour
{
   MIDNIGHT = 0u,
   ONE_O_CLOCK,
   TWO_O_CLOCK,
   THREE_O_CLOCK,
   FOUR_O_CLOCK,
   FIVE_O_CLOCK,
   SIX_O_CLOCK,
   SEVEN_O_CLOCK,
   EIGHT_O_CLOCK,
   NINE_O_CLOCK,
   TEN_O_CLOCK,
   ELEVEN_O_CLOCK,
   TWELVE_O_CLOCK,
   THIRTEEN_O_CLOCK,
   FOURTEEN_O_CLOCK,
   FIFTEEN_O_CLOCK,
   SIXTEEN_O_CLOCK,
   SEVENTEEN_O_CLOCK,
   EIGHTEEN_O_CLOCK,
   NINETEEN_O_CLOCK,
   TWENTY_O_CLOCK,
   TWENTY_ONE_O_CLOCK,
   TWENTY_TWO_O_CLOCK,
   TWENTY_THREE_O_CLOCK
};

typedef enum ClockMins
{
   ZERO_MINS = 0u,
   FIFTEEN_MINS = 15u,
   THIRTY_MINS = 30u,
   FOURTY_FIVE_MINS = 45u
};

typedef struct _ClockTime
{
   ClockHour hour;
   ClockMins Mins;
} ClockTime;

typedef struct _ClockSpan
{
   ClockTime startTime;
   ClockTime endTime;
} ClockSpan;

typedef struct _CalendarEvent
{
   char16_t name[CALENDAR_EVENT_MESSAGE_SIZE];
   ClockSpan timespan;

} CalendarEvent;

typedef struct _CalendarDay
{
   private ClockSpan currTimeslot;
   public uint16_t numEvents;
   public CalendarEvent events[CALENDAR_MAX_EVENTS_PER_DAY];
   public DAY_NEXT_TIMESLOT;
   public DAY_PREV_TIMESLOT;
   public DAY_CURR_TIMESLOT;
   public DAY_NEXT_EVENT_IN_TIMESLOT;
   public DAY_PREV_EVENT_IN_TIMESLOT;
   public DAY_CURR_EVENT;
} CalendarDay;

typedef struct _CalendarWeek
{
   private uint16_t currDay;
   public CalendarDay days[CALENDAR_DAYS_IN_WEEK]; // null/0 value indicates day not in use
   public WEEK_NEXT_DAY;
   public WEEK_PREV_DAY;
   public WEEK_CURR_DAY;
} CalendarWeek;

typedef struct _CalendarMonth
{
   private uint16_t numDays;
   private uint16_t currWeek;
   public CalendarWeek weeks[CALENDAR_WEEKS_IN_MONTH];
   public MONTH_NEXT_WEEK NextWeek;
   public MONTH_PREV_WEEK PrevWeek;
   public MONTH_NEXT_DAY NextDay;
   public MONTH_PREV_DAY PrevDay;
   public MONTH_CURR_DAY CurrDay;
} CalendarMonth;

// foward definition
typedef _CalendarModel CalendarModel;

//
// Selects currently focused object in model, object may perform action
//
typedef
uint16_t
(*CALENDAR_SELECT) (
   void
);

typedef struct _CalendarModel
{
   uint16_t currentDay;
} CalendarModel;
