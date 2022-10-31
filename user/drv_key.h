/************************************************
* DESCRIPTION:
*
************************************************/
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/************************************************
* Declaration
************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	//kKeyPower,
	kKey2,
	kKey3,
	kKey4,
	kKeyIDMax
}KeyID;

struct key_gpio_config {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool pressed_value;
};

//it will scan all key state and put at the param
void drv_key_scan(int8_t key_state[kKeyIDMax]);
bool drv_key_is_pressed(KeyID key);
// bool drv_key_is_high(KeyID key);

#ifdef __cplusplus
}
#endif


