#include "drv_key.h"

/************************************************
* Declaration
************************************************/

/************************************************
* Variable 
************************************************/
const static struct key_gpio_config  g_key_gpios[kKeyIDMax] = {
    //{KEY_POWER_GPIO_Port, KEY_POWER_Pin, false},
    {KEY2_GPIO_Port, KEY2_Pin, true},
    {KEY3_GPIO_Port, KEY3_Pin, false},	
    {KEY4_GPIO_Port, KEY4_Pin, false},
};

/************************************************
* Function 
************************************************/
//it will scan all key state and put at the param
void drv_key_scan(int8_t key_state[kKeyIDMax])
{
    for (int i = 0; i < (int)kKeyIDMax; ++i) {
        key_state[i] = (int8_t)drv_key_is_pressed((KeyID)i);
    }
}

bool drv_key_is_pressed(KeyID key)
{
    if (HAL_GPIO_ReadPin(g_key_gpios[key].port, g_key_gpios[key].pin) == g_key_gpios[key].pressed_value) {
        return true;
    } else {
        return false;
    }
}


// bool drv_key_is_high(KeyID key){
// 	return ssz_gpio_is_high(g_key_gpios[key].gpio, g_key_gpios[key].pin_mask);
// }

