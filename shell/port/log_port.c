/**
 * @file log_port.c
 * @author jlli
 * @brief 
 * @version 0.1
 * @date 2020-09-03
 * 
 * @copyright (c) 2019 Letter
 * 
 */
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "shell_port.h"
#include "log.h"
#include "stdbool.h"
#include "shell_port.h"

/************************************************
* Variable 
************************************************/
void uartLogWrite(char *buffer, short len);

static Log uartLog = 
{
    .write = uartLogWrite,
    .active = true,
    .level = LOG_DEBUG,
    .shell = &shell
};

void uartLogWrite(char *buffer, short len)
{
    // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, HAL_MAX_DELAY);
    if (uartLog.shell)
    {
        shellWriteEndLine(uartLog.shell, buffer, len);
    }
}



void log_init()
{
    logRegister(&uartLog, NULL);
}

