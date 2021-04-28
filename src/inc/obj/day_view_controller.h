#pragma once

#include <obj/view_controller.h>
#include <obj/month_view_controller.h>
#include <obj/scheduler_view_controller.h>

typedef struct _DayViewController_t DayViewController_t;

typedef struct _DayViewController_t {
   ViewController_t _super;
} DayViewController_t;

void InitDayViewController(DayViewController_t* controller, ViewController_t* monthViewController, ViewController_t* schedulerViewController);
