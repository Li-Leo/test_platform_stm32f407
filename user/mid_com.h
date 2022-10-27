/*------------------------------------------------------------------------------------
 * @ Description  :
 * @ Change Logs:
 * Date           Author       Notes
 *
 *
 * @ Copyright (C) . All rights reserved.
 -------------------------------------------------------------------------------------*/

/* Define to prevent recursive inclusion ---------------------------------------------*/
#ifndef __MID_COM_H
#define __MID_COM_H

/* Private includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "ssz_safe_queue.h"

/* Exported types --------------------------------------------------------------------*/

/* Exported constants ----------------------------------------------------------------*/

/* Exported macro --------------------------------------------------------------------*/
/* 接收一字节数据回调函数 */
typedef void (*DriverRecieveHandle)(void *Sem);

/* ch is the received byte */
typedef int (*MidComReceiveHandler)(uint8_t ch);

typedef struct
{
    uint8_t buffer[48];                        //数据缓存区
    SszSafeQueue com_recv_queue;               //数据缓存队列
    DriverRecieveHandle DriRecieveHandleFunc;  //接收一字节数据同步回调函数
    MidComReceiveHandler ComReceiveHandleFunc; //接收一字节数据处理回调函数
    void *SemID;
    void *obj;
} struct_com_t;

typedef enum
{
    k_ComPCUart = 0, // communicate with PC by UART
    // k_ComMCUUart,    // Master-Slaver communicate

    kComIDMax
} enum_com_id;

/* Exported functions prototypes -----------------------------------------------------*/
bool uart_send(void *obj, const void *buff, int buff_size);
void mid_com_init(enum_com_id com_id, void *sem, DriverRecieveHandle handler1, MidComReceiveHandler handler2);

/* 一字节数据接收处理函数 */
void com_handle_new_received_data(enum_com_id com_id);
struct_com_t *getDrvComInfo(enum_com_id com_id);

/* Enddefine to prevent recursive inclusion ------------------------------------------*/
#endif /* __MID_COM_H */

