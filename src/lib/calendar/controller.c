#include <lib/modelview_controller_lib.h>
#include <obj/view_controller.h>
#include <lib/graphics_lib.h>
#include <tm4c_defines.h>
#include <lib/view_lib.h>

// implements the 'concrete'

ViewController_t* ControllerUpdate(ViewController_t* currViewController) {
   ViewController_t* nextView = currViewController->Update(currViewController, 0);

	 if (nextView != currViewController)
	 {
			 FillScreen(BACKGROUND_COLOR);
			 nextView->Update(nextView, 1);
	 }

	 return nextView;
}
