#include "bsp.h"
#include "Scheduler.h"
/************************************************
函数名称 ： SysInit
功    能 ： 系统初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void SysInit(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init();
}

/************************************************
函数名称 ： main
功    能 ： 主函数入口
参    数 ： 无
返 回 值 ： int
作    者 ： SWUST Tom
*************************************************/
int main(void)
{
  /* 1、SYS初始化 */
  SysInit();

  /* 2、创建任务 */
	BSP_Init();                                    //底层驱动初始化

	/* 3、开始任务 */
	Task_Scheduler();
}

