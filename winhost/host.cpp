//#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#include <lib/graphics_lib.h>
#include <lib/view_lib.h>
#include <obj/day_view.h>
#include <obj/month_view.h>
#include <obj/scheduler_view.h>
#include <lib/interface_lib.h>

HDC          globalhdc;
uint8_t      globalPendingButton = 0u;
uint8_t      DayViewSelected = 0u;
uint8_t      MonthViewSelected = 0u;
uint8_t      SchedulerViewSelected = 1u;

#define GRAPHICS_SCALING 4

RgbColor BACKGROUND_COLOR = { 255, 255, 255 }; // white
RgbColor TEXT_COLOR = { 0, 0, 0 }; // black
RgbColor LINE_COLOR = { 224, 224, 224 }; // grey

VOID OnPaint()
{
		Graphics graphics(globalhdc);/*
		Pen      pen(Color(255, 0, 0, 255));
		graphics.DrawLine(&pen, 0, 0, 128, 128);*/
		RgbColor c = { 255, 0, 0 };
		DrawHLine(0, 0, DISPLAY_MAX_X, c);
		DrawVLine(0, 0, DISPLAY_MAX_Y, c);
}

// draw horz line
void DrawHLine(int x, int y, int w, RgbColor color) {
		Graphics graphics(globalhdc);
		Color col = Color(255, color.red, color.green, color.blue);
		Pen      pen(col);
		x *= GRAPHICS_SCALING;
		y *= GRAPHICS_SCALING;
		w *= GRAPHICS_SCALING;
		graphics.DrawLine(&pen, x, y, x+w, y);
}

// draw vert line
void DrawVLine(int x, int y, int h, RgbColor color) {
		Graphics graphics(globalhdc);
		Color col = Color(255, color.red, color.green, color.blue);
		Pen      pen(col);
		x *= GRAPHICS_SCALING;
		y *= GRAPHICS_SCALING;
		h *= GRAPHICS_SCALING;
		graphics.DrawLine(&pen, x, y, x, y+h);
}

// fill rect
void FillRect(int x, int y, int w, int h, RgbColor color) {
		SolidBrush brush(Color(255, color.red, color.green, color.blue));
		Graphics graphics(globalhdc);
		x *= GRAPHICS_SCALING;
		y *= GRAPHICS_SCALING;
		h *= GRAPHICS_SCALING;
		w *= GRAPHICS_SCALING;
		graphics.FillRectangle(&brush, x, y, w, h);
}

// draw string
void DrawString(int x, int y, char *pt, RgbColor color) {
		Graphics    graphics(globalhdc);
		SolidBrush brush(Color(255, color.red, color.green, color.blue));
		FontFamily  fontFamily(L"Times New Roman");
		Font        font(&fontFamily, GRAPHICS_SCALING*5, FontStyleRegular, UnitPixel);
		x *= GRAPHICS_SCALING;
		y *= GRAPHICS_SCALING;
		PointF      pointF(x, y);
		const size_t WCHARBUF = 100;
		wchar_t  wszDest[WCHARBUF];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pt, -1, wszDest, WCHARBUF);
		graphics.DrawString(wszDest, -1, &font, pointF, &brush);
}

void FillScreen(RgbColor color) {
		Graphics    graphics(globalhdc);
		SolidBrush brush(Color(255, color.red, color.green, color.blue));
		graphics.FillRectangle(&brush, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void SetPendingButtonPress(uint8_t buttonId) {
		globalPendingButton = buttonId;
}
void WaitForInput() {
		return; // no wait in emulated env
}
uint8_t IsUpButtonPressed() {
		return (globalPendingButton == UP);
}
uint8_t IsDownButtonPressed() {
		return (globalPendingButton == DOWN);
}
uint8_t IsLeftButtonPressed() {
		return (globalPendingButton == LEFT);
}
uint8_t IsRightButtonPressed() {
		return (globalPendingButton == RIGHT);
}
uint8_t IsSelectButtonPressed() {
		return (globalPendingButton == SELECT);
}
uint8_t IsBackButtonPressed() {
		return (globalPendingButton == BACK);
}

void DayViewSetup() {
		View_t* view;
		Date_t date = { 4, April, 2021 };
		static ScheduledEvent_t events[8] = {
				{ date, {8, 0, 30}, "30min event" },
				{ date, {8, 15, 15}, "15min event" },
				{ date, {8, 15, 15}, "15min event" },
				{ date, {8, 30, 15}, "15min event" },
				{ date, {8, 30, 45}, "45min event" },
				{ date, {8, 30, 60}, "60min event" },
				{ date, {9, 15, 15}, "15min event" },
				{ date, {9, 15, 15}, "15min event" }
		};
		view = GetDayView();
		((DayView_t*)view)->SetDate((DayView_t*)view, &date);
		((DayView_t*)view)->SetEvents((DayView_t*)view, events, 8);
}

void MonthViewSetup() {
		View_t* view;
		Date_t startDate = { 1, April, 2021 };
		view = GetMonthView();
		((MonthView_t*)view)->SetMonthInfo((MonthView_t*)view, &startDate, 28);
}

void SchedulerViewSetup() {
		View_t* view;
		static Date_t date = { 4, April, 2021 };
		static Timeslot_t timeslot = { 8, 15, 30 };
		view = GetSchedulerView();
		((SchedulerView_t*)view)->SetEventBaseInfo((SchedulerView_t*)view, &date, &timeslot);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
		HWND                hWnd;
		MSG                 msg;
		WNDCLASS            wndClass;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;

		// Initialize calendar
		DayViewSetup();
		MonthViewSetup();
		SchedulerViewSetup();

		// Initialize GDI+.
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = TEXT("GettingStarted");

		RegisterClass(&wndClass);

		hWnd = CreateWindow(
				TEXT("GettingStarted"),   // window class name
				TEXT("Getting Started"),  // window caption
				WS_OVERLAPPEDWINDOW,      // window style
				CW_USEDEFAULT,            // initial x position
				CW_USEDEFAULT,            // initial y position
				CW_USEDEFAULT,            // initial x size
				CW_USEDEFAULT,            // initial y size
				NULL,                     // parent window handle
				NULL,                     // window menu handle
				hInstance,                // program instance handle
				NULL);                    // creation parameters

		ShowWindow(hWnd, iCmdShow);
		UpdateWindow(hWnd);

		while (GetMessage(&msg, NULL, 0, 0))
		{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}

		GdiplusShutdown(gdiplusToken);
		return msg.wParam;
}  // WinMain

void DayViewController() {
		DayView_t* view;

		if (DayViewSelected)
		{
				view = (DayView_t*)GetDayView();
				if (IsUpButtonPressed()) {
						view->SelectPrevTimeslot(view);
				}
				if (IsDownButtonPressed()) {
						view->SelectNextTimeslot(view);
				}
				if (IsLeftButtonPressed()) {
						view->SelectPrevEvent(view);
				}
				if (IsRightButtonPressed()) {
						view->SelectNextEvent(view);
				}
				if (IsBackButtonPressed()) {
						DayViewSelected = 0;
						MonthViewSelected = 1;
				}

				((View_t*)view)->Draw((View_t*)view);
		}
}

void MonthViewController() {
		MonthView_t* view;

		if (MonthViewSelected)
		{
				view = (MonthView_t*)GetMonthView();
				if (IsUpButtonPressed()) {
						view->SelectPrevWeek(view);
				}
				if (IsDownButtonPressed()) {
						view->SelectNextWeek(view);
				}
				if (IsLeftButtonPressed()) {
						view->SelectPrevDay(view);
				}
				if (IsRightButtonPressed()) {
						view->SelectNextDay(view);
				}
				if (IsSelectButtonPressed()) {
						DayViewSelected = 1;
						MonthViewSelected = 0u;
				}

				((View_t*)view)->Draw((View_t*)view);
		}
}

void SchedulerViewController() {
		SchedulerView_t* view;

		if (SchedulerViewSelected)
		{
				view = (SchedulerView_t*)GetSchedulerView();
				if (IsUpButtonPressed()) {
						view->MoveFocusUp(view);
				}
				if (IsDownButtonPressed()) {
						view->MoveFocusDown(view);
				}
				if (IsLeftButtonPressed()) {
						view->MoveFocusLeft(view);
				}
				if (IsRightButtonPressed()) {
						view->MoveFocusRight(view);
				}
				if (IsSelectButtonPressed()) {
						view->SelectCurrElement(view);
				}
				if (IsBackButtonPressed()) {
						view->UnselectCurrElement(view);
				}

				((View_t*)view)->Draw((View_t*)view);
		}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
		WPARAM wParam, LPARAM lParam)
{
		PAINTSTRUCT  ps;
		char *testStr = "teststr";

		switch (message)
		{
				case WM_PAINT:
						globalhdc = BeginPaint(hWnd, &ps);
						OnPaint();
						// go call out to the calendar code here, which will
						// call back into graphics_lib functions defined here
						// hdc must be set to a global for this function so that
						// the graphics functions can access it
						SchedulerViewController();
						DayViewController();
						MonthViewController();
						EndPaint(hWnd, &ps);
						return 0;
				case WM_KEYDOWN:
						globalPendingButton = 0;
						switch (wParam) {
								case VK_LEFT:
										SetPendingButtonPress(LEFT);
										break;
								case VK_RIGHT:
										SetPendingButtonPress(RIGHT);
										break;
								case VK_UP:
										SetPendingButtonPress(UP);
										break;
								case VK_DOWN:
										SetPendingButtonPress(DOWN);
										break;
								case VK_RETURN:
										SetPendingButtonPress(SELECT);
										break; // enter key will be the 'SELECT' key
								case VK_ESCAPE:
										SetPendingButtonPress(BACK);
										break; // escape will be the 'BACK' key
								default:
										break;
						}
						if (globalPendingButton != 0) {
								globalhdc = GetDC(hWnd);
								SchedulerViewController();
								MonthViewController();
								DayViewController();
								ReleaseDC(hWnd, globalhdc);
								globalPendingButton = 0; // reset to unpressed after handled by listeners
						}
						return 0;
				case WM_DESTROY:
						PostQuitMessage(0);
						return 0;
				default:
						return DefWindowProc(hWnd, message, wParam, lParam);
		}
} // WndProc
