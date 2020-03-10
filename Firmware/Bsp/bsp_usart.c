#include "bsp_usart.h"
#include "bsp.h"
/************************************************
�������� �� DR16_Init
��    �� �� D-BUS������ʼ�� ��DR16���ջ���
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void DR16_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	/* ʱ������ */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                    //UART Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* UART���� */
	USART_InitStructure.USART_BaudRate = 100000;                       //������100K
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //����λ��8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;             //ֹͣλ1
	USART_InitStructure.USART_Parity = USART_Parity_Even ;             //żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx;                    //�չ���
	USART_Init(USART1, &USART_InitStructure);

	USART_ClearFlag(USART1, USART_FLAG_RXNE | USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                  //�����ж�

	USART_Cmd(USART1, ENABLE);                                      //ʹ��UART1
}
//ȫ��ͨ������
u16 DBUS_Link[6];
s16 CH[6];

float CH_Old[6];
float CH_filter[6];
float CH_filter_Old[6];
float CH_filter_D[6];
u8 NS,CH_Error[6];
u16 NS_cnt,CLR_CH_Error[6];

s16 MAX_CH[6]  = {1684 ,1684 ,1684 ,1684 ,3   ,3 };	//ҡ�����
s16 MIN_CH[6]  = {364  ,364  ,364  ,364  ,1   ,1 };	//ҡ����С
char CH_DIR[6] = {0    ,1    ,0    ,0    ,0   ,0 };  //ҡ�˷���
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
//  ȫ�������CH_filter[],0�����1������2���ţ�3���� ��Χ��0-1000	
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
/*DBUS ���ջ���ͨ������*/
unsigned char USART_RX_BUF[25]; //���ڽ��ջ�����
unsigned int Pack_Num=0;    //1sʱ�����հ���
unsigned char Sync_Time=0;  //ͬ��ʱ�䣬�󽮽��ջ�ͬ��ʱ�����
//DBUS����Э��  ������100K żУ��  ���ݴ󽮵�ң����
void USART1_IRQHandler(void)
{
  unsigned char res;
	static uint16_t Data_num=0;
	static uint64_t DBUS_timestamp=0;
	static float old_time=0;
	static float new_time=0;
	if(USART1->SR&(1<<5))//���յ�����
	{
		res = USART1->DR;    //�����Ĵ������� //�����Ĵ�������
		/* ��ȡʱ��� �ж϶����Լ�����������ջ�  ʵ�ʴ󽮽��ջ�14msͬ��һ������   ����7msͬ��һ������*/
		if((TimeStamp_1ms-DBUS_timestamp)>7ull)
		{
			Data_num=0;
		}
		/* �������� */
		USART_RX_BUF[Data_num++]=res;

		if(Data_num==17)  //ң�������ݽ������
		{
			Sync_Time = TimeStamp_1ms - DBUS_timestamp;
			Pack_Num++;
			//���
			DBUS_Link[0]=(((unsigned short)USART_RX_BUF[1]&0x07)<<8)+(unsigned short)USART_RX_BUF[0];
			//����
			DBUS_Link[1]=(((unsigned short)USART_RX_BUF[2]&0x3F)<<5)+(unsigned short)(USART_RX_BUF[1]>>3);
			//ƫ��
			DBUS_Link[3]=(((unsigned short)USART_RX_BUF[4]&0x01)<<10)+((unsigned short)USART_RX_BUF[3]<<2)+((unsigned short)USART_RX_BUF[2]>>6);
			//����
			DBUS_Link[2]=(((unsigned short)USART_RX_BUF[5]&0x0F)<<7)+((unsigned short)USART_RX_BUF[4]>>1);
			//�󲦸� 1 3 2
			DBUS_Link[4]=((unsigned short)USART_RX_BUF[5]>>6);
			//�Ҳ��� 1 3 2
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
�������� �� USART_Initializes
��    �� �� ���ڳ�ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void USART_Initializes(void)
{
  DR16_Init();
}

