#ifndef _APP_H
#define _APP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"


/* 宏定义 --------------------------------------------------------------------*/
#define APP_STACK_SIZE            128                      //开始任务堆栈大小
#define APP_TASK_PRIORITY         1                        //开始任务优先级

#define PPM_STACK_SIZE            128                      //开始任务堆栈大小
#define PPM_TASK_PRIORITY         2                        //开始任务优先级

/* 函数申明 ------------------------------------------------------------------*/
void AppTaskCreate(void);


#endif /* _APP_H */
