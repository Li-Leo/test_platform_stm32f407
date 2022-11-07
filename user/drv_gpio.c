#include "drv_gpio.h"
#include "main.h"

void drv_gpio_set_pin_high(GPIO_TypeDef* gpio, uint16_t pin)
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
}

void drv_gpio_set_pin_low(GPIO_TypeDef* gpio, uint16_t pin)
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}

void drv_gpio_toggle_pin(GPIO_TypeDef* gpio, uint16_t pin)
{
    HAL_GPIO_TogglePin(gpio, pin);
}

