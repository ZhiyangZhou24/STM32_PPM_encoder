#include "bsp.h"
#include "bsp_timer.h"
#include "bsp_usart.h"

/************************************************
函数名称 ： LED_Initializes
功    能 ： LED初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void LED_Initializes(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK,ENABLE);     //开启时钟
	
	GPIO_InitStruct.GPIO_Pin = GLED_PIN|RLED_PIN|BLED_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitStruct);                       //初始化IO
	
	GPIO_SetBits(LED_GPIO_PORT,GLED_PIN|RLED_PIN|BLED_PIN);
}

void PPM_Pin_Initializes(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);     //开启时钟
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStruct);                       //初始化IO
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}
/************************************************
函数名称 ： BSP_Init
功    能 ： 底层驱动初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void BSP_Init(void)
{
  LED_Initializes();      //LED底层初始化
	PPM_Pin_Initializes();  //PPM底层初始化
  TIM_Initializes();      //TIM底层初始化  PPM编码器初始化
  USART_Initializes(); 		//USART底层初始化 D-BUS协议初始化
}
