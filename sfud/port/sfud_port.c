/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>
#include <stdint.h>
#include "main.h"
#include "spi.h"
#include "drv_gpio.h"

static char log_buf[256];
/* Flash对应的SPI接口 */
static SPI_HandleTypeDef *spi = &hspi1;

void sfud_log_debug(const char *file, const long line, const char *format, ...);
void sfud_log_info(const char *format, ...);

/**
 * spi write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi_dev, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {

    sfud_err result = SFUD_SUCCESS;

    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }

	//if both write size and read size are not equal zero, do write and read.
	if (write_size && read_size) {
        drv_gpio_set_pin_low(FLASH_CS_GPIO_Port, FLASH_CS_Pin);
        if (HAL_SPI_Transmit(spi, (uint8_t *)write_buf, write_size, HAL_MAX_DELAY) == HAL_OK) {
            ;
        }

        if(HAL_SPI_Receive(spi , (uint8_t *)read_buf, read_size, HAL_MAX_DELAY) == HAL_OK) {
            ;
        }
        drv_gpio_set_pin_high(FLASH_CS_GPIO_Port, FLASH_CS_Pin);

        // SFUD_INFO("spi write and read data.");
    } else if (write_size) {
        drv_gpio_set_pin_low(FLASH_CS_GPIO_Port, FLASH_CS_Pin);

        if (HAL_SPI_Transmit(spi, (uint8_t *)write_buf, write_size, HAL_MAX_DELAY) == HAL_OK) {
            ;
        }

        drv_gpio_set_pin_high(FLASH_CS_GPIO_Port, FLASH_CS_Pin);

		// SFUD_INFO("spi write data.");
	} else if (read_size) {
        drv_gpio_set_pin_low(FLASH_CS_GPIO_Port, FLASH_CS_Pin);

        if (HAL_SPI_Receive(spi , (uint8_t *)read_buf, read_size, HAL_MAX_DELAY) == HAL_OK) {

        }

        drv_gpio_set_pin_high(FLASH_CS_GPIO_Port, FLASH_CS_Pin);
        // SFUD_INFO("spi read data.");
    } else {
        ;          
	}

    return result;
}

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your qspi read flash data code
     */

    return result;
}
#endif /* SFUD_USING_QSPI */

void delay_us(uint32_t us)
{
    uint32_t counter;

    /* Compute number of CPU cycles to wait */
    counter = ((us + 4) / 5 * (HAL_RCC_GetHCLKFreq() / 1000000U));
    
    do {
        ;
    } while (counter--);
}

static void retry_delay_100us(void) 
{
	delay_us(100U);
}

static void spi_lock(const sfud_spi *spi) 
{
    ;
    //sfud_flash *sfud_dev = (sfud_flash *) (spi->user_data);
    //struct spi_flash_device *rtt_dev = (struct spi_flash_device *) (sfud_dev->user_data);

    //rt_mutex_take(&(rtt_dev->lock), RT_WAITING_FOREVER);
}

static void spi_unlock(const sfud_spi *spi) 
{
    ;
    //sfud_flash *sfud_dev = (sfud_flash *) (spi->user_data);
    //struct spi_flash_device *rtt_dev = (struct spi_flash_device *) (sfud_dev->user_data);

    //rt_mutex_release(&(rtt_dev->lock));
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */
    switch (flash->index) {
        case SFUD_W25Q128JV_DEVICE_INDEX: {
            drv_gpio_set_pin_high(FLASH_CS_GPIO_Port, FLASH_CS_Pin);

            /* 同步 Flash 移植所需的接口及数据 */
            flash->spi.wr = spi_write_read;
            flash->spi.lock = spi_lock;
            flash->spi.unlock = spi_unlock; 
            /* about 100 microsecond delay */
            flash->retry.delay = retry_delay_100us;
            /* adout 60 seconds timeout */
            flash->retry.times = 60 * 10000;

            break;
        }
    }
    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\r\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...)
{
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\r\n", log_buf);
    va_end(args);
}
