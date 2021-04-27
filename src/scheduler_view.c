#include <typedef.h>
#include <stdlib.h>
#include <string.h>
#include <lib/view_lib.h>
#include <obj/scheduler_view.h>
#include <lib/graphics_lib.h>

#define CHAR_HEIGHT 7
#define SELECTION_BULLET_X LEFT_CHAR_OFFSET_X_POS
#define SELECTION_BULLET_SIDE_LEN 3
#define EVENT_NAME_X (SELECTION_BULLET_X + 5)
#define EVENT_NAME_Y (TOP_CHAR_OFFSET_Y_POS + HEADER_HEIGHT)
#define EVENT_START_X EVENT_NAME_X
#define EVENT_START_Y (EVENT_NAME_Y + CHAR_HEIGHT * 2)
#define EVENT_END_X EVENT_NAME_X
#define EVENT_END_Y (EVENT_START_Y + CHAR_HEIGHT * 2)
#define BUTTON_HEIGHT 20
#define BUTTON_WIDTH 40
#define SAVE_BUTTON_X 5
#define SAVE_BUTTON_Y (DISPLAY_MAX_Y - 5 - BUTTON_HEIGHT)
#define CANCEL_BUTTON_X (DISPLAY_MAX_X - 5 - BUTTON_WIDTH)
#define CANCEL_BUTTON_Y (DISPLAY_MAX_Y - 5 - BUTTON_HEIGHT)
#define KEY_WIDTH 16
#define KEY_HEIGHT 16
#define KEYBOARD_START_X 0
#define KEYBOARD_START_Y (DISPLAY_MAX_Y - (KEY_HEIGHT * 4))

RgbColor FocusedTextColor = { 255, 0, 0 };
RgbColor SaveButtonColor = { 255, 0, 0 };
RgbColor SaveButtonSelectedColor = { 255, 204, 204 };
RgbColor CancelButtonColor = {0, 255, 0};
RgbColor CancelButtonSelectedColor = { 204, 255, 204 };

uint8_t selectedKeyIdx = 0;
uint8_t selectedKeyJdx = 0;

char* keyboard[4][8]{
		{"a", "b", "c", "d", "e", "f", "g", "h"},
		{"i", "j", "k", "l", "m", "n", "o", "p"},
		{"q", "r", "s", "t", "u", "v", "w", "x"},
		{"y", "z", "_", ",", ".", ":", "&", "del"}
};

typedef enum _SchedulerViewObjId {
		EVENT_NAME=0,
		EVENT_START,
		EVENT_END,
		EVENT_SAVE,
		EVENT_CANCEL
};

SchedulerView_t* SchedulerViewSingleton = null;

void DrawHeader(SchedulerView_t* self)
{
		char* headerStr = (char*)calloc(MAX_HEADER_CHAR_LEN, sizeof(char));
		sprintf(headerStr, "Schedule Event On: %2d/%.2d/%.4d", self->_event.scheduledDay.day, self->_event.scheduledDay.month, self->_event.scheduledDay.year);
		DrawString(LEFT_CHAR_OFFSET_X_POS, TOP_CHAR_OFFSET_Y_POS, headerStr, TEXT_COLOR);
		DrawHLine(0, HEADER_HEIGHT, DISPLAY_MAX_X, LINE_COLOR);
		free(headerStr);
}

void DrawEventName(SchedulerView_t* self)
{
		FillRect(EVENT_NAME_X, EVENT_NAME_Y, DISPLAY_MAX_X, CHAR_HEIGHT, BACKGROUND_COLOR);
		RgbColor color = (self->_currObjId == EVENT_NAME && self->_focused) ? FocusedTextColor : TEXT_COLOR;
		char* headerStr = (char*)calloc(13u + MAX_EVENT_NAME_LEN, sizeof(char));
		sprintf(headerStr, "Event name: %s", self->_event.eventName);
		DrawString(EVENT_NAME_X, EVENT_NAME_Y, headerStr, color);
		free(headerStr);
}

void DrawEventStartTime(SchedulerView_t* self)
{
		FillRect(EVENT_START_X, EVENT_START_Y, DISPLAY_MAX_X, CHAR_HEIGHT, BACKGROUND_COLOR);
		RgbColor color = (self->_currObjId == EVENT_START && self->_focused) ? FocusedTextColor : TEXT_COLOR;
		char* headerStr = (char*)calloc(13u + MAX_EVENT_NAME_LEN, sizeof(char));
		sprintf(headerStr, "Start time: %.2d:%.2d", self->_event.scheduledTime.hour, self->_event.scheduledTime.minute);
		DrawString(EVENT_START_X, EVENT_START_Y, headerStr, color);
		free(headerStr);
}

void DrawEventEndtime(SchedulerView_t* self)
{
		FillRect(EVENT_END_X, EVENT_END_Y, DISPLAY_MAX_X, CHAR_HEIGHT, BACKGROUND_COLOR);
		RgbColor color = (self->_currObjId == EVENT_END && self->_focused) ? FocusedTextColor : TEXT_COLOR;
		char* headerStr = (char*)calloc(13u + MAX_EVENT_NAME_LEN, sizeof(char));
		Timeslot_t endTime = {};
		endTime.hour = self->_event.scheduledTime.hour;
		endTime.minute = self->_event.scheduledTime.minute;
		AddMinsToTimeslot(&endTime, self->_event.scheduledTime.durationMins);
		sprintf(headerStr, "End time: %.2d:%.2d", endTime.hour, endTime.minute);
		DrawString(EVENT_END_X, EVENT_END_Y, headerStr, color);
		free(headerStr);
}

void DrawSaveAndCancelButtons(SchedulerView_t* self)
{
		RgbColor saveColor = (self->_currObjId == EVENT_SAVE) ? SaveButtonSelectedColor : SaveButtonColor;
		FillRect(SAVE_BUTTON_X, SAVE_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, saveColor);
		DrawString(SAVE_BUTTON_X + LEFT_CHAR_OFFSET_X_POS, SAVE_BUTTON_Y + TOP_CHAR_OFFSET_Y_POS, "Save", TEXT_COLOR);
		RgbColor cancelColor = (self->_currObjId == EVENT_CANCEL) ? CancelButtonSelectedColor : CancelButtonColor;
		FillRect(CANCEL_BUTTON_X, CANCEL_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, cancelColor);
		DrawString(CANCEL_BUTTON_X + LEFT_CHAR_OFFSET_X_POS, CANCEL_BUTTON_Y + TOP_CHAR_OFFSET_Y_POS, "Cancel", TEXT_COLOR);
}

void DrawSelectionBullet(uint8_t ypos, RgbColor color)
{
		FillRect(SELECTION_BULLET_X, ypos, SELECTION_BULLET_SIDE_LEN, SELECTION_BULLET_SIDE_LEN, color);
}

void DrawSelectionBulletIfNeeded(SchedulerView_t* self)
{
		uint8_t ypos = 0;

		switch (self->_prevObjId)
		{
		case EVENT_NAME:
				ypos = EVENT_NAME_Y;
				DrawSelectionBullet(ypos, BACKGROUND_COLOR);
				break;
		case EVENT_START:
				ypos = EVENT_START_Y;
				DrawSelectionBullet(ypos, BACKGROUND_COLOR);
				break;
		case EVENT_END:
				ypos = EVENT_END_Y;
				DrawSelectionBullet(ypos, BACKGROUND_COLOR);
				break;
		default:
				break;
		}

		switch (self->_currObjId)
		{
				case EVENT_NAME:
						ypos = EVENT_NAME_Y;
						DrawSelectionBullet(ypos, TEXT_COLOR);
						break;
				case EVENT_START:
						ypos = EVENT_START_Y;
						DrawSelectionBullet(ypos, TEXT_COLOR);
						break;
				case EVENT_END:
						ypos = EVENT_END_Y;
						DrawSelectionBullet(ypos, TEXT_COLOR);
						break;
				default:
						break;
		}
}

void DrawKeyBoardIfNeeded(SchedulerView_t* self)
{
		if (self->_currObjId == EVENT_NAME && self->_focused)
		{
				RgbColor KeyboardColor = { 180, 180, 180 };
				RgbColor KeyboardColorSelected = { 255, 180, 180 };
				FillRect(KEYBOARD_START_X, KEYBOARD_START_Y, DISPLAY_MAX_X - KEYBOARD_START_X, DISPLAY_MAX_Y - KEYBOARD_START_Y, KeyboardColor);
				for (uint8_t jdx=0; jdx < 4; jdx++)
				{
						for (uint8_t idx=0; idx < 8; idx++)
						{
								// TODO: fill the entire rect of keyboard at one time rather than bunch of separate fills
								if (selectedKeyIdx == idx && selectedKeyJdx == jdx)
								{
										FillRect(KEYBOARD_START_X + (idx*KEY_WIDTH), KEYBOARD_START_Y + (jdx*KEY_HEIGHT), KEY_WIDTH, KEY_HEIGHT, KeyboardColorSelected);
								}
								else
								{
										// FillRect(KEYBOARD_START_X + (idx*KEY_WIDTH), KEYBOARD_START_Y + (jdx*KEY_HEIGHT), KEY_WIDTH, KEY_HEIGHT, KeyboardColor);
								}
								DrawString(KEYBOARD_START_X + (idx*KEY_WIDTH) + LEFT_CHAR_OFFSET_X_POS, KEYBOARD_START_Y + (jdx*KEY_HEIGHT) + TOP_CHAR_OFFSET_Y_POS, keyboard[jdx][idx], TEXT_COLOR);
						}
				}
		}
		else
		{
				FillRect(KEYBOARD_START_X, KEYBOARD_START_Y, DISPLAY_MAX_X - KEYBOARD_START_X, DISPLAY_MAX_Y - KEYBOARD_START_Y, BACKGROUND_COLOR);
		}
}

void SchedulerViewDraw(View_t* self)
{
		SchedulerView_t* selfView = (SchedulerView_t*)self;
		DrawHeader(selfView);
		DrawEventName(selfView);
		DrawEventStartTime(selfView);
		DrawEventEndtime(selfView);	
		DrawKeyBoardIfNeeded(selfView);
		if (!(selfView->_currObjId == EVENT_NAME && selfView->_focused))
		{
				DrawSaveAndCancelButtons(selfView);
		}
		DrawSelectionBulletIfNeeded(selfView);
}

void ImplSetEventBaseInfo(SchedulerView_t *self, Date_t* eventDate, Timeslot_t* timeslot)
{
		self->_event.scheduledDay.day = eventDate->day;
		self->_event.scheduledDay.month = eventDate->month;
		self->_event.scheduledDay.year = eventDate->year;

		self->_event.scheduledTime.hour = timeslot->hour;
		self->_event.scheduledTime.minute = timeslot->minute;
		self->_event.scheduledTime.durationMins = timeslot->durationMins;
}

ScheduledEvent_t* ImplCopyScheduledEvent(SchedulerView_t *self)
{
		return NULL;
}

SchedulerViewState ImplGetSchedulerViewState(SchedulerView_t *self)
{
		return self->_state;
}

void ImplSelectCurrElement(SchedulerView_t *self)
{
		if (!self->_focused)
		{
				self->_focused = 1;
				switch (self->_currObjId)
				{
						case EVENT_SAVE:
								self->_state = SAVED;
								break;
						case EVENT_CANCEL:
								self->_state = CANCELLED;
								break;
						default:
								break;
				}
		}
		else
		{
				uint8_t lastChar = 0;

				switch (self->_currObjId)
				{
						case EVENT_NAME:
								lastChar = strlen(self->_event.eventName);
								if (selectedKeyJdx == 3 && selectedKeyIdx == 7)
								{
										if (lastChar > 0)
										{
												self->_event.eventName[lastChar - 1] = '\0';
										}
								}
								else if (lastChar < (MAX_EVENT_NAME_LEN-1))
								{
										self->_event.eventName[lastChar] = keyboard[selectedKeyJdx][selectedKeyIdx][0];
								}
								break;
						default:
								break;
				}
		}
}

void ImplUnselectCurrElement(SchedulerView_t *self)
{
		if (self->_focused)
		{
				self->_focused = 0;
				selectedKeyIdx = 0;
				selectedKeyJdx = 0;
		}
}

void ImplMoveFocusUp(SchedulerView_t *self)
{
		if (!self->_focused)
		{
				self->_prevObjId = self->_currObjId;
				self->_currObjId = (self->_currObjId + EVENT_CANCEL) % (EVENT_CANCEL + 1);
		}
		else
		{
				switch (self->_currObjId)
				{
						case EVENT_NAME:
								// adjust keyboard here
								selectedKeyJdx = (selectedKeyJdx - 1 + 4) % 4;
								break;
						case EVENT_START:
								SubMinsFromTimeslot(&self->_event.scheduledTime, TIMESPAN_LEN_IN_MINS);
								break;
						case EVENT_END:
								if (self->_event.scheduledTime.durationMins > TIMESPAN_LEN_IN_MINS)
								{
										self->_event.scheduledTime.durationMins -= TIMESPAN_LEN_IN_MINS;
								}
						default:
								break;
				}
		}
}

void ImplMoveFocusDown(SchedulerView_t *self)
{
		if (!self->_focused)
		{
				self->_prevObjId = self->_currObjId;
				self->_currObjId = (self->_currObjId + 1) % (EVENT_CANCEL+1);
		}
		else
		{
				switch (self->_currObjId)
				{
						case EVENT_NAME:
								// adjust keyboard here
								selectedKeyJdx = (selectedKeyJdx + 1) % 4;
								break;
						case EVENT_START:
								AddMinsToTimeslot(&self->_event.scheduledTime, TIMESPAN_LEN_IN_MINS);
								break;
						case EVENT_END:
								if (self->_event.scheduledTime.durationMins < (HOURS_PER_DAY * MINUTES_PER_HOUR))
								{
										self->_event.scheduledTime.durationMins += TIMESPAN_LEN_IN_MINS;
								}
						default:
								break;
				}
		}
}

void ImplMoveFocusLeft(SchedulerView_t *self)
{
		selectedKeyIdx = (selectedKeyIdx - 1 + 8) % 8;
}

void ImplMoveFocusRight(SchedulerView_t *self)
{
		selectedKeyIdx = (selectedKeyIdx + 1) % 8;
}

void InitSchedulerView(SchedulerView_t* view)
{
		if (view != null)
		{
				memset(view, 0u, sizeof(*view));

				view->_super.Draw = SchedulerViewDraw;
				view->_prevObjId = EVENT_NAME;
				view->_currObjId = EVENT_NAME;
				view->CopyScheduledEvent = ImplCopyScheduledEvent;
				view->GetSchedulerViewState = ImplGetSchedulerViewState;
				view->SetEventBaseInfo = ImplSetEventBaseInfo;
				view->SelectCurrElement = ImplSelectCurrElement;
				view->UnselectCurrElement = ImplUnselectCurrElement;
				view->MoveFocusUp = ImplMoveFocusUp;
				view->MoveFocusDown = ImplMoveFocusDown;
				view->MoveFocusLeft = ImplMoveFocusLeft;
				view->MoveFocusRight = ImplMoveFocusRight;
		}
}

View_t* GetSchedulerView() {
		if (SchedulerViewSingleton == null)
		{
				SchedulerViewSingleton = (SchedulerView_t*)calloc(1u, sizeof(*SchedulerViewSingleton));
				InitSchedulerView(SchedulerViewSingleton);
		}

		return (View_t*)SchedulerViewSingleton;
}