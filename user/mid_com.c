/*------------------------------------------------------------------------------------
 * @ Description  : 
 * @ Change Logs:
 * Date           Author       Notes
 * 
 * 
 * @ Copyright (C). All rights reserved.
 -------------------------------------------------------------------------------------*/


/* Includes --------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "string.h"
#include "ssz_safe_queue.h"
#include "mid_com.h"
#include "usart.h"
#include "ssz_check.h"
#include "ssz_common.h"
#include "shell.h"
#include "log.h"

extern UART_HandleTypeDef huart1;

static struct_com_t s_com_info[kComIDMax] = 
{
    {
        .obj = (void *)&huart1,
    },
};

/* functions ------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------
 *@ description: 
 *@ param {*}
 *@ return {*}
 -------------------------------------------------------------------------------------*/
bool uart_send(void* obj, const void *buff, int buff_size)
{
    if (HAL_UART_Transmit((UART_HandleTypeDef*)obj, (uint8_t*)buff, buff_size, HAL_MAX_DELAY) != HAL_OK) {
        return false;
    }
    
   return true;
}

/*------------------------------------------------------------------------------------
 *@ description: 
 *@ param {*}
 *@ return {*}
 -------------------------------------------------------------------------------------*/
//usually called at interrupt handler, save one byte into buffer.
static void com_receive_byte(enum_com_id com_id, unsigned char ch)
{
    if (ssz_safe_queue_push(&s_com_info[com_id].com_recv_queue, &ch) != 0) {
        //ssz_traceln("com[%d] is full, ignore byte[%x]\n", com_id, ch);
    }

    //同步接收到数据
    s_com_info[com_id].com_recieve_semaphore_sync_cb(s_com_info[com_id].semaphore);
}

/*------------------------------------------------------------------------------------
 *@ description: 
 *@ param {*}
 *@ return {*}
 -------------------------------------------------------------------------------------*/
// handle one byte received
void com_handle_new_received_data(enum_com_id com_id)
{
    uint8_t *data;
   
    //从队列中取出数据，有可能接收了多字节数据，没来得及处理。
    for (;;) {
        if (!ssz_safe_queue_is_empty(&s_com_info[com_id].com_recv_queue)) {
            data = ssz_safe_queue_front(&s_com_info[com_id].com_recv_queue);

            if (s_com_info[com_id].com_recieve_one_byte_cb(*data) == false) {
                //ssz_traceln("com %d receive unknow byte[%x]\n", com_id, a);
            }

            ssz_safe_queue_pop(&s_com_info[com_id].com_recv_queue);
        } else {
            return;  //队列中数据取出完毕。
        }
    }
}

/*------------------------------------------------------------------------------------
 *@ description: 
 *@ param {*}
 *@ return {*}
 -------------------------------------------------------------------------------------*/
void mid_com_init(enum_com_id com_id, void *sem, com_recieve_semaphore_sync_handler handler1, com_recieve_one_byte_handler handler2)
{

    //初始化设备信息
    s_com_info[com_id].com_recieve_semaphore_sync_cb = handler1;
    s_com_info[com_id].com_recieve_one_byte_cb = handler2;
    s_com_info[com_id].semaphore  = sem ;
    //初始化接收缓存队列
    ssz_safe_queue_init(&s_com_info[com_id].com_recv_queue, 
        s_com_info[com_id].buffer,
        ssz_array_size(s_com_info[com_id].buffer), 
        ssz_array_node_size(s_com_info[com_id].buffer));
    
    //使能接收中断
    __HAL_UART_ENABLE_IT((UART_HandleTypeDef*)s_com_info[com_id].obj, UART_IT_RXNE);
}

/*------------------------------------------------------------------------------------
 *@ description: 
 *@ param {*}
 *@ return {*}
 -------------------------------------------------------------------------------------*/
struct_com_t *get_com_info(enum_com_id com_id)
{
    return &s_com_info[com_id];
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: the HAL_UART_RxCpltCallback could be implemented in the user file
    */

    /* USER CODE BEGIN */
    if ((huart == &huart1) && (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))) {
        //save one byte data and release semaphore
        com_receive_byte(k_ComPCUart, __HAL_UART_FLUSH_DRREGISTER(huart));

        return;
    }

    /* USER CODE END */
}

/**
  * private callbacsk function which has pointer to 
  * a DMA_HandleTypeDef structure as parameter.
  */
void UART_DMA_XferCpltCallback( DMA_HandleTypeDef * dma)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(dma);

    /* USER CODE BEGIN */


    /* USER CODE END */
}
