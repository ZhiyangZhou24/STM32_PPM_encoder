#ifndef _APP_H
#define _APP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"


/* �궨�� --------------------------------------------------------------------*/
#define APP_STACK_SIZE            128                      //��ʼ�����ջ��С
#define APP_TASK_PRIORITY         1                        //��ʼ�������ȼ�

#define PPM_STACK_SIZE            128                      //��ʼ�����ջ��С
#define PPM_TASK_PRIORITY         2                        //��ʼ�������ȼ�

/* �������� ------------------------------------------------------------------*/
void AppTaskCreate(void);


#endif /* _APP_H */
