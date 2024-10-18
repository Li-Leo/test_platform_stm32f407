#include "led_tm1652.h"
#include "log.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"


unsigned char digit[10] = {0x3f,0x06,0x9b,0x8f,0xa6,0xad, 0xbd,0x07,0xbf,0xaf};

void delay_us_aaa(int us)
{
    uint32_t counter;

    counter = ((us+4)/5 * (SystemCoreClock / 1000000U));
    do {
        __NOP();
    } while(counter--);
}

void tm1652_send_byte(unsigned char data)
{
    unsigned char i = 0, sfalg = 0;

    //start bit
    SDA_HIGH;
    SDA_LOW;
    delay_us_aaa(DELAY_TIME);

    //data byte 8bits
    for (i = 0; i < 8; i++)
    {
        if (data & 0x01) {
            SDA_HIGH;
            sfalg++;
        } else {
            SDA_LOW;
        }
        delay_us_aaa(DELAY_TIME);
        data >>= 1;
    }

    //check bit
    if (sfalg % 2 == 0) {
        SDA_HIGH;
    } else {
        SDA_LOW;
    }
    delay_us_aaa(DELAY_TIME);

    //stop bit
    SDA_HIGH;
    delay_us_aaa(DELAY_TIME);
}

void led_off(void)
{
    tm1652_send_byte(0x18);
    tm1652_send_byte(0x00);

    delay_us_aaa(3000);
}

void led_on(void)
{
    tm1652_send_byte(0x18);
    tm1652_send_byte(0x1e);

    delay_us_aaa(3000);
}

void led_display(uint32_t value)
{
    unsigned char i;
    unsigned char count;
    unsigned char code[5];
    uint32_t tmp = value;

    if (tmp == 0) {
        count = 1;
    }

    while (tmp != 0) {
        tmp /= 10;
        ++count;
    }

    code[0] = digit[value/10000];
    code[1] = digit[value/1000%10];
    code[2] = digit[value/100%10];
    code[3] = digit[value/10%10];
    code[4] = digit[value%10];

    tm1652_send_byte(0x08);

    for (i = 0; i < 5; i++) {
        if (i < 5 - count) {
            code[i] = 0;
        }
        tm1652_send_byte(code[i]);
    }

    delay_us_aaa(3000);

    led_off();
    led_on();
}

char *s = "0123456789";
void led_str(char *str)
{
    unsigned char i;
    unsigned char code[5];

    tm1652_send_byte(0x08);

    for (i = 0; i < 5; i++) {

        code[i] = digit[*(str++) - '0'];

        tm1652_send_byte(code[i]);
    }

    delay_us_aaa(3000);

    led_off();
    led_on();
}


int g_pos = 0;

void display(void)
{
    char str[5];
    
    for (int i = 0; i < 5; i++) {
        if (g_pos < 6) {
            str[i] = *(s + g_pos + i);
        } else {
            if (i < 10 - g_pos) {
                str[i] = *(s + g_pos + i);
            } else {
                str[i] = *(s + i - (10 - g_pos));
            }
        }
    }
    
    led_str(str);
}

void display1(void)
{
    char str[5];
    
    for (int i = 0; i < 5; i++) {
        if (g_pos < 6) {
            str[i] = ('0' + g_pos + i);
        } else {
            if (i < 10 - g_pos) {
                str[i] = ('0' + g_pos + i);
            } else {
                str[i] = ('0' + i - (10 - g_pos));
            }
        }
    }
    
    led_str(str);
}

void g_pos_set(int t)
{
    g_pos = t;
}

void che_timer_callback(void *parameter);
osTimerId_t scan_timer_id;
void start_display(void)
{
    scan_timer_id = osTimerNew(che_timer_callback, osTimerPeriodic, NULL, NULL);
    osTimerStart(scan_timer_id, 100);
}

void che_timer_callback(void *parameter)
{
    display();

    if (g_pos <= 9) {
        g_pos++;
    } else {
        g_pos = 0;
    }

}

void stop_display()
{
    osTimerStop(scan_timer_id);
}


// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led_test, led_test, led_test);
// SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led_test1, led_test1, led_test1);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led_on, led_on, led_on);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led_off, led_off, led_off);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), led_display, led_display, led_display);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), display, display, display);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), display1, display1, display1);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), start_display, start_display, start_display);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), stop_display, stop_display, stop_display);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), g_pos_set, g_pos_set, g_pos);
