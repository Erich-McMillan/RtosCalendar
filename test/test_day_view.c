#include "unity.h"
// #include <support/UnityHelper.h>
#include "inc/typedef.h"
#include "obj/day_view.h"
#include "lib/view_lib.h"
#include "mock_date_time_lib.h"

void setUp(void) {
   View_t* dayView = GetDayView();
   InitDayView(dayView);
}

void tearDown(void) {

}

void test_GetDayView_ReturnsSameObj_Everytime(void)
{
   // Arrange
   View_t* obj1;
   View_t* obj2;

   // Act
   obj1 = GetDayView();
   obj2 = GetDayView();

   // Assert
   TEST_ASSERT_EQUAL_PTR(obj1, obj2);
}

void test_DayView_SetCurrTimeslot_UpdatesTimeslotToNearest(void) {
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   Timeslot_t slot = {15u, TIMESPAN_LEN_IN_MINS / 2};

   // Act
   view->SetCurrTimeslot(view, slot);

   // Assert
   Timeslot_t* currSlot = view->GetCurrTimeslot(view);
   TEST_ASSERT_EQUAL_UINT(slot.hour, currSlot->hour);
   TEST_ASSERT_INT_WITHIN(TIMESPAN_LEN_IN_MINS / 2, slot.minutes, currSlot->minutes);
}

void test_DayView_NextTimeslot_WrapsFromEveningToMorning(void)
{
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   Timeslot_t lastTimeSlot = {23u, MINUTES_PER_HOUR - TIMESPAN_LEN_IN_MINS};
   view->SetCurrTimeslot(view, lastTimeSlot);

   // Act
   view->SelectNextTimeslot(view);

   // Assert
   Timeslot_t* currTimeslot = view->GetCurrTimeslot(view);
   TEST_ASSERT_TRUE(currTimeslot->minutes == 0u);
   TEST_ASSERT_TRUE(currTimeslot->hour == 0u);
}

void test_DayView_PrevTimeslot_WrapsFromMorningToEvening(void)
{
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   Timeslot_t firstTimeSlot = {0u, 0u};
   view->SetCurrTimeslot(view, firstTimeSlot);

   // Act
   view->SelectPrevTimeslot(view);

   // Assert
   Timeslot_t* currTimeslot = view->GetCurrTimeslot(view);
   TEST_ASSERT_TRUE(currTimeslot->minutes == MINUTES_PER_HOUR - TIMESPAN_LEN_IN_MINS);
   TEST_ASSERT_TRUE(currTimeslot->hour == 23u);
}

uint8_t stub_TimeslotsDontOverlap(Timeslot_t a, Timeslot_t b) {
   return 0;
}

uint8_t stub_TimeslotsDoOverlap(Timeslot_t a, Timeslot_t b) {
   return 1;
}

void test_DayView_SelectNextEvent_DoesNothing_IfNoEventsOverlapWithTimeslot(void)
{
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   ScheduledEvent_t eventToAdd = {};
   view->SetEvents(view, &eventToAdd, 1u);
   DoTimeslotsOverlap_StubWithCallback(stub_TimeslotsDontOverlap);

   // Act
   view->SelectNextEvent(view);

   // Assert
   ScheduledEvent_t* currEvent = view->GetSelectedEvent(view);
   TEST_ASSERT_EQUAL_PTR(null, currEvent);
}

void test_DayView_SelectNextEvent_SetsToNextEvent_IfEventOverlapsWithTimeslot(void)
{
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   ScheduledEvent_t eventToAdd = {};
   view->SetEvents(view, &eventToAdd, 1u);
   DoTimeslotsOverlap_StubWithCallback(stub_TimeslotsDoOverlap);

   // Act
   view->SelectNextEvent(view);

   // Assert
   ScheduledEvent_t* currEvent = view->GetSelectedEvent(view);
   TEST_ASSERT_EQUAL_PTR(&eventToAdd, currEvent);
}

void test_DayView_SelectPrevEvent_DoesNothing_IfNoEventsOverlapWithTimeslot(void)
{
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   ScheduledEvent_t eventToAdd = {};
   view->SetEvents(view, &eventToAdd, 1u);
   DoTimeslotsOverlap_StubWithCallback(stub_TimeslotsDontOverlap);

   // Act
   view->SelectPrevEvent(view);

   // Assert
   ScheduledEvent_t* currEvent = view->GetSelectedEvent(view);
   TEST_ASSERT_EQUAL_PTR(null, currEvent);
}

void test_DayView_SelectPrevEvent_SetsToPrevEvent_IfEventOverlapsWithTimeslot(void)
{
   // Arrange
   DayView_t* view = (DayView_t*) GetDayView();
   ScheduledEvent_t eventToAdd = {};
   view->SetEvents(view, &eventToAdd, 1u);
   DoTimeslotsOverlap_StubWithCallback(stub_TimeslotsDoOverlap);

   // Act
   view->SelectPrevEvent(view);

   // Assert
   ScheduledEvent_t* currEvent = view->GetSelectedEvent(view);
   TEST_ASSERT_EQUAL_PTR(&eventToAdd, currEvent);
}
