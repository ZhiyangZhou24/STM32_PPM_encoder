#include "bsp.h"
#include "bsp_timer.h"
#include "bsp_usart.h"

/************************************************
�������� �� LED_Initializes
��    �� �� LED��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void LED_Initializes(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK,ENABLE);     //����ʱ��
	
	GPIO_InitStruct.GPIO_Pin = GLED_PIN|RLED_PIN|BLED_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;            //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStruct);                       //��ʼ��IO
	
	GPIO_SetBits(LED_GPIO_PORT,GLED_PIN|RLED_PIN|BLED_PIN);
}

void PPM_Pin_Initializes(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);     //����ʱ��
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;            //�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStruct);                       //��ʼ��IO
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}
/************************************************
�������� �� BSP_Init
��    �� �� �ײ�������ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void BSP_Init(void)
{
  LED_Initializes();      //LED�ײ��ʼ��
	PPM_Pin_Initializes();  //PPM�ײ��ʼ��
  TIM_Initializes();      //TIM�ײ��ʼ��  PPM��������ʼ��
  USART_Initializes(); 		//USART�ײ��ʼ�� D-BUSЭ���ʼ��
}
