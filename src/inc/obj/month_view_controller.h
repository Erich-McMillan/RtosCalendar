#pragma once

#include <obj/view_controller.h>

typedef struct _MonthViewController_t MonthViewController_t;

typedef struct _MonthViewController_t {
   ViewController_t _super;
} MonthViewController_t;

void InitMonthViewController(MonthViewController_t* controller, ViewController_t* yearViewController, ViewController_t* dayViewController);
