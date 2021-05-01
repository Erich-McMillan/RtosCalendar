#include <lib/view_lib.h>
#include <lib/interface_lib.h>
#include <obj/set_time_view.h>
#include <obj/set_time_view_controller.h>
#include <obj/year_view.h>
#include <lib/date_time_lib.h>

ViewController_t* UpdateSetTimeView(ViewController_t* self, uint8_t reload) {
   SetTimeView_t* view = (SetTimeView_t*) self->_view;

	 if (reload) {
	 }

   if (IsUpButtonPressed()) {
         view->PrevElement(view);
   }
   if (IsDownButtonPressed()) {
         view->NextElement(view);
   }
   if (IsLeftButtonPressed()) {
         view->DecrementValue(view);
   }
   if (IsRightButtonPressed()) {
         view->IncrementValue(view);
   }

   ((View_t*)view)->Draw((View_t*)view);

   if (IsSelectButtonPressed() && self->_nextView != NULL) {
			 YearView_t* yearView = (YearView_t*) self->_nextView->_view;
			 SetTimeView_t* setTimeView = (SetTimeView_t*)self->_view;
          Date_t* date;
          CalendarTime_t* time;
          setTimeView->GetSelectedDateTime(setTimeView, &date, &time);
         //  SetSystemTime();
          yearView->SetInfo(yearView, date);
			 return self->_nextView;
   }
   if (IsBackButtonPressed() && self->_prevView != NULL) {
			return self->_prevView;
   }

   return self;
}

void InitSetTimeViewController(SetTimeViewController_t* controller, ViewController_t* yearViewController) {
   controller->_super._view = GetSetTimeView();
   controller->_super._prevView = NULL;
   controller->_super._nextView = yearViewController;
   controller->_super.Update = UpdateSetTimeView;
}
