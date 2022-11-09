
#include <stdio.h>
#include "adc.h"
#include "mid_adc.h"
#include "shell.h"

void adc_channel_config(adc_channel ch)
{
    ADC_ChannelConfTypeDef sConfig;

    switch (ch) {
    case kChannel5:
      /* code */
      sConfig.Channel = ADC_CHANNEL_5;
      break;
    case kChannel6:
      /* code */
      sConfig.Channel = ADC_CHANNEL_6;
      break;
    case kChannel7:
      /* code */
      sConfig.Channel = ADC_CHANNEL_7;
      break;

    default:
      break;
    }

    sConfig.Rank = 1;                             
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;       

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        /* Channel Configuration Error */
        Error_Handler();
    }
}
 
uint16_t get_one_channel_adc(adc_channel ch)
{
    uint16_t result = 0;

    adc_channel_config(ch);
    
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        Error_Handler();
    }

    if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
    {
        result = HAL_ADC_GetValue(&hadc1);
    }

    return result;
}

uint32_t get_one_channel_avg_adc(adc_channel ch)
{
    uint32_t i;
    uint32_t adc_avg, sum, max, min;
    uint32_t adc_value[ADC_GET_AVERAGE_NUMBER] = {0};
    
    adc_value[0] = get_one_channel_adc(ch);
    max = adc_value[0];
    min = adc_value[0];
    sum = adc_value[0];

    for (i = 1; i < ADC_GET_AVERAGE_NUMBER; i++) {
        adc_value[i] = get_one_channel_adc(ch);
        
        if(adc_value[i] < min)
        {
            min = adc_value[i];
        }
            
        if(adc_value[i] > max)
        {
            max = adc_value[i];
        }
           
        sum += adc_value[i];
    }

    sum -= (max + min);
    adc_avg = sum / (ADC_GET_AVERAGE_NUMBER - 2);

    return adc_avg;
}

void test_get_adc(adc_channel ch)
{
  uint16_t result = get_one_channel_adc(ch);

  printf("adc_value = %u, voltage = %fmV\n", result, result * 3300.0 / 4095.0);
}

void test_get_adc_avg(adc_channel ch)
{
  uint16_t result = get_one_channel_avg_adc(ch);

  printf("avg_adc_value = %u, voltage = %fmV\n", result, result * 3300.0 / 4095.0);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), get_adc, test_get_adc, get adc value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), get_avg_adc, test_get_adc_avg, get adc avg value);

