#ifndef _BSP_CAN_H
#define _BSP_CAN_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f10x.h"


/* �궨�� --------------------------------------------------------------------*/
#if 1 /* ��վ�忨���� */
#define CANx                      CAN1
#define CAN_CLK                   RCC_APB1Periph_CAN1
#define CAN_GPIO_CLK              RCC_APB2Periph_GPIOB
#define CAN_RX_PIN                GPIO_Pin_8
#define CAN_TX_PIN                GPIO_Pin_9
#define CAN_GPIO_PORT             GPIOB                    //ͬһ��PORT
#define CAN_RX_IRQn               USB_LP_CAN1_RX0_IRQn
#define CAN_RX_Priority           10                       //�жϺ����ӿ�(��stm32f10x_it.c)
#define CAN_RX_IRQHandler         USB_LP_CAN1_RX0_IRQHandler
#else /* ��վ�忨���� */
#define CANx                      CAN1
#define CAN_CLK                   RCC_APB1Periph_CAN1
#define CAN_GPIO_CLK              RCC_APB2Periph_GPIOA
#define CAN_RX_PIN                GPIO_Pin_11
#define CAN_TX_PIN                GPIO_Pin_12
#define CAN_GPIO_PORT             GPIOA                    //ͬһ��PORT
#define CAN_RX_IRQn               USB_LP_CAN1_RX0_IRQn
#define CAN_RX_Priority           10                       //�жϺ����ӿ�(��stm32f10x_it.c)
#define CAN_RX_IRQHandler         USB_LP_CAN1_RX0_IRQHandler
#endif


/* �������� ------------------------------------------------------------------*/
void CAN_Initializes(void);


#endif /* _BSP_CAN_H */
