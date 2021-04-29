#include <lib/view_lib.h>
#include <lib/interface_lib.h>
#include <obj/month_view.h>
#include <obj/month_view_controller.h>
#include <obj/day_view.h>
#include <lib/date_time_lib.h>

ViewController_t* UpdateMonthView(ViewController_t* self, uint8_t reload) {
   MonthView_t* view = (MonthView_t*) self->_view;

	 if (reload) {
	 }

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

   ((View_t*)view)->Draw((View_t*)view);

   if (IsSelectButtonPressed() && self->_nextView != NULL) {
			 DayView_t* dayView = (DayView_t*)self->_nextView->_view;
			 MonthView_t* monthView = (MonthView_t*)self->_view;
			 dayView->SetDate(dayView, monthView->GetSelectedDate(monthView));
			 return self->_nextView;
   }
   if (IsBackButtonPressed() && self->_prevView != NULL) {
			return self->_prevView;
   }

   return self;
}

void InitMonthViewController(MonthViewController_t* controller, ViewController_t* dayViewController) {
   controller->_super._view = GetMonthView();
   controller->_super._prevView = NULL;
   controller->_super._nextView = dayViewController;
   controller->_super.Update = UpdateMonthView;
}
