#pragma once

#include <typedef.h>

typedef struct _RgbColor {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
} RgbColor;

// draw horz line
void DrawHLine(int x, int y, int w, RgbColor color);

// draw vert line
void DrawVLine(int x, int y, int h, RgbColor color);

// fill rect
void FillRect(int x, int y, int w, int h, RgbColor color);

// draw string
void DrawString(int x, int y, char *pt, RgbColor textColor);

// fill screen one color
void FillScreen(RgbColor fillColor);
