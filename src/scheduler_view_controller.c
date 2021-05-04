#include <lib/view_lib.h>
#include <lib/interface_lib.h>
#include <obj/scheduler_view.h>
#include <obj/scheduler_view_controller.h>
#include <lib/event_storage_lib.h>

ViewController_t* UpdateSchedulerView(ViewController_t* self, uint8_t reload) {
   SchedulerView_t* view = (SchedulerView_t*) self->_view;

	 if (reload) {
			 // SchedulerView should present a reset interface?
		   view->_focused = 0;
			 view->_state = EDITING;
	 }

   if (IsUpButtonPressed()) {
         view->MoveFocusUp(view);
   }
   if (IsDownButtonPressed()) {
         view->MoveFocusDown(view);
   }
   if (IsLeftButtonPressed()) {
         view->MoveFocusLeft(view);
   }
   if (IsRightButtonPressed()) {
         view->MoveFocusRight(view);
   }
   if (IsSelectButtonPressed()) {
         view->SelectCurrElement(view);
   }
   if (IsBackButtonPressed()) {
			if (view->_focused) {
					view->UnselectCurrElement(view);
			}
			else {
				view->_state = CANCELED;
			}
   }

   ((View_t*)view)->Draw((View_t*)view);

   if (IsSelectButtonPressed() && self->_nextView != NULL) {
      return self->_nextView;
   }
   if (view->_state == SAVED) {
      // perform save object here if needed
			 SaveEvent(&view->_event);
			 view->_focused = 0;
       return self->_prevView;
   }
	 if (view->_state == DELETED) {
			 DeleteEvent(&view->_event);
		   view->_focused = 0;
			 return self->_prevView;
	 }
	 if (view->_state == CANCELED) {
		   view->_focused = 0;
		   return self->_prevView;
	 }

   return self;
}

void InitSchedulerViewController(SchedulerViewController_t* controller, ViewController_t* dayViewController) {
   controller->_super._view = GetSchedulerView();
   controller->_super._prevView = dayViewController;
   controller->_super._nextView = NULL;
   controller->_super.Update = UpdateSchedulerView;
}
