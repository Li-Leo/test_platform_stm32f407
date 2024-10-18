#include "stdint.h"
#include "main.h"

#define DELAY_TIME 40

#define SDA_HIGH    drv_gpio_set_pin_high(GPIOE,GPIO_PIN_15)
#define SDA_LOW     drv_gpio_set_pin_low(GPIOE,GPIO_PIN_15)

void led_display(uint32_t value);
void display(void);
