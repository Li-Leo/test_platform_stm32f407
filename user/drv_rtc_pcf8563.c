/*
 *  PCF8563.c
 *
 *	The MIT License.
 *  Created on: 5.09.2019
 *      Author: jlli
 *		Contact: ljl_leo@163.com
 *
 */
#include "main.h"
#include "drv_rtc_pcf8563.h"

I2C_HandleTypeDef *hi2c_pcf8563;
uint8_t g_rtc_buffer[7];

void write_control_register_bit(uint8_t reg, uint8_t bit, uint8_t value)
{
    uint8_t tmp;

    HAL_I2C_Mem_Read(hi2c_pcf8563, PCF8563_ADDRESS_READ, reg, 1, &tmp, 1, PCF8563_I2C_TIMEOUT);
    tmp &= ~(1 << bit);
    tmp |= (value << bit);

    if (reg == PCF8563_REG_CONTROL_STATUS1) 
        tmp &= 0xA8;
    else
        tmp &= 0x1F;

    HAL_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS_WRITE, reg, 1, &tmp, 1, PCF8563_I2C_TIMEOUT);
}

void drv_rtc_test1_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS1, PCF8563_CONTROL1_TEST1, value);
}

void drv_rtc_stop_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS1, PCF8563_CONTROL1_STOP, value);
}

void drv_rtc_testc_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS1, PCF8563_CONTROL1_TESTC, value);
}

void drv_rtc_interrupt_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_TI_TP, value);
}

void drv_rtc_alarm_flag_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_AF, value);
}

void drv_rtc_timer_flag_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_TF, value);
}

void drv_rtc_alarm_interrupt_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_AIE, value);
}

void drv_rtc_timer_interrupt_enable(uint8_t value)
{
    write_control_register_bit(PCF8563_REG_CONTROL_STATUS2, PCF8563_CONTROL2_TIE, value);
}

void drv_rtc_clockout_enable(uint8_t value)
{
    HAL_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS_WRITE, PCF8563_REG_CLKOUT, 1, &value, 1, PCF8563_I2C_TIMEOUT);
}

void drv_rtc_clk_out(clk_out_freq_t freq)
{
    uint8_t tmp;

    HAL_I2C_Mem_Read(hi2c_pcf8563, PCF8563_ADDRESS_READ, PCF8563_REG_CLKOUT, 1, &tmp, 1, PCF8563_I2C_TIMEOUT);
    tmp &= ~(3 << PCF8563_CLKOUT_CONTROL_FD0);
    tmp |= (freq << PCF8563_CLKOUT_CONTROL_FD0);
    HAL_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS_WRITE, PCF8563_REG_CLKOUT, 1, &tmp, 1, PCF8563_I2C_TIMEOUT);
}

uint8_t bcd2dec(uint8_t bcd_code)
{
    return (((bcd_code & 0xF0) >> 4) * 10) + (bcd_code & 0xF);
}

uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10) << 4) + (dec % 10);
}

// int dayofweek(int Day, int Month, int Year)
// {
//     int Y, C, M, N, D;
//     M = 1 + (9 + Month) % 12;
//     Y = Year - (M > 10);
//     C = Y / 100;
//     D = Y % 100;
//     N = ((13 * M - 1) / 5 + D + D / 4 + 6 * C + Day + 5) % 7;
//     return (7 + N) % 7;
// }

void drv_rtc_set_date_time(date_time *datetime)
{
    uint8_t tmp[7];

    if (datetime->second > 59)
        datetime->second = 59;
    if (datetime->minute > 59)
        datetime->minute = 59;
    if (datetime->hour > 23)
        datetime->hour = 23;
    if (datetime->day > 31)
        datetime->day = 31;
    if (datetime->month > 12)
        datetime->month = 12;
    if (datetime->year > 2099)
        datetime->year = 2099;

    tmp[0] = dec2bcd((datetime->second) & 0x7F);
    tmp[1] = dec2bcd(datetime->minute);
    tmp[2] = dec2bcd(datetime->hour);
    tmp[3] = dec2bcd(datetime->day);
    tmp[4] = ssz_week_day( datetime->year, datetime->month, datetime->day);
    tmp[5] = dec2bcd(datetime->month);
    tmp[6] = dec2bcd(datetime->year - SSZ_BASE_YEAR);

    HAL_I2C_Mem_Write(hi2c_pcf8563, PCF8563_ADDRESS_WRITE, PCF8563_REG_TIME, 1, tmp, 7, PCF8563_I2C_TIMEOUT);
}

void drv_rtc_cal_date_time(date_time *datetime)
{
    datetime->second = bcd2dec((g_rtc_buffer[0]) & 0x7F);
    datetime->minute = bcd2dec(g_rtc_buffer[1]);
    datetime->hour = bcd2dec(g_rtc_buffer[2]);
    datetime->day = g_rtc_buffer[3];
    datetime->weekday = bcd2dec(g_rtc_buffer[4] + 1); // too keep weekdays in 1-7 format
    datetime->month = bcd2dec(g_rtc_buffer[5] & 0x1F);
    datetime->year = SSZ_BASE_YEAR + bcd2dec(g_rtc_buffer[6]);
}

#ifdef PCF8563_USE_DMA

void PCF8563_ReceiveDateTimeDMA(void)
{
    HAL_I2C_Mem_Re
    ad_DMA(hi2c_pcf8563, PCF8563_ADDRESS, PCF8563_REG_TIME, 1, Pcf8563Buffer, 7);
}

#else

void drv_rtc_get_date_time(date_time *dt)
{
    HAL_I2C_Mem_Read(hi2c_pcf8563, PCF8563_ADDRESS_READ, PCF8563_REG_TIME, 1, g_rtc_buffer, 7, PCF8563_I2C_TIMEOUT);
    drv_rtc_cal_date_time(dt);
}

#endif

void drv_rtc_int(I2C_HandleTypeDef *hi2c)
{
    hi2c_pcf8563 = &hi2c1;

    // drv_rtc_clk_out(CLKOUT_FREQ_1HZ);
    drv_rtc_clockout_enable(0);
    drv_rtc_stop_enable(0);
}
