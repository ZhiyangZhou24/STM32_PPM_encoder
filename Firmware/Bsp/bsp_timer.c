#include "bsp_timer.h"
#include "bsp.h"
unsigned int TimeStamp_1ms = 0;

//1msʱ���
static void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
/*PPM������*/
static void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}
/*PPM���ü��ʱ�䣬��λus*/
static void Set_Interval(u16 time_us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = time_us-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		TimeStamp_1ms++;
	}
}

unsigned int  count=0;
unsigned int  lastad[7]={0};
unsigned int  T_value[7]={0};
unsigned int  lastad_f=0;
unsigned int  T_value_f=0;
unsigned int  n=0;
/*ȫ��ҡ�˲������˲�֮��*/
extern float CH_filter[6];
unsigned int  O_PPM[7] = {0};
//��ʱ��4�жϷ������
//����PPM�����ź�
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		count++;
		O_PPM[0]=CH_filter[0]+1;//roll
	  O_PPM[1]=CH_filter[1]+1;//pitch
	  O_PPM[2]=CH_filter[2]+1;//yaw
	  O_PPM[3]=CH_filter[3]+1;//thr
	  O_PPM[4]=CH_filter[4]+1;//left
	  O_PPM[5]=(unsigned int)1;
		O_PPM[6]=CH_filter[5]+1;
		switch(count)//ͨ���������
		{
			case 1:PPM_PIN(0);Set_Interval(500);lastad[0]=(unsigned int)O_PPM[1]; //1ͨ��  0.5MS�ȴ�ʱ��
			 		 T_value[0]=(500+lastad[0]); break;//��ǰ�����´θߵ�ƽʱ��
			case 2:PPM_PIN(1);Set_Interval(T_value[0]); break;

			case 3:PPM_PIN(0);Set_Interval(500);lastad[1]=(unsigned int)O_PPM[0]; //2ͨ��
			 		 T_value[1]=(500+lastad[1]); break;
			case 4:PPM_PIN(1);Set_Interval(T_value[1]);break;

			case 5:PPM_PIN(0);Set_Interval(500);lastad[2]=(unsigned int)O_PPM[3]; //3ͨ��
			 	   T_value[2]=(500+lastad[2]); break;
			case 6:PPM_PIN(1);Set_Interval(T_value[2]);break;
			
			case 7:PPM_PIN(0);Set_Interval(500);lastad[3]=(unsigned int)O_PPM[2]; //4ͨ��
			 	   T_value[3]=(500+lastad[3]); break;
			case 8:PPM_PIN(1);Set_Interval(T_value[3]);break;

			case 9:PPM_PIN(0);Set_Interval(500);lastad[4]=(unsigned int)O_PPM[4]; //5ͨ��
			 		 T_value[4]=(500+lastad[4]); break;
			case 10:PPM_PIN(1);Set_Interval(T_value[4]);break;

			case 11:PPM_PIN(0);Set_Interval(500);lastad[5]=(unsigned int)O_PPM[5];//6ͨ�� Ϊ0
			 		 T_value[5]=(500+lastad[5]); break;
			case 12:PPM_PIN(1);Set_Interval(T_value[5]);break;
	
			case 13:PPM_PIN(0);Set_Interval(500);lastad[6]=(unsigned int)O_PPM[6];//7ͨ��
			 		 T_value[6]=(500+lastad[6]); break;
			case 14:PPM_PIN(1);Set_Interval(T_value[6]);break;

			case 15:PPM_PIN(0);Set_Interval(500);break;
			case 16:PPM_PIN(1);for(n=0;n<7;n++)T_value_f +=T_value[n];Set_Interval(20000-T_value_f-8*500);count=0;T_value_f = 0;break;
			default : break;
		}
	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
}
static void Timestamp_TIM_Configuration(void)
{
	TIM3_Int_Init(10-1,SystemCoreClock/10000); //10KHZ����Ƶ�ʣ��ǵ�10Ϊ1ms
}
/************************************************
�������� �� TIM_Initializes
��    �� �� TIM��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� SWUST Tom
*************************************************/
void TIM_Initializes(void)
{
  Timestamp_TIM_Configuration();
	TIM4_Int_Init(20-1,SystemCoreClock/1000);//20ms
}
