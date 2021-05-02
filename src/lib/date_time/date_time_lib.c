#include <typedef.h>
// #include <stdlib.h>
#include <lib/date_time_lib.h>

int days_in_month[]={31,28,31,30,31,30,31,31,30,31,30,31};

#define TRUE    1
#define FALSE   0

// following code is ripped and modified from https://www.codingunit.com/how-to-make-a-calendar-in-c
int determinedaycode(int year)
{
	int daycode;
	int d1, d2, d3;
	
	d1 = (year - 1.)/ 4.0;
	d2 = (year - 1.)/ 100.;
	d3 = (year - 1.)/ 400.;
	daycode = (year + d1 - d2 + d3) %7;
	return daycode;
}

int IsLeapYear(int year)
{
	if(year % 4 != 0)
	{
		return FALSE;
	}
	else if (year % 100 != 0)
	{
			return TRUE;
	}
	else if (year % 400 != 0)
	{
			return FALSE;
	}
	return TRUE;
}

#undef FALSE
#undef TRUE

uint8_t GetNumDaysInMonth(Month month, uint16_t year) {
	if (month == February && IsLeapYear(year)) {
		return days_in_month[February] + 1;
	}
	return days_in_month[month];
}

Weekday GetDayOfWeek(Date_t* Date) {
	Weekday dayCodeFirstDayOfYear = (Weekday) determinedaycode(Date->year);
	int dayCodeOfDate = dayCodeFirstDayOfYear -1;

	for (uint8_t idx = 0; idx < Date->month; idx++) {
			dayCodeOfDate += GetNumDaysInMonth((Month)idx, Date->year);
	}

	dayCodeOfDate += Date->day;

	return (Weekday) (dayCodeOfDate % NUM_DAYS_IN_WEEK);
}

uint8_t DoTimeslotsOverlap(Timeslot_t* a, Timeslot_t* b) {
		uint16_t a_start_mins_since_midnight = a->hour*MINUTES_PER_HOUR + a->minute;
		uint16_t a_end_mins_since_midnight = a_start_mins_since_midnight + a->durationMins;

		uint16_t b_start_mins_since_midnight = b->hour*MINUTES_PER_HOUR + b->minute;
		uint16_t b_end_mins_since_midnight = b_start_mins_since_midnight + b->durationMins;

		// a in b
		if ((a_start_mins_since_midnight >= b_start_mins_since_midnight) && (a_start_mins_since_midnight < b_end_mins_since_midnight))
		{
				return 1;
		}

		// b in a
		if ((b_start_mins_since_midnight >= a_start_mins_since_midnight) && (b_start_mins_since_midnight < a_end_mins_since_midnight))
		{
				return 1;
		}
		return 0;
}

uint16_t TimeslotStartSplitMins(Timeslot_t* a, Timeslot_t* b) {
		uint16_t a_min_since_midnight = a->hour*MINUTES_PER_HOUR + a->minute;
		uint16_t b_start_mins_since_midnight = b->hour*MINUTES_PER_HOUR + b->minute;

		return (a_min_since_midnight > b_start_mins_since_midnight) ? a_min_since_midnight - b_start_mins_since_midnight : b_start_mins_since_midnight - a_min_since_midnight;
}

void AddMinsToTimeslot(Timeslot_t* a, uint16_t minsToAdd) {
		uint8_t hoursToAdd = (a->minute + minsToAdd) / MINUTES_PER_HOUR;

		uint8_t clockMins = (a->minute + minsToAdd) % MINUTES_PER_HOUR;
		uint8_t clockHour = (a->hour + hoursToAdd) % HOURS_PER_DAY;

		a->minute = clockMins;
		a->hour = clockHour;
}

void SubMinsFromTimeslot(Timeslot_t* a, uint16_t minsToSub) {
		uint8_t hoursToSub = 0;

		if (a->minute < minsToSub) {
				hoursToSub++;
				a->minute += MINUTES_PER_HOUR;
		}

		hoursToSub += minsToSub / MINUTES_PER_HOUR;

		uint8_t clockMins = (a->minute - minsToSub);
		if (hoursToSub > a->hour) {
				a->hour += HOURS_PER_DAY;
		}
		uint8_t clockHour = (a->hour - hoursToSub) % HOURS_PER_DAY;

		a->minute = clockMins;
		a->hour = clockHour;
}

CalendarTime_t CurrentTime = {0};
Date_t CurrentDate = {0};

CalendarTime_t* GetCurrentSystemTime() {
	return &CurrentTime;
}

Date_t* GetCurrentSystemDate() {
	return &CurrentDate;
}

void SetSystemTime(CalendarTime_t* Time) {
	CurrentTime.hour = Time->hour;
	CurrentTime.minute = Time->minute;
}

void SetSystemDate(Date_t* Date) {
	CurrentDate.year = Date->year;
	CurrentDate.month = Date->month;
	CurrentDate.day = Date->day;
}
