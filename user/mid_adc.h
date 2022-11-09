/*------------------------------------------------------------------------------------
 * @ Description  : ADC functions
 * @ Change Logs:
 * Date           Author       Notes
 * 2022-11-09     jlli      the first version
 * 
 * @ Copyright (C) 2022 Alfa Medtech. All rights reserved.
 -------------------------------------------------------------------------------------*/

#ifndef __MID_ADC_H
#define __MID_ADC_H

#define ADC_GET_AVERAGE_NUMBER 10       
#define ADC_REF_VOLTAGE 3300 
/* adc max digit code = 2^(adc's bits) */
#define ADC_MAX_DIGIT_CODE 4095
#define BATTERY_VOLTAGE_RADIO 2
#define M_get_battery_voltage_from_adc(adc_value) ((adc_value) * ADC_REF_VOLTAGE * BATTERY_VOLTAGE_RADIO / ADC_MAX_DIGIT_CODE)

typedef enum
{
    kChannel5 = 5,
    kChannel6,
    kChannel7,
    
    kADCChanelMAX,
} adc_channel;

uint32_t get_one_channel_avg_adc(adc_channel ch);

#endif /* __MID_ADC_H */

