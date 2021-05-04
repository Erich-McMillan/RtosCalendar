#include <lib/event_storage_lib.h>
#include <eFile.h>
#include <string.h>

extern int32_t FileReadySema, FileWriteSema;
#define EVENT_STORAGE_FILE_ID 0

#pragma pack(1)
typedef struct _EventStorage {
    uint8_t NumEventsStored;
    uint8_t Events[511];
} EventStorage;
#pragma pack()

EventStorage eventStorage = {0};

void LoadEventsForDay(Date_t* day, out ScheduledEvent_t** events, out uint8_t* numEvents) {
    OS_Wait(&FileReadySema);
    MountDirectory();
    // note since this only uses first file sector only 512 bytes can be used total
    // don't really care for this demo, but if more event storage is needed then
    // file size should be checked then each occupied sector should be read into
    // a large enough buffer
		*numEvents = 0;
		uint8_t fileSectors = OS_File_Size(EVENT_STORAGE_FILE_ID);

    if (fileSectors == 1) {
        OS_File_Read(EVENT_STORAGE_FILE_ID, 0, (uint8_t*) &eventStorage);
        
        *numEvents = 0;
        for (uint8_t idx = 0; idx < eventStorage.NumEventsStored; idx++) {
            ScheduledEvent_t* event = &((ScheduledEvent_t*) eventStorage.Events)[idx];
            if (event->scheduledDay.month == day->month && event->scheduledDay.year == day->year) {
                events[*numEvents] = event;
                (*numEvents)++;
            }
        }
    }
}

void SaveEvent(ScheduledEvent_t* event) {
    OS_Wait(&FileReadySema);
    memcpy(&((ScheduledEvent_t*) eventStorage.Events)[eventStorage.NumEventsStored], (void*) event, sizeof(ScheduledEvent_t));
		eventStorage.NumEventsStored++;
    OS_File_Format();
    MountDirectory();
		uint8_t fileId = OS_File_New();
    OS_File_Append(EVENT_STORAGE_FILE_ID, (uint8_t*) &eventStorage);
    OS_Signal(&FileWriteSema);
}

uint8_t CompareEvents(ScheduledEvent_t* a, ScheduledEvent_t* b) {
	if (strcmp(a->eventName, b->eventName)) {
		if (a->scheduledDay.day == b->scheduledDay.day && a->scheduledDay.month == b->scheduledDay.month && a->scheduledDay.year == b->scheduledDay.year) {
			if (a->scheduledTime.durationMins == b->scheduledTime.durationMins && a->scheduledTime.hour == b->scheduledTime.hour && a->scheduledTime.minute == b->scheduledTime.minute) {
				return 1;
			}
		}
	}
	return 0;
}

void DeleteEvent(ScheduledEvent_t* event) {
    uint8_t events[511] = {0};
    uint16_t bytePos = 0;

    OS_Wait(&FileReadySema);

    for (uint8_t eventNum = 0; eventNum < eventStorage.NumEventsStored; eventNum++) {
        ScheduledEvent_t* currEvent = &((ScheduledEvent_t*) eventStorage.Events)[eventNum];
        if (CompareEvents(currEvent, event) == 1) {
            memcpy(&events[bytePos], currEvent, sizeof(ScheduledEvent_t));
            bytePos += sizeof(ScheduledEvent_t);
        }
    }

    memcpy(eventStorage.Events, events, sizeof(events));

    OS_File_Format();
    MountDirectory();
		uint8_t fileId = OS_File_New();
    OS_File_Append(EVENT_STORAGE_FILE_ID, (uint8_t*) &eventStorage);
    
    OS_Signal(&FileWriteSema);
}
