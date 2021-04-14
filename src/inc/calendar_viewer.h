//
// Author: Erich McMillan
// License: MIT
//
// Purpose: Viewer abstract for calendar

#include <inc/calendar_model.h>

typedef
void
(*VIEW_RENDER) (
   CalendarModel model
);

typedef
void
(*VIEW_UPDATE) (
   CalendarModel model
);

typedef struct _CalendarView
{
   VIEW_RENDER Render;
   VIEW_UPDATE Update;
} CalendarView;
