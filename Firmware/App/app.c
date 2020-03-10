#include "app.h"
#include "bsp.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
/* 静态申明 ------------------------------------------------------------------*/
static void APP_Init(void);
static void APP_Task(void *pvParameters);
/************************************************
函数名称 ： AppTaskCreate
功    能 ： 创建任务
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void AppTaskCreate(void)
{
  BaseType_t xReturn;

  /* 创建应用程序(开始任务) */
	xTaskCreate((TaskFunction_t )APP_Task,     	
							(const char*    )"APP_Task",   	
							(uint16_t       )APP_STACK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )APP_TASK_PRIORITY,	
							(TaskHandle_t*  )NULL);   
  if(pdPASS != xReturn)
  {
    printf("APP_Task create failed");
    return;                                      //创建任务失败
  }
}

/************************************************
函数名称 ： APP_Init
功    能 ： 应用程序初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
static void APP_Init(void)
{

}
/************************************************
函数名称 ： APP_Task
功    能 ： 应用程序(开始任务)
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
static void APP_Task(void *pvParameters)
{
  BSP_Init();                                    //底层驱动初始化
  APP_Init();                                    //应用程序初始化

  for(;;)
  {
    vTaskDelay(50);
		
    BLED_TOGGLE();                                //LED闪烁
  }
}
