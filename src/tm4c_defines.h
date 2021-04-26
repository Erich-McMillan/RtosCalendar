#pragma once

#include <lib/date_time_lib.h>
#include <lib/graphics_lib.h>

#define MAX_EVENTS_PER_DAY 16u
#define TIMESPANS_PER_HOUR 4u
#define TIMESPAN_LEN_IN_MINS MINUTES_PER_HOUR / TIMESPANS_PER_HOUR

#define DISPLAY_MAX_X 128
#define DISPLAY_MAX_Y 128

#define HEADER_HEIGHT 13u
#define MAX_HEADER_CHAR_LEN 100
#define LEFT_CHAR_OFFSET_X_POS 2
#define TOP_CHAR_OFFSET_Y_POS 2

extern RgbColor TEXT_COLOR;
extern RgbColor LINE_COLOR;
extern RgbColor BACKGROUND_COLOR;