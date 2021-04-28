#include <lib/view_lib.h>
#include <lib/interface_lib.h>
#include <obj/month_view.h>
#include <obj/day_view.h>
#include <obj/day_view_controller.h>
#include <obj/scheduler_view.h>
#include <lib/event_storage_lib.h>

ViewController_t* UpdateDayView(ViewController_t* self, uint8_t reload) {
   DayView_t* view = (DayView_t*) self->_view;

	 view->_forceRedraw = reload;

	 if (reload) {
			 ScheduledEvent_t* events = NULL;
			 uint8_t numEvents = 0;
			 LoadEventsForDay(view->GetDate(view), &events, &numEvents);
			 view->SetEvents(view, events, numEvents);
	 }

   if (IsUpButtonPressed()) {
         view->SelectPrevTimeslot(view);
   }
   if (IsDownButtonPressed()) {
         view->SelectNextTimeslot(view);
   }
   if (IsLeftButtonPressed()) {
         view->SelectPrevEvent(view);
   }
   if (IsRightButtonPressed()) {
         view->SelectNextEvent(view);
   }

   ((View_t*)view)->Draw((View_t*)view);

   if (IsSelectButtonPressed() && self->_nextView != NULL) {
			 SchedulerView_t* schedulerView = (SchedulerView_t*)self->_nextView->_view;
			 ScheduledEvent_t* selectedEvent = view->GetSelectedEvent(view);
			 if (selectedEvent != NULL)
			 {
					 schedulerView->SetExistingEventInfo(schedulerView, selectedEvent);
			 }
			 else
			 {
					 schedulerView->SetEventBaseInfo(schedulerView, view->GetDate(view), view->GetCurrTimeslot(view));
			 }
       return self->_nextView;
   }
   if (IsBackButtonPressed()) {
      return self->_prevView;
   }

   return self;
}

void InitDayViewController(DayViewController_t* controller, ViewController_t* monthViewController, ViewController_t* schedulerViewController) {
   controller->_super._view = GetDayView();
   controller->_super._prevView = monthViewController;
   controller->_super._nextView = schedulerViewController;
   controller->_super.Update = UpdateDayView;
}
