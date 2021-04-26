#include <typedef.h>
// #include <stdlib.h>
#include <lib/date_time_lib.h>

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

Weekday GetDayOfWeek(Date_t* Date) {
		// TODO: this should actually calculate the day of week
		return Monday;
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