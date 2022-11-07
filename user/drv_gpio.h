#ifndef _FDB_DEF_H_
#define _FDB_DEF_H_

#include "main.h"

void drv_gpio_set_pin_high(GPIO_TypeDef* gpio, uint16_t pin);
void drv_gpio_set_pin_low(GPIO_TypeDef* gpio, uint16_t pin);
void drv_gpio_toggle_pin(GPIO_TypeDef* gpio, uint16_t pin);

#endif

