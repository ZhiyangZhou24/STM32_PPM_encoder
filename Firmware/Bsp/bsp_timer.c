#include "bsp_timer.h"
#include "bsp.h"
unsigned int TimeStamp_1ms = 0;

//1ms时间戳
static void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
/*PPM发生器*/
static void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}
/*PPM设置间隔时间，单位us*/
static void Set_Interval(u16 time_us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = time_us-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx	
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		TimeStamp_1ms++;
	}
}

unsigned int  count=0;
unsigned int  lastad[7]={0};
unsigned int  T_value[7]={0};
unsigned int  lastad_f=0;
unsigned int  T_value_f=0;
unsigned int  n=0;
/*全局摇杆参数，滤波之后*/
extern float CH_filter[6];
unsigned int  O_PPM[7] = {0};
//定时器4中断服务程序
//产生PPM编码信号
void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		count++;
		O_PPM[0]=CH_filter[0]+1;//roll
	  O_PPM[1]=CH_filter[1]+1;//pitch
	  O_PPM[2]=CH_filter[2]+1;//yaw
	  O_PPM[3]=CH_filter[3]+1;//thr
	  O_PPM[4]=CH_filter[4]+1;//left
	  O_PPM[5]=(unsigned int)1;
		O_PPM[6]=CH_filter[5]+1;
		switch(count)//通道输出计算
		{
			case 1:PPM_PIN(0);Set_Interval(500);lastad[0]=(unsigned int)O_PPM[1]; //1通道  0.5MS等待时间
			 		 T_value[0]=(500+lastad[0]); break;//提前计算下次高电平时间
			case 2:PPM_PIN(1);Set_Interval(T_value[0]); break;

			case 3:PPM_PIN(0);Set_Interval(500);lastad[1]=(unsigned int)O_PPM[0]; //2通道
			 		 T_value[1]=(500+lastad[1]); break;
			case 4:PPM_PIN(1);Set_Interval(T_value[1]);break;

			case 5:PPM_PIN(0);Set_Interval(500);lastad[2]=(unsigned int)O_PPM[3]; //3通道
			 	   T_value[2]=(500+lastad[2]); break;
			case 6:PPM_PIN(1);Set_Interval(T_value[2]);break;
			
			case 7:PPM_PIN(0);Set_Interval(500);lastad[3]=(unsigned int)O_PPM[2]; //4通道
			 	   T_value[3]=(500+lastad[3]); break;
			case 8:PPM_PIN(1);Set_Interval(T_value[3]);break;

			case 9:PPM_PIN(0);Set_Interval(500);lastad[4]=(unsigned int)O_PPM[4]; //5通道
			 		 T_value[4]=(500+lastad[4]); break;
			case 10:PPM_PIN(1);Set_Interval(T_value[4]);break;

			case 11:PPM_PIN(0);Set_Interval(500);lastad[5]=(unsigned int)O_PPM[5];//6通道 为0
			 		 T_value[5]=(500+lastad[5]); break;
			case 12:PPM_PIN(1);Set_Interval(T_value[5]);break;
	
			case 13:PPM_PIN(0);Set_Interval(500);lastad[6]=(unsigned int)O_PPM[6];//7通道
			 		 T_value[6]=(500+lastad[6]); break;
			case 14:PPM_PIN(1);Set_Interval(T_value[6]);break;

			case 15:PPM_PIN(0);Set_Interval(500);break;
			case 16:PPM_PIN(1);for(n=0;n<7;n++)T_value_f +=T_value[n];Set_Interval(20000-T_value_f-8*500);count=0;T_value_f = 0;break;
			default : break;
		}
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
}
static void Timestamp_TIM_Configuration(void)
{
	TIM3_Int_Init(10-1,SystemCoreClock/10000); //10KHZ计数频率，记到10为1ms
}
/************************************************
函数名称 ： TIM_Initializes
功    能 ： TIM初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void TIM_Initializes(void)
{
  Timestamp_TIM_Configuration();
	TIM4_Int_Init(20-1,SystemCoreClock/1000);//20ms
}
