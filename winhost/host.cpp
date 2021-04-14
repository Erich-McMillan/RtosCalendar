//#include <stdafx.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#include <lib/graphics_lib.h>

HDC          globalhdc;

#define GRAPHICS_SCALING 4

VOID OnPaint(HDC hdc)
{
		Graphics graphics(globalhdc);
		Pen      pen(Color(255, 0, 0, 255));
		graphics.DrawLine(&pen, 0, 0, 128, 128);
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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
		HWND                hWnd;
		MSG                 msg;
		WNDCLASS            wndClass;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
		WPARAM wParam, LPARAM lParam)
{
		PAINTSTRUCT  ps;
		char *testStr = "teststr";

		switch (message)
		{
		case WM_PAINT:
				globalhdc = BeginPaint(hWnd, &ps);
				// go call out to the calendar code here, which will
				// call back into graphics_lib functions defined here
				// hdc must be set to a global for this function so that
				// the graphics functions can access it
				RgbColor c;
				c.red = 0;
				c.green = 255;
				c.blue = 0;
				DrawHLine(0, 0, 128, c);
				DrawVLine(0, 0, 128, c);
				FillRect(40, 40, 30, 88, c);
				DrawString(100, 50, testStr, c);
				EndPaint(hWnd, &ps);
				return 0;
		case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
		default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
} // WndProc
