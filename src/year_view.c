#include <typedef.h>
#include <stdlib.h>
#include <string.h>
#include <lib/view_lib.h>
#include <obj/year_view.h>
#include <lib/graphics_lib.h>
#include <lib/interface_lib.h>
#include <lib/date_time_lib.h>

#define YEAR_CHAR_HEIGHT 7

YearView_t* YearViewSingleton = null;

void DrawHeaderYearView(YearView_t *self)
{
		DrawString(LEFT_CHAR_OFFSET_X_POS, TOP_CHAR_OFFSET_Y_POS, "Set y/m to view", TEXT_COLOR);
		DrawHLine(0, HEADER_HEIGHT, DISPLAY_MAX_X, LINE_COLOR);
}

void DrawYearMonth(YearView_t *self)
{
		RgbColor SelectedColor = { 255, 0, 0 };

		FillRect(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS, DISPLAY_MAX_X, YEAR_CHAR_HEIGHT, BACKGROUND_COLOR);
		char* yearStr = (char*)calloc(11, sizeof(char));
		sprintf(yearStr, "Year: %.4d", self->_selectedDate.year);
		RgbColor color = (self->_selectedItem == YEAR) ? SelectedColor : TEXT_COLOR;
		DrawString(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS, yearStr, color);
		free(yearStr);

		FillRect(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS+ YEAR_CHAR_HEIGHT, DISPLAY_MAX_X, YEAR_CHAR_HEIGHT, BACKGROUND_COLOR);
		char* monthStr = (char*)calloc(8, sizeof(char));
		sprintf(monthStr, "Mon: %.2d", self->_selectedDate.month+1);
		color = (self->_selectedItem == MONTH) ? SelectedColor : TEXT_COLOR;
		DrawString(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS + YEAR_CHAR_HEIGHT, monthStr, color);
		free(monthStr);
}

void ImplYearViewDraw(View_t *self)
{
		YearView_t* selfYearView = (YearView_t*)self;
		DrawHeaderYearView(selfYearView);
		DrawYearMonth(selfYearView);
}

Date_t* ImplGetDateYearView(YearView_t *self)
{
		return &self->_selectedDate;
}

void ImplYearViewIncrementValue(YearView_t *self)
{
		if (self->_selectedItem == YEAR) {
				self->_selectedDate.year++;
		} 
		else if (self->_selectedItem == MONTH) {
				self->_selectedDate.month = (Month) ((self->_selectedDate.month+1) % 12);
		}
}

void ImplYearViewDecrementValue(YearView_t *self)
{
		if (self->_selectedItem == YEAR) {
				self->_selectedDate.year--;
		}
		else if (self->_selectedItem == MONTH) {
				self->_selectedDate.month = (Month)((self->_selectedDate.month+12-1) % 12);
		}
}

void ImplSelectYear(YearView_t *self)
{
		self->_selectedItem = YEAR;
}

void ImplSelectMonth(YearView_t *self)
{
		self->_selectedItem = MONTH;
}

void ImplSetDateYearView(YearView_t* self, Date_t *currDate)
{
		self->_selectedDate.year = currDate->year;
		self->_selectedDate.month = currDate->month;
		self->_selectedDate.day = 1;
}

void InitYearView(YearView_t* view)
{
		if (view != null)
		{
				memset(view, 0u, sizeof(*view));

				YearViewSingleton->_super.Draw = ImplYearViewDraw;
				YearViewSingleton->SetInfo = ImplSetDateYearView;
				YearViewSingleton->GetSelectedDate = ImplGetDateYearView;
				YearViewSingleton->IncrementValue = ImplYearViewIncrementValue;
				YearViewSingleton->DecrementValue = ImplYearViewDecrementValue;
				YearViewSingleton->SelectYear = ImplSelectYear;
				YearViewSingleton->SelectMonth = ImplSelectMonth;
				YearViewSingleton->_selectedDate.day = 1;
				YearViewSingleton->_selectedDate.month = January;
				YearViewSingleton->_selectedDate.year = 2020;
				YearViewSingleton->_selectedItem = YEAR;
		}
}

View_t* GetYearView() {
		if (YearViewSingleton == null)
		{
				YearViewSingleton = (YearView_t*)calloc(1u, sizeof(*YearViewSingleton));
				InitYearView(YearViewSingleton);
		}

		return (View_t*)YearViewSingleton;
}
