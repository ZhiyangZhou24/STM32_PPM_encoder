#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f10x.h"


/* 宏定义 --------------------------------------------------------------------*/
/*PPM 输出脚*/
#define PPM_GPIO_CLK              RCC_APB2Periph_GPIOA
#define PPM_PIN(n)							  n?GPIO_SetBits(GPIOA, GPIO_Pin_12):GPIO_ResetBits(GPIOA, GPIO_Pin_12)

#define LED_GPIO_CLK              RCC_APB2Periph_GPIOA
#define RLED_PIN                  GPIO_Pin_5
#define GLED_PIN                  GPIO_Pin_6
#define BLED_PIN                  GPIO_Pin_7
#define LED_GPIO_PORT             GPIOA

/* LED开关 */
#define RLED_ON()                  GPIO_SetBitsGPIO_ResetBits(LED_GPIO_PORT, RLED_PIN)
#define RLED_OFF()                 GPIO_SetBits(LED_GPIO_PORT, RLED_PIN)
#define RLED_TOGGLE()              (LED_GPIO_PORT->ODR ^= RLED_PIN)
#define RLED(n)                    n?GPIO_SetBits(LED_GPIO_PORT, RLED_PIN):GPIO_ResetBits(LED_GPIO_PORT, RLED_PIN)

#define GLED_ON()                  GPIO_SetBitsGPIO_ResetBits(LED_GPIO_PORT, GLED_PIN)
#define GLED_OFF()                 GPIO_SetBits(LED_GPIO_PORT, GLED_PIN)
#define GLED_TOGGLE()              (LED_GPIO_PORT->ODR ^= GLED_PIN)
#define GLED(n)                    n?GPIO_SetBits(LED_GPIO_PORT, GLED_PIN):GPIO_ResetBits(LED_GPIO_PORT, GLED_PIN)

#define BLED_ON()                  GPIO_SetBits(LED_GPIO_PORT, BLED_PIN)
#define BLED_OFF()                 GPIO_ResetBits(LED_GPIO_PORT, BLED_PIN)
#define BLED_TOGGLE()              (LED_GPIO_PORT->ODR ^= BLED_PIN)
#define BLED(n)                    n?GPIO_SetBits(LED_GPIO_PORT, BLED_PIN):GPIO_ResetBits(LED_GPIO_PORT, BLED_PIN)

#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

/* 函数申明 ------------------------------------------------------------------*/
void BSP_Init(void);

extern unsigned int TimeStamp_1ms ;
#endif /* _BSP_H */

