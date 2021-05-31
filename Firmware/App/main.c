#include "bsp.h"
#include "Scheduler.h"
/************************************************
�������� �� SysInit
��    �� �� ϵͳ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void SysInit(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init();
}

/************************************************
�������� �� main
��    �� �� ���������
��    �� �� ��
�� �� ֵ �� int
��    �� �� SWUST Tom
*************************************************/
int main(void)
{
  /* 1��SYS��ʼ�� */
  SysInit();

  /* 2���������� */
	BSP_Init();                                    //�ײ�������ʼ��

	/* 3����ʼ���� */
	Task_Scheduler();
}

