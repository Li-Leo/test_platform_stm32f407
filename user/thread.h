#pragma once
#include "cmsis_os2.h"

#define KEY2_FLAG 0x00000001
#define KEY3_FLAG 0x00000002
#define KEY4_FLAG 0x00000004

extern osThreadId_t thread_handle;
extern osSemaphoreId_t sem;
extern osEventFlagsId_t event_thread_handle;

void start_thread(void);
void start_sem_thread(void);
void start_event_thread(void);

