#include "main.h"
#include "drv_gpio.h"
#include "thread.h"
#include "log.h"
#include "shell.h"

#define THREAD_CONTROL_TRACE

#ifdef THREAD_CONTROL_TRACE
    #define thread_trace(...)   logInfo(__VA_ARGS__);
#else
    #define laser_control_trace(...) 
#endif


osThreadId_t thread_handle;
const osThreadAttr_t thread_handle_attributes = {
    // .name = "laser_ctrl",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityAboveNormal
};

osThreadId_t sem_thread_handle;
const osThreadAttr_t sem_handle_attributes = {
    // .name = "s",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal
};

osEventFlagsId_t event_thread_handle;
const osThreadAttr_t event_handle_attributes = {
    // .name = "s",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal
};

osSemaphoreId_t sem;

void thread_handler(void *argument)
{
    uint32_t flag;
    
    for (;;) {
        flag = osThreadFlagsWait(KEY2_FLAG | KEY3_FLAG | KEY4_FLAG, osFlagsWaitAny, osWaitForever);
    
        switch (flag)
        {
        case KEY2_FLAG:
            // osThreadFlagsClear(KEY2_FLAG);
            logDebug("KEY2_FLAG...");
            thread_trace("KEY2_FLAG...");
            break;
        case KEY3_FLAG:
            // osThreadFlagsClear(KEY2_FLAG);
            logWarning("KEY3_FLAG...");

            break;
        case KEY4_FLAG:
            logError("KEY4_FLAG...");
            logVerbose("KEY4_FLAG...");
            // osThreadFlagsClear(KEY2_FLAG);
            break;

        default:
            break;
        }
    }
}

void start_thread(void)
{
    thread_handle = osThreadNew(thread_handler, NULL, &thread_handle_attributes);
}


void sem_thread_handler(void *argument)
{
    sem = osSemaphoreNew(1, 0, NULL);
    
    for (;;) {
        osSemaphoreAcquire(sem, osWaitForever);
        logDebug("semphore get...");
    }
}

void start_sem_thread(void)
{
    sem_thread_handle = osThreadNew(sem_thread_handler, NULL, &sem_handle_attributes);
}



void event_thread_handler(void *argument)
{
    uint32_t flag;
    event_thread_handle = osEventFlagsNew(NULL);
    
    for (;;) {
        flag = osEventFlagsWait(event_thread_handle, KEY2_FLAG | KEY3_FLAG | KEY4_FLAG, osFlagsWaitAny, osWaitForever);
        
        switch (flag)
        {
        case KEY2_FLAG:
            logDebug("KEY2_FLAG event...");
            break;
        case KEY3_FLAG:

            logWarning("KEY3_FLAG event...");

            break;
        case KEY4_FLAG:
            logError("KEY4_FLAG event...");
            break;

        default:
            break;
        }
    }
}

void start_event_thread(void)
{
    sem_thread_handle = osThreadNew(event_thread_handler, NULL, &event_handle_attributes);
}


// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), standby, laser_set_standby, laser_set_standby);
// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), start, laser_gate_start, start [on_ms] [period_ms]);
// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), stop, laser_stop, laser_stop);

