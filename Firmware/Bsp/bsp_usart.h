#ifndef _BSP_USART_H
#define _BSP_USART_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>


/* �궨�� --------------------------------------------------------------------*/
/* DEBUG */
#define DEBUG_COM                 USART1
#define DEBUG_COM_CLK             RCC_APB2Periph_USART1
#define DEBUG_COM_TX_GPIO_CLK     RCC_APB2Periph_GPIOA     //UART TX
#define DEBUG_COM_TX_PIN          GPIO_Pin_9
#define DEBUG_COM_TX_GPIO_PORT    GPIOA
#define DEBUG_COM_RX_GPIO_CLK     RCC_APB2Periph_GPIOA     //UART RX
#define DEBUG_COM_RX_PIN          GPIO_Pin_10
#define DEBUG_COM_RX_GPIO_PORT    GPIOA
#define DEBUG_COM_IRQn            USART1_IRQn
#define DEBUG_COM_Priority        10                       //���ȼ�
#define DEBUG_COM_BaudRate        115200                   //������
#define DEBUG_COM_IRQHandler      USART1_IRQHandler        //�жϺ����ӿ�(��stm32f10x_it.c)


/* �������� ------------------------------------------------------------------*/
void USART_Initializes(void);
void RC_Duty( float T);

#endif /* _BSP_USART_H */

