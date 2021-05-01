#pragma once

#include <obj/view_controller.h>

typedef struct _YearViewController_t YearViewController_t;

typedef struct _YearViewController_t {
   ViewController_t _super;
} YearViewController_t;

void InitYearViewController(YearViewController_t* controller, ViewController_t* setTimeViewController, ViewController_t* monthViewController);
