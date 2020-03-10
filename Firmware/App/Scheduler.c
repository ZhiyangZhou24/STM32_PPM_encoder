#include "bsp.h"
#include "Scheduler.h"
#include "RGBLED.h"

void Task_Scheduler(void)
{
	while(1)
	{
		if(TimeStamp_1ms%20) //20ms
		{
			RGB_Task();
		}
	}
}
