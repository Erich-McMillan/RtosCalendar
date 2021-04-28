#pragma once

#include <obj/view_controller.h>

typedef struct _SchedulerViewController_t SchedulerViewController_t;

typedef struct _SchedulerViewController_t {
   ViewController_t _super;
} SchedulerViewController_t;

void InitSchedulerViewController(SchedulerViewController_t* controller, ViewController_t* dayViewController);
