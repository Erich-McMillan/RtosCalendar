#include <stdint.h>
#include "../inc/BSP.h"
#include "../inc/CortexM.h"
#include "eDisk.h"
#include "../inc/Profile.h"
#include "Texas.h"
#include "eFile.h"
#include <tm4c123gh6pm.h>
#include <os.h>

#include <lib/date_time_lib.h>
#include <lib/view_lib.h>
#include <obj/month_view.h>
#include <obj/month_view_controller.h>
#include <lib/modelview_controller_lib.h>
#include <obj/day_view.h>
#include <obj/day_view_controller.h>
#include <obj/scheduler_view.h>
#include <obj/scheduler_view_controller.h>
#include <obj/year_view.h>
#include <obj/year_view_controller.h>
#include <obj/set_time_view.h>
#include <obj/set_time_view_controller.h>
#include <lib/graphics_lib.h>
#include <lib/interface_lib.h>
#include <lib/event_storage_lib.h>

SchedulerViewController_t SchedulerViewController;
DayViewController_t DayViewController;
MonthViewController_t MonthViewController;
YearViewController_t YearViewController;
SetTimeViewController_t SetTimeViewController;
ViewController_t* CurrentController = (ViewController_t*) &SetTimeViewController;
Date_t dummyDate = { 4, April, 2021 };
int32_t InputSema = 0;
int32_t ReadyForInputSema = 0;
ButtonId CurrentPressedButton = NONE;

void SchedulerViewSetup() {
		View_t* view;
		static Date_t date = { 4, April, 2021 };
		static Timeslot_t timeslot = { 8, 15, 30 };
		view = GetSchedulerView();
		((SchedulerView_t*)view)->SetEventBaseInfo((SchedulerView_t*)view, &date, &timeslot);

		InitSchedulerViewController(&SchedulerViewController, (ViewController_t*) &DayViewController);
}

void DayViewSetup() {
		View_t* view;
		static ScheduledEvent_t events[8] = {
				{ { 4, April, 2021 }, {8, 0, 30}, "30min event" },
				{ { 4, April, 2021 }, {8, 15, 15}, "15min event" },
				{ { 4, April, 2021 }, {8, 15, 15}, "15min event" },
				{ { 4, April, 2021 }, {8, 30, 15}, "15min event" },
				{ { 4, April, 2021 }, {8, 30, 45}, "45min event" },
				{ { 4, April, 2021 }, {8, 30, 60}, "60min event" },
				{ { 4, April, 2021 }, {9, 15, 15}, "15min event" },
				{ { 4, April, 2021 }, {9, 15, 15}, "15min event" }
		};
		view = GetDayView();
		((DayView_t*)view)->SetDate((DayView_t*)view, &dummyDate);
		((DayView_t*)view)->SetEvents((DayView_t*)view, events, 8);

		InitDayViewController(&DayViewController, (ViewController_t*) &MonthViewController, (ViewController_t*) &SchedulerViewController);
}

void MonthViewSetup() {
		View_t* view;
		Date_t startDate = { 1, April, 2021 };
		view = GetMonthView();
		((MonthView_t*)view)->SetMonthInfo((MonthView_t*)view, &startDate);

		InitMonthViewController(&MonthViewController, (ViewController_t*) &YearViewController, (ViewController_t*) &DayViewController);
}

void YearViewSetup() {
   InitYearViewController(&YearViewController, (ViewController_t*) &SetTimeViewController, (ViewController_t*) &MonthViewController);
}

void SetTimeViewSetup() {
   InitSetTimeViewController(&SetTimeViewController, (ViewController_t*) &YearViewController);
}

void TestWriteToRom() {
	uint8_t buff[512];
	// OS_File_Format();
	// MountDirectory();
	uint8_t fileSize = OS_File_Size(0);
	if (fileSize == 0) {
		buff[0] = 0xDE;
		buff[1] = 0xAD;
		buff[2] = 0xBE;
		buff[3] = 0xEF;
		OS_File_Append(0, buff);
		OS_File_Flush();
	}
}

void TestReadFromRom() {
	uint8_t buff[512];
	uint8_t bytesRead = OS_File_Read(0, 0, buff);
	return;
}

void CalendarThread() {
   while(1) {
		CurrentController = ControllerUpdate(CurrentController);
      OS_Signal(&ReadyForInputSema);
      OS_Wait(&InputSema);
	}
}

void FilesystemThread() {
   while(1) {

   }
}

void EmptyThread() {
   while(1) {
      
   }
}

uint8_t ImplIsUpButtonPressed(void)
{
	uint16_t x;
	uint16_t y;
	uint8_t sel;
	BSP_Joystick_Input(&x, &y, &sel);
	if(y>767)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t ImplIsDownButtonPressed(void) {
	uint16_t x;
	uint16_t y;
	uint8_t sel;
	BSP_Joystick_Input(&x, &y, &sel);
	if(y<256)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t ImplIsLeftButtonPressed(void) {
	uint16_t x;
	uint16_t y;
	uint8_t sel;
	BSP_Joystick_Input(&x, &y, &sel);
	if(x>75+(1023/2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t ImplIsRightButtonPressed(void) {
	uint16_t x;
	uint16_t y;
	uint8_t sel;
	BSP_Joystick_Input(&x, &y, &sel);
	if(x<(1023/2)-75)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t ImplIsSelectButtonPressed(void) {
	return (0==BSP_Button1_Input());
	//return 0;
}
uint8_t ImplIsBackButtonPressed(void) {
	//return 0;
	return (0==BSP_Button2_Input());
}

void InputThread() {
   if (ReadyForInputSema == 1 && InputSema == 0) {
      CurrentPressedButton = NONE;
      if (ImplIsUpButtonPressed()) {
         CurrentPressedButton = UP;
      }
      else if (ImplIsDownButtonPressed()) {
         CurrentPressedButton = DOWN;
      }
      else if (ImplIsLeftButtonPressed()) {
         CurrentPressedButton = LEFT;
      }
      else if (ImplIsRightButtonPressed()) {
         CurrentPressedButton = RIGHT;
      }
      else if (ImplIsSelectButtonPressed()) {
         CurrentPressedButton = SELECT;
      }
      else if (ImplIsBackButtonPressed()) {
         CurrentPressedButton = BACK;
      }

      if (CurrentPressedButton != NONE) {
				 ReadyForInputSema = 0;
         OS_Signal(&InputSema);
      }
   }
}

void TimerThread() {

}

int main(void){
  volatile int i;
  DisableInterrupts();
  BSP_Clock_InitFastest();
  Profile_Init();               // initialize the 7 hardware profiling pins
  eDisk_Init(0);
  //OS_File_Format();
  MountDirectory();
  BSP_Button1_Init();
  BSP_Button2_Init();
  BSP_Joystick_Init();
  BSP_LCD_Init();
  BSP_LCD_FillScreen(LCD_WHITE);

	SchedulerViewSetup();
	DayViewSetup();
	MonthViewSetup();
	YearViewSetup();
   SetTimeViewSetup();

   OS_Init();
   OS_AddThreads3(CalendarThread, FilesystemThread, EmptyThread);
   OS_AddPeriodicEventThreads(InputThread, 10, TimerThread, 16000);

   OS_Launch(BSP_Clock_GetFreq()/100);
}

RgbColor BACKGROUND_COLOR = {255, 255, 255}; // white
RgbColor TEXT_COLOR = { 0, 0, 0 }; // black
RgbColor LINE_COLOR = { 224, 224, 224 }; // grey

// draw horz line
void DrawHLine(int x, int y, int w, RgbColor color) {
	uint16_t color16 = BSP_LCD_Color565(color.red, color.green, color.blue);
	BSP_LCD_DrawFastHLine(x, y, w, color16);
}

// draw vert line
void DrawVLine(int x, int y, int h, RgbColor color) {
	uint16_t color16 = BSP_LCD_Color565(color.red, color.green, color.blue);
	BSP_LCD_DrawFastVLine(x, y, h, color16);
}

// fill rect
void FillRect(int x, int y, int w, int h, RgbColor color) {
	uint16_t color16 = BSP_LCD_Color565(color.red, color.green, color.blue);
	BSP_LCD_FillRect(x, y, w, h, color16);
}

// draw string
void DrawString(int x, int y, char *pt, RgbColor textColor) {
	uint16_t color16 = BSP_LCD_Color565(textColor.red, textColor.green, textColor.blue);
	BSP_LCD_DrawString(x, y, pt, color16);
}

// fill screen one color
void FillScreen(RgbColor fillColor) {
	uint16_t color16 = BSP_LCD_Color565(fillColor.red, fillColor.green, fillColor.blue);
	BSP_LCD_FillScreen(color16);
}

//void SetPendingButtonPress(uint8_t buttonid);
//void WaitForInput(void);
uint8_t IsUpButtonPressed(void) {
   uint8_t pressed = (CurrentPressedButton == UP);
   CurrentPressedButton = (pressed) ? NONE : CurrentPressedButton;
   return pressed;
}

uint8_t IsDownButtonPressed(void) {
   uint8_t pressed = (CurrentPressedButton == DOWN);
   CurrentPressedButton = (pressed) ? NONE : CurrentPressedButton;
   return pressed;
}

uint8_t IsLeftButtonPressed(void) {
   uint8_t pressed = (CurrentPressedButton == LEFT);
   CurrentPressedButton = (pressed) ? NONE : CurrentPressedButton;
   return pressed;
}

uint8_t IsRightButtonPressed(void) {
   uint8_t pressed = (CurrentPressedButton == RIGHT);
   CurrentPressedButton = (pressed) ? NONE : CurrentPressedButton;
   return pressed;
}

uint8_t IsSelectButtonPressed(void) {
   uint8_t pressed = (CurrentPressedButton == SELECT);
   CurrentPressedButton = (pressed) ? NONE : CurrentPressedButton;
   return pressed;
}

uint8_t IsBackButtonPressed(void) {
   uint8_t pressed = (CurrentPressedButton == BACK);
   CurrentPressedButton = (pressed) ? NONE : CurrentPressedButton;
   return pressed;
}

CalendarTime_t DummyTime = {8, 12};
CalendarTime_t* GetCurrentCalendarTime(void) {
	// systick to update the time
	return &DummyTime;
}
