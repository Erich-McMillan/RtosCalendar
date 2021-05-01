#pragma once

#include "typedef.h"
#include "tm4c_defines.h"
#include <obj/view.h>
#include <lib/date_time_lib.h>

typedef struct _YearView_t YearView_t;

typedef void(*SetYearInfo) (YearView_t* self, Date_t *currDate);
typedef Date_t* (*YearGetDate) (YearView_t *self);
typedef void(*IncrementValue) (YearView_t *self);
typedef void(*DecrementValue) (YearView_t *self);
typedef void(*SelectYear) (YearView_t *self);
typedef void(*SelectMonth) (YearView_t *self);

typedef enum _YearSelectedItem {
		YEAR,
		MONTH
} YearSelectedItem;

typedef struct _YearView_t {
		View_t _super;
		YearSelectedItem _selectedItem;
		Date_t _selectedDate;
		Month _selectedMonth;
		SetYearInfo SetInfo;
		YearGetDate GetSelectedDate;
		IncrementValue IncrementValue;
		DecrementValue DecrementValue;
		SelectYear SelectYear;
		SelectMonth SelectMonth;
} YearView_t;

void InitYearView(YearView_t* view);
