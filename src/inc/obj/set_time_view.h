#pragma once

#include "typedef.h"
#include "tm4c_defines.h"
#include <obj/view.h>
#include <lib/date_time_lib.h>

typedef struct _SetTimeView_t SetTimeView_t;

typedef void(*SetSetTimeInfo) (SetTimeView_t* self, Date_t *currDate);
typedef void (*SetTimeGetDate) (SetTimeView_t *self, out Date_t** date, out CalendarTime_t** Time);
typedef void(*SetTimeIncrementValue) (SetTimeView_t *self);
typedef void(*SetTimeDecrementValue) (SetTimeView_t *self);
typedef void (*SetTimeMoveFocusUp) (SetTimeView_t *self);
typedef void (*SetTimeMoveFocusDown) (SetTimeView_t *self);

typedef enum _SetTimeSelectedItem {
		TIME_YEAR,
		TIME_MONTH,
      TIME_DAY,
      TIME_HOUR,
      TIME_MIN
} SetTimeSelectedItem;

typedef struct _SetTimeView_t {
		View_t _super;
		SetTimeSelectedItem _selectedItem;
		Date_t _selectedDate;
      CalendarTime_t _selectedTime;
		SetSetTimeInfo SetDate;
		SetTimeGetDate GetSelectedDateTime;
		SetTimeIncrementValue IncrementValue;
		SetTimeDecrementValue DecrementValue;
		SetTimeMoveFocusUp PrevElement;
		SetTimeMoveFocusDown NextElement;
} SetTimeView_t;

void InitSetTimeView(SetTimeView_t* view);
