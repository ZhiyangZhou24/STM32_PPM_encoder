#include "app.h"
#include "bsp.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
/* ��̬���� ------------------------------------------------------------------*/
static void APP_Init(void);
static void APP_Task(void *pvParameters);
/************************************************
�������� �� AppTaskCreate
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void AppTaskCreate(void)
{
  BaseType_t xReturn;

  /* ����Ӧ�ó���(��ʼ����) */
	xTaskCreate((TaskFunction_t )APP_Task,     	
							(const char*    )"APP_Task",   	
							(uint16_t       )APP_STACK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )APP_TASK_PRIORITY,	
							(TaskHandle_t*  )NULL);   
  if(pdPASS != xReturn)
  {
    printf("APP_Task create failed");
    return;                                      //��������ʧ��
  }
}

/************************************************
�������� �� APP_Init
��    �� �� Ӧ�ó����ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
static void APP_Init(void)
{

}
/************************************************
�������� �� APP_Task
��    �� �� Ӧ�ó���(��ʼ����)
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
static void APP_Task(void *pvParameters)
{
  BSP_Init();                                    //�ײ�������ʼ��
  APP_Init();                                    //Ӧ�ó����ʼ��

  for(;;)
  {
    vTaskDelay(50);
		
    BLED_TOGGLE();                                //LED��˸
  }
}
