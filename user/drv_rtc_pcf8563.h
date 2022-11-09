#ifndef _DRV_RTC_H_
#define _DRV_RTC_H_
//
//	Uncomment when you are using DMA reading
//
// #define PCF8563_USE_DMA
#include <stdint.h>
#include "i2c.h"
#include "ssz_time_utility.h"

#define PCF8563_ADDRESS (0x51 << 1)
#define PCF8563_ADDRESS_READ  0xA3
#define PCF8563_ADDRESS_WRITE 0xA2
#define PCF8563_I2C_TIMEOUT 100

#define PCF8563_REG_CONTROL_STATUS1 0x00
#define PCF8563_REG_CONTROL_STATUS2 0x01
#define PCF8563_REG_TIME 0x02
#define PCF8563_REG_SECONDS 0x02
#define PCF8563_REG_MINUTES 0x03
#define PCF8563_REG_HOURS 0x04
#define PCF8563_REG_DATE 0x05
#define PCF8563_REG_WEEKDAY 0x06
#define PCF8563_REG_MONTH 0x07
#define PCF8563_REG_YEAR 0x08
#define PCF8563_REG_ALARM_MINUTE 0x09
#define PCF8563_REG_ALARM_HOUR 0x0A
#define PCF8563_REG_ALARM_DAY 0x0B
#define PCF8563_REG_ALARM_WEEKDAY 0x0C
#define PCF8563_REG_CLKOUT 0x0D
#define PCF8563_REG_TIMER_CONTROL 0x0E
#define PCF8563_REG_TIMER 0x0F

//
//	Controll register 1 0x00
//
#define PCF8563_CONTROL1_TEST1 7
#define PCF8563_CONTROL1_STOP 5
#define PCF8563_CONTROL1_TESTC 3

//
//	Controll register 2 0x01
//
#define PCF8563_CONTROL2_TI_TP 4
#define PCF8563_CONTROL2_AF 3
#define PCF8563_CONTROL2_TF 2
#define PCF8563_CONTROL2_AIE 1
#define PCF8563_CONTROL2_TIE 0

//
//	CLKOUT control register 0x0D
//
#define PCF8563_CLKOUT_CONTROL_FE 7
#define PCF8563_CLKOUT_CONTROL_FD1 1
#define PCF8563_CLKOUT_CONTROL_FD0 0

typedef enum
{
	CLKOUT_FREQ_1HZ = 3,
	CLKOUT_FREQ_32HZ = 2,
	CLKOUT_FREQ_1024HZ = 1,
	CLKOUT_FREQ_32768HZ = 0
} clk_out_freq_t;

void drv_rtc_test1_enable(uint8_t value);
void drv_rtc_stop_enable(uint8_t value);
void drv_rtc_testc_enable(uint8_t value);
void drv_rtc_interrupt_enable(uint8_t value);
void drv_rtc_alarm_flag_enable(uint8_t value);
void drv_rtc_timer_flag_enable(uint8_t value);
void drv_rtc_alarm_interrupt_enable(uint8_t value);
void drv_rtc_timer_interrupt_enable(uint8_t value);
void drv_rtc_clk_out(clk_out_freq_t freq);

// void PCF8563_ReceiveDateTimeDMA(void);                  // Use in PCF8563 Interrupt handler
void drv_rtc_cal_date_time(date_time *dt);              // Use in DMA Complete Receive interrupt
void drv_rtc_set_date_time(date_time *dt);              // Use in blocking/interrupt mode in PCF8563_INT EXTI handler


void drv_rtc_int(I2C_HandleTypeDef *hi2c);

#endif
