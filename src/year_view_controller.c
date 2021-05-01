#include <lib/view_lib.h>
#include <lib/interface_lib.h>
#include <obj/year_view.h>
#include <obj/year_view_controller.h>
#include <obj/month_view.h>
#include <lib/date_time_lib.h>

ViewController_t* UpdateYearView(ViewController_t* self, uint8_t reload) {
   YearView_t* view = (YearView_t*) self->_view;

	 if (reload) {
	 }

   if (IsUpButtonPressed()) {
         view->SelectYear(view);
   }
   if (IsDownButtonPressed()) {
         view->SelectMonth(view);
   }
   if (IsLeftButtonPressed()) {
         view->DecrementValue(view);
   }
   if (IsRightButtonPressed()) {
         view->IncrementValue(view);
   }

   ((View_t*)view)->Draw((View_t*)view);

   if (IsSelectButtonPressed() && self->_nextView != NULL) {
			 MonthView_t* monthView = (MonthView_t*) self->_nextView->_view;
			 YearView_t* yearView = (YearView_t*)self->_view;
			 monthView->SetMonthInfo(monthView, yearView->GetSelectedDate(yearView));
			 return self->_nextView;
   }
   if (IsBackButtonPressed() && self->_prevView != NULL) {
			return self->_prevView;
   }

   return self;
}

void InitYearViewController(YearViewController_t* controller, ViewController_t* setTimeViewController, ViewController_t* monthViewController) {
   controller->_super._view = GetYearView();
   controller->_super._prevView = setTimeViewController;
   controller->_super._nextView = monthViewController;
   controller->_super.Update = UpdateYearView;
}
