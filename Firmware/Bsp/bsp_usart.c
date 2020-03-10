#include "bsp_usart.h"
#include "bsp.h"
/************************************************
函数名称 ： DR16_Init
功    能 ： D-BUS解析初始化 （DR16接收机）
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void DR16_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	/* 时钟配置 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                    //UART Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* NVIC配置 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* UART配置 */
	USART_InitStructure.USART_BaudRate = 100000;                       //波特率100K
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //传输位数8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;             //停止位1
	USART_InitStructure.USART_Parity = USART_Parity_Even ;             //偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx;                    //收功能
	USART_Init(USART1, &USART_InitStructure);

	USART_ClearFlag(USART1, USART_FLAG_RXNE | USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                  //接收中断

	USART_Cmd(USART1, ENABLE);                                      //使能UART1
}
//全局通道变量
u16 DBUS_Link[6];
s16 CH[6];

float CH_Old[6];
float CH_filter[6];
float CH_filter_Old[6];
float CH_filter_D[6];
u8 NS,CH_Error[6];
u16 NS_cnt,CLR_CH_Error[6];

s16 MAX_CH[6]  = {1684 ,1684 ,1684 ,1684 ,3   ,3 };	//摇杆最大
s16 MIN_CH[6]  = {364  ,364  ,364  ,364  ,1   ,1 };	//摇杆最小
char CH_DIR[6] = {0    ,1    ,0    ,0    ,0   ,0 };  //摇杆方向
float filter_A;

void RC_Duty( float T)
{
	u8 i;
	s16 CH_TMP[6];
	
	for(i=0;i<6;i++)
	{
		
		if( !CH_DIR[i] )
		{
			CH[i] =   LIMIT ( (s16)( ( DBUS_Link[i] - MIN_CH[i] )/(float)( MAX_CH[i] - MIN_CH[i] ) * 1000), 0, 1000); //0-1000
		}
		else
		{
			CH[i] = - LIMIT ( (s16)( ( DBUS_Link[i] - MIN_CH[i] )/(float)( MAX_CH[i] - MIN_CH[i] ) * 1000), 0, 1000); //0-1000
		}
//=================== filter ===================================
//  全局输出，CH_filter[],0横滚，1俯仰，2油门，3航向 范围：0-1000	
//=================== filter =================================== 		
			
		filter_A = 3.14f *20 *T;
		
		if( ABS(CH_TMP[i] - CH_filter[i]) <100 )
		{
			CH_filter[i] += filter_A *(CH[i] - CH_filter[i]) ;
		}
		else
		{
			CH_filter[i] += 0.5f *filter_A *( CH[i] - CH_filter[i]) ;
		}
		CH_filter_D[i] 	= ( CH_filter[i] - CH_filter_Old[i] );
		CH_filter_Old[i] = CH_filter[i];
		CH_Old[i] 		= CH[i];
	}
}
/*DBUS 接收机各通道数据*/
unsigned char USART_RX_BUF[25]; //串口接收缓冲区
unsigned int Pack_Num=0;    //1s时间内收包数
unsigned char Sync_Time=0;  //同步时间，大疆接收机同步时间计算
//DBUS接收协议  波特率100K 偶校验  兼容大疆的遥控器
void USART1_IRQHandler(void)
{
  unsigned char res;
	static uint16_t Data_num=0;
	static uint64_t DBUS_timestamp=0;
	static float old_time=0;
	static float new_time=0;
	if(USART1->SR&(1<<5))//接收到数据
	{
		res = USART1->DR;    //读出寄存器数据 //读出寄存器数据
		/* 获取时间戳 判断丢步以及重新连入接收机  实际大疆接收机14ms同步一次数据   这里7ms同步一次数据*/
		if((TimeStamp_1ms-DBUS_timestamp)>7ull)
		{
			Data_num=0;
		}
		/* 接收数据 */
		USART_RX_BUF[Data_num++]=res;

		if(Data_num==17)  //遥控器数据接收完成
		{
			Sync_Time = TimeStamp_1ms - DBUS_timestamp;
			Pack_Num++;
			//横滚
			DBUS_Link[0]=(((unsigned short)USART_RX_BUF[1]&0x07)<<8)+(unsigned short)USART_RX_BUF[0];
			//俯仰
			DBUS_Link[1]=(((unsigned short)USART_RX_BUF[2]&0x3F)<<5)+(unsigned short)(USART_RX_BUF[1]>>3);
			//偏航
			DBUS_Link[3]=(((unsigned short)USART_RX_BUF[4]&0x01)<<10)+((unsigned short)USART_RX_BUF[3]<<2)+((unsigned short)USART_RX_BUF[2]>>6);
			//油门
			DBUS_Link[2]=(((unsigned short)USART_RX_BUF[5]&0x0F)<<7)+((unsigned short)USART_RX_BUF[4]>>1);
			//左拨杆 1 3 2
			DBUS_Link[4]=((unsigned short)USART_RX_BUF[5]>>6);
			//右拨杆 1 3 2
			DBUS_Link[5]=(((unsigned short)USART_RX_BUF[5]&0x30)>>4);

			Data_num=0;
			DBUS_timestamp=TimeStamp_1ms;
		  old_time = new_time;
			new_time = (float)TimeStamp_1ms/1000.0f - old_time;
			//refresh RC Channel
			RC_Duty(new_time);
		}
		DBUS_timestamp=TimeStamp_1ms;
	}
}


/************************************************
函数名称 ： USART_Initializes
功    能 ： 串口初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： SWUST Tom
*************************************************/
void USART_Initializes(void)
{
  DR16_Init();
}

