#include "bsp.h"
#include "Scheduler.h"
#include "RGBLED.h"

void Task_Scheduler(void)
{
	while(1)
	{
			RGB_Task();
			delay_ms(500);
		//uart1_send_string("hello!\r\n");
		
		//USART_SendData(USART1,0xaa);
	}
}
