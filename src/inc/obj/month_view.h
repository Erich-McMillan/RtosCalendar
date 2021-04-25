#pragma once

#include "typedef.h"
#include "tm4c_defines.h"
#include <obj/view.h>
#include <lib/date_time_lib.h>

typedef struct _MonthView_t MonthView_t;

typedef Date_t* (*MonthGetDate) (MonthView_t *self);
typedef void (*SetMonthInfo) (MonthView_t* self, Date_t *startDate, uint8_t numDays);
typedef void (*SelectNextWeek) (MonthView_t *self);
typedef void (*SelectPrevWeek) (MonthView_t *self);
typedef void (*SelectNextDay) (MonthView_t *self);
typedef void (*SelectPrevDay) (MonthView_t *self);

typedef struct _MonthView_t {
		View_t _super;
		Date_t _startDate;
		uint8_t _numDays;
		Weekday _startDateWeekday;
		Date_t _selectedDate;
		uint8_t _prevSelectedWeekDay;
		uint8_t _prevSelectedWeek;
		uint8_t _selectedWeekDay;
		uint8_t _selectedWeek;
		SetMonthInfo SetMonthInfo;
		MonthGetDate GetSelectedDate;
		SelectNextWeek SelectNextWeek;
		SelectPrevWeek SelectPrevWeek;
		SelectNextDay SelectNextDay;
		SelectPrevDay SelectPrevDay;
} MonthView_t;

void InitMonthView(MonthView_t* view);
