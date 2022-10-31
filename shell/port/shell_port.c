/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "shell.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "log.h"
#include "cmsis_os2.h"
#include "mid_com.h"
#include "log_port.h"

Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)data, len, HAL_MAX_DELAY);
    
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    return 0;// return serialReceive(&debugSerial, (uint8_t *)data, len, 0);
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

void pc_uart_receive_semaphore_sync(void *sem)
{
   //发送同步信号量，同步数据处理任务
	if (osSemaphoreRelease(sem) != osOK ) {
		;//ssz_traceln("Sem release fail.\n", com_id, ch);//错误处理
    }
}

int pc_comm_receive_byte_cb(uint8_t ch)
{
    shellHandler(&shell, ch);
	return 0;
}

/**
 * @brief 用户shell初始化
 * 
 */

osSemaphoreId_t pc_uart_semaphore; 

void userShellInit(void)
{
    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
    log_init();

    pc_uart_semaphore = osSemaphoreNew(1U, 0U, NULL);

    if (pc_uart_semaphore == NULL) {
        logError("pc_uart_semaphore created\n");
    }

    mid_com_init(k_ComPCUart, pc_uart_semaphore, pc_uart_receive_semaphore_sync, pc_comm_receive_byte_cb);

    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS) {
        logError("shell task creat failed");
    }
}


