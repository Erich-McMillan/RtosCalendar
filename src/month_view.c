#include <typedef.h>
#include <stdlib.h>
#include <string.h>
#include <lib/view_lib.h>
#include <obj/month_view.h>
#include <lib/graphics_lib.h>
#include <lib/interface_lib.h>

MonthView_t* MonthViewSingleton = null;

#define NUM_DAYS_IN_WEEK 7
#define NUM_WEEKS_IN_MONTH_VIEW 5
#define DAY_OF_WEEK_HEADER_HEIGHT 13
#define DAY_WIDTH 18
#define DAY_HEIGHT 20
#define SELECTED_INDICATOR_X_OFFSET 5
#define SELECTED_INDICATOR_Y_OFFSET 7
#define SELECTED_INDICATOR_SIDE_LENGTH 4

RgbColor SelectedIndicatorColor = { 0, 0, 0 };


#define DAY_DRAW_X_OFFSET 2
#define DAY_DRAW_Y_OFFSET 2

void CalculateDayPosition(uint8_t week, uint8_t dayOfWeek, out uint8_t* x, out uint8_t* y)
{
		*x = (DAY_WIDTH * dayOfWeek) + DAY_DRAW_X_OFFSET;
		*y = DAY_OF_WEEK_HEADER_HEIGHT + HEADER_HEIGHT + (DAY_HEIGHT * week) + DAY_DRAW_Y_OFFSET;
}

void DrawHeader(MonthView_t* self)
{
		char* headerStr = (char*)calloc(MAX_HEADER_CHAR_LEN, sizeof(char));
		sprintf(headerStr, "%.2d/%.4d", self->_startDate.month, self->_startDate.year);
		DrawString(LEFT_CHAR_OFFSET_X_POS, TOP_CHAR_OFFSET_Y_POS, headerStr, TEXT_COLOR);
		DrawHLine(0, HEADER_HEIGHT, DISPLAY_MAX_X, LINE_COLOR);
		free(headerStr);
}

void DrawDayOfWeekHeader()
{
		static char* dayStrings[NUM_DAYS_IN_WEEK] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		for (uint8_t Idx = 0u; Idx < NUM_DAYS_IN_WEEK; Idx++)
		{
				DrawString(DAY_DRAW_X_OFFSET + (Idx * DAY_WIDTH), HEADER_HEIGHT + DAY_DRAW_X_OFFSET, dayStrings[Idx], TEXT_COLOR);
		}
		DrawHLine(0, HEADER_HEIGHT+DAY_OF_WEEK_HEADER_HEIGHT, DISPLAY_MAX_X, LINE_COLOR);
}

void DrawMonthGrid()
{
		for (uint8_t idx = 1u; idx <= NUM_DAYS_IN_WEEK; idx++)
		{
				DrawVLine(idx*DAY_WIDTH, HEADER_HEIGHT+DAY_OF_WEEK_HEADER_HEIGHT, DISPLAY_MAX_X - HEADER_HEIGHT - DAY_OF_WEEK_HEADER_HEIGHT, LINE_COLOR);
		}

		for (uint8_t idx = 1u; idx <= NUM_WEEKS_IN_MONTH_VIEW; idx++)
		{
				DrawHLine(0u, idx*DAY_HEIGHT + HEADER_HEIGHT+ DAY_OF_WEEK_HEADER_HEIGHT, DISPLAY_MAX_Y, LINE_COLOR);
		}
}

void DrawDayNumbers(MonthView_t* self)
{
		uint8_t dayCount = 1u;
		uint8_t firstDayWeekdayIndex = (uint8_t)self->_startDateWeekday;

		for (uint8_t weekIdx=0; weekIdx < NUM_WEEKS_IN_MONTH_VIEW; weekIdx++)
		{
				for (uint8_t dayIdx = firstDayWeekdayIndex; (dayIdx < NUM_DAYS_IN_WEEK) && (dayCount <= self->_numDays); dayIdx++)
				{
						char dayStr[3] = { 0 };
						uint8_t x, y;
						CalculateDayPosition(weekIdx, dayIdx, &x, &y);
						sprintf(dayStr, "%d", dayCount);
						DrawString(x, y, dayStr, TEXT_COLOR);
						dayCount++;
				}
				firstDayWeekdayIndex = 0u;
		}
}

void DrawSelectedDayIndicator(MonthView_t* self)
{
		uint8_t x, y;

		// draw over previous selector
		CalculateDayPosition(self->_prevSelectedWeek, self->_prevSelectedWeekDay, &x, &y);
		FillRect(x + SELECTED_INDICATOR_X_OFFSET, y + SELECTED_INDICATOR_Y_OFFSET, SELECTED_INDICATOR_SIDE_LENGTH, SELECTED_INDICATOR_SIDE_LENGTH, BACKGROUND_COLOR);
		
		// draw newselector
		CalculateDayPosition(self->_selectedWeek, self->_selectedWeekDay, &x, &y);
		FillRect(x + SELECTED_INDICATOR_X_OFFSET, y + SELECTED_INDICATOR_Y_OFFSET, SELECTED_INDICATOR_SIDE_LENGTH, SELECTED_INDICATOR_SIDE_LENGTH, SelectedIndicatorColor);
}

void ImplMonthViewDraw(View_t *self)
{
		MonthView_t* selfMonthView = (MonthView_t*)self;
		DrawHeader(selfMonthView);
		DrawDayOfWeekHeader();
		DrawMonthGrid();
		DrawDayNumbers(selfMonthView);
		DrawSelectedDayIndicator(selfMonthView);
}

void ImplSetMonthInfo(MonthView_t* self, Date_t *startDate, uint8_t numDays)
{
		self->_startDate.day = startDate->day;
		self->_startDate.month = startDate->month;
		self->_startDate.year = startDate->year;
		self->_numDays = numDays;
		self->_prevSelectedWeek = 0;
		self->_selectedWeek = 0;
		self->_startDateWeekday = GetDayOfWeek(&self->_startDate);
		self->_prevSelectedWeekDay = self->_startDateWeekday;
		self->_selectedWeekDay = self->_startDateWeekday;
}

uint8_t PositionToDayOfMonth(MonthView_t *self, uint8_t week, uint8_t day)
{
		if (week == 0 && day < self->_startDateWeekday > day)
		{
				return 0;
		}

		uint8_t projectedDay = (NUM_DAYS_IN_WEEK * week) + day - self->_startDateWeekday + 1;

		if (projectedDay > self->_numDays)
		{
				return 0;
		}
		return projectedDay;
}

Date_t* ImplGetDate(MonthView_t *self)
{
		uint8_t dayOfMonth = PositionToDayOfMonth(self, self->_selectedWeek, self->_selectedWeekDay);

		if (dayOfMonth != 0)
		{
				self->_selectedDate.day = dayOfMonth;
				self->_selectedDate.month = self->_startDate.month;
				self->_selectedDate.year = self->_startDate.year;
				return &self->_selectedDate;
		}
		// return NULL if selected grid is not a day of this month
		return NULL;
}

void ImplSelectNextWeek(MonthView_t *self)
{
		self->_prevSelectedWeek = self->_selectedWeek;
		self->_prevSelectedWeekDay = self->_selectedWeekDay;

		self->_selectedWeek = (self->_selectedWeek + 1) % NUM_WEEKS_IN_MONTH_VIEW;
}

void ImplSelectPrevWeek(MonthView_t *self)
{
		self->_prevSelectedWeek = self->_selectedWeek;
		self->_prevSelectedWeekDay = self->_selectedWeekDay;
		
		self->_selectedWeek = (self->_selectedWeek - 1 + NUM_WEEKS_IN_MONTH_VIEW) % NUM_WEEKS_IN_MONTH_VIEW;
}

void ImplSelectNextDay(MonthView_t *self)
{
		self->_prevSelectedWeek = self->_selectedWeek;
		self->_prevSelectedWeekDay = self->_selectedWeekDay;

		self->_selectedWeekDay = (self->_selectedWeekDay + 1) % NUM_DAYS_IN_WEEK;
}

void ImplSelectPrevDay(MonthView_t *self)
{
		self->_prevSelectedWeek = self->_selectedWeek;
		self->_prevSelectedWeekDay = self->_selectedWeekDay;

		self->_selectedWeekDay = (self->_selectedWeekDay - 1 + NUM_DAYS_IN_WEEK) % NUM_DAYS_IN_WEEK;
}

void InitMonthView(MonthView_t* view)
{
		if (view != null)
		{
				memset(view, 0u, sizeof(*view));

				MonthViewSingleton->_super.Draw = ImplMonthViewDraw;
				MonthViewSingleton->GetSelectedDate = ImplGetDate;
				MonthViewSingleton->SetMonthInfo = ImplSetMonthInfo;
				MonthViewSingleton->SelectNextWeek = ImplSelectNextWeek;
				MonthViewSingleton->SelectPrevWeek = ImplSelectPrevWeek;
				MonthViewSingleton->SelectNextDay = ImplSelectNextDay;
				MonthViewSingleton->SelectPrevDay = ImplSelectPrevDay;
				MonthViewSingleton->_numDays = 0u;
				MonthViewSingleton->_selectedWeek = 0u;
				MonthViewSingleton->_selectedWeekDay = 0u;
		}
}

View_t* GetMonthView() {
		if (MonthViewSingleton == null)
		{
				MonthViewSingleton = (MonthView_t*)calloc(1u, sizeof(*MonthViewSingleton));
				InitMonthView(MonthViewSingleton);
		}

		return (View_t*)MonthViewSingleton;
}