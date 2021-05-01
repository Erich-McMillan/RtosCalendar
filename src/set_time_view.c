#include <typedef.h>
#include <stdlib.h>
#include <string.h>
#include <lib/view_lib.h>
#include <obj/set_time_view.h>
#include <lib/graphics_lib.h>
#include <lib/interface_lib.h>
#include <lib/date_time_lib.h>

#define TIME_YEAR_CHAR_HEIGHT 7

SetTimeView_t* SetTimeViewSingleton = null;

void DrawHeaderSetTimeView(SetTimeView_t *self)
{
      DrawString(LEFT_CHAR_OFFSET_X_POS, TOP_CHAR_OFFSET_Y_POS, "Set system time", TEXT_COLOR);
      DrawHLine(0, HEADER_HEIGHT, DISPLAY_MAX_X, LINE_COLOR);
}

void DrawFields(SetTimeView_t *self)
{
      RgbColor SelectedColor = { 255, 0, 0 };

      FillRect(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS, DISPLAY_MAX_X, TIME_YEAR_CHAR_HEIGHT, BACKGROUND_COLOR);
      char* yearStr = (char*)calloc(11, sizeof(char));
      sprintf(yearStr, "Year: %.4d", self->_selectedDate.year);
      RgbColor color = (self->_selectedItem == TIME_YEAR) ? SelectedColor : TEXT_COLOR;
      DrawString(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS, yearStr, color);
      free(yearStr);

      FillRect(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS+ TIME_YEAR_CHAR_HEIGHT, DISPLAY_MAX_X, TIME_YEAR_CHAR_HEIGHT, BACKGROUND_COLOR);
      char* monthStr = (char*)calloc(8, sizeof(char));
      sprintf(monthStr, "Mon: %.2d", self->_selectedDate.month+1);
      color = (self->_selectedItem == TIME_MONTH) ? SelectedColor : TEXT_COLOR;
      DrawString(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS + TIME_YEAR_CHAR_HEIGHT, monthStr, color);
      free(monthStr);

      FillRect(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS+ 2*TIME_YEAR_CHAR_HEIGHT, DISPLAY_MAX_X, TIME_YEAR_CHAR_HEIGHT, BACKGROUND_COLOR);
      char* dayStr = (char*)calloc(8, sizeof(char));
      sprintf(dayStr, "Day: %.2d", self->_selectedDate.day);
      color = (self->_selectedItem == TIME_DAY) ? SelectedColor : TEXT_COLOR;
      DrawString(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS + 2*TIME_YEAR_CHAR_HEIGHT, dayStr, color);
      free(dayStr);

      FillRect(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS+ 3*TIME_YEAR_CHAR_HEIGHT, DISPLAY_MAX_X, TIME_YEAR_CHAR_HEIGHT, BACKGROUND_COLOR);
      char* timeStr = (char*)calloc(12, sizeof(char));
      sprintf(timeStr, "Time: %.2d:%.2d", self->_selectedTime.hour, self->_selectedTime.minute);
      color = (self->_selectedItem == TIME_HOUR || self->_selectedItem == TIME_MIN) ? SelectedColor : TEXT_COLOR;
      DrawString(LEFT_CHAR_OFFSET_X_POS, HEADER_HEIGHT + TOP_CHAR_OFFSET_Y_POS + 3*TIME_YEAR_CHAR_HEIGHT, timeStr, color);
      free(timeStr);
}

void ImplSetTimeViewDraw(View_t *self)
{
      SetTimeView_t* selfSetTimeView = (SetTimeView_t*)self;
      DrawHeaderSetTimeView(selfSetTimeView);
			DrawFields(selfSetTimeView);
}

void ImplGetDateSetTimeView(SetTimeView_t *self, Date_t** date, CalendarTime_t** time)
{
   *date = &self->_selectedDate;
   *time = &self->_selectedTime;
}

void ImplIncrementValue(SetTimeView_t *self)
{
   if (self->_selectedItem == TIME_YEAR) {
         self->_selectedDate.year++;
			self->_selectedDate.day = 1;
   } 
   else if (self->_selectedItem == TIME_MONTH) {
         self->_selectedDate.month = (Month) ((self->_selectedDate.month+1) % 12);
			self->_selectedDate.day = 1;
   }
   else if (self->_selectedItem == TIME_DAY) {
      uint8_t daysInMonth = GetNumDaysInMonth(self->_selectedDate.month, self->_selectedDate.year);
      self->_selectedDate.day = (self->_selectedDate.day+1) % (daysInMonth+1);
      self->_selectedDate.day = (self->_selectedDate.day == 0) ? 1 : self->_selectedDate.day;
   }
   else if (self->_selectedItem == TIME_HOUR) {
      self->_selectedTime.hour = (self->_selectedTime.hour + 1) % HOURS_PER_DAY;
   }
   else if (self->_selectedItem == TIME_MIN) {
      self->_selectedTime.minute = (self->_selectedTime.minute + 1) % MINUTES_PER_HOUR;
   }
}

void ImplDecrementValue(SetTimeView_t *self)
{
   if (self->_selectedItem == TIME_YEAR) {
      self->_selectedDate.year--;
			self->_selectedDate.day = 1;
   }
   else if (self->_selectedItem == TIME_MONTH) {
      self->_selectedDate.month = (Month)((self->_selectedDate.month+12-1) % 12);
      self->_selectedDate.day = 1;
   }
   else if (self->_selectedItem == TIME_DAY) {
      uint8_t daysInMonth = GetNumDaysInMonth(self->_selectedDate.month, self->_selectedDate.year);
      self->_selectedDate.day = (self->_selectedDate.day+daysInMonth-1) % daysInMonth;
      self->_selectedDate.day = (self->_selectedDate.day == 0) ? daysInMonth : self->_selectedDate.day;
   }
   else if (self->_selectedItem == TIME_HOUR) {
      self->_selectedTime.hour = (self->_selectedTime.hour + HOURS_PER_DAY -1) % HOURS_PER_DAY;
   }
   else if (self->_selectedItem == TIME_MIN) {
      self->_selectedTime.minute = (self->_selectedTime.minute + MINUTES_PER_HOUR -1) % MINUTES_PER_HOUR;
   }
}

void ImplNextItem(SetTimeView_t *self)
{
   if (self->_selectedItem == TIME_YEAR) {
      self->_selectedItem = TIME_MONTH;
   }
   else if (self->_selectedItem == TIME_MONTH) {
      self->_selectedItem = TIME_DAY;
   }
   else if (self->_selectedItem == TIME_DAY) {
      self->_selectedItem = TIME_HOUR;
   }
   else if (self->_selectedItem == TIME_HOUR) {
      self->_selectedItem = TIME_MIN;
   }
}

void ImplPrevItem(SetTimeView_t *self)
{
   if (self->_selectedItem == TIME_MONTH) {
      self->_selectedItem = TIME_YEAR;
   }
   else if (self->_selectedItem == TIME_DAY) {
      self->_selectedItem = TIME_MONTH;
   }
   else if (self->_selectedItem == TIME_HOUR) {
      self->_selectedItem = TIME_DAY;
   }
   else if (self->_selectedItem == TIME_MIN) {
      self->_selectedItem = TIME_HOUR;
   }
}

void ImplSetDateSetTimeView(SetTimeView_t* self, Date_t *currDate)
{
      self->_selectedDate.year = currDate->year;
      self->_selectedDate.month = currDate->month;
      self->_selectedDate.day = 1;
}

void InitSetTimeView(SetTimeView_t* view)
{
      if (view != null)
      {
            memset(view, 0u, sizeof(*view));

            SetTimeViewSingleton->_super.Draw = ImplSetTimeViewDraw;
            SetTimeViewSingleton->SetDate = ImplSetDateSetTimeView;
            SetTimeViewSingleton->GetSelectedDateTime = ImplGetDateSetTimeView;
            SetTimeViewSingleton->IncrementValue = ImplIncrementValue;
            SetTimeViewSingleton->DecrementValue = ImplDecrementValue;
            SetTimeViewSingleton->NextElement = ImplNextItem;
            SetTimeViewSingleton->PrevElement = ImplPrevItem;
            SetTimeViewSingleton->_selectedDate = { 1, January, 2020 };
            SetTimeViewSingleton->_selectedTime = { 8, 0 };
            SetTimeViewSingleton->_selectedItem = TIME_YEAR;
      }
}

View_t* GetSetTimeView() {
      if (SetTimeViewSingleton == null)
      {
            SetTimeViewSingleton = (SetTimeView_t*)calloc(1u, sizeof(*SetTimeViewSingleton));
            InitSetTimeView(SetTimeViewSingleton);
      }

      return (View_t*)SetTimeViewSingleton;
}
