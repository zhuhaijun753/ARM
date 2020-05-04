#include "EventRecorder.h"
#include "stdio.h"

int main(void)
{
	EventRecorderInitialize (EventRecordAll, 1);
	for(uint32_t counter =0;;++counter) {
		printf("counter equals: %d\n", counter);
	}
	return 0;
}
