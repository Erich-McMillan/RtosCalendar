#pragma once

#include <obj/view_controller.h>

typedef struct _SetTimeViewController_t SetTimeViewController_t;

typedef struct _SetTimeViewController_t {
   ViewController_t _super;
} SetTimeViewController_t;

void InitSetTimeViewController(SetTimeViewController_t* controller, ViewController_t* yearViewController);
