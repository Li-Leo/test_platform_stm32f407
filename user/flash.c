#include "sfud.h"
#include "shell.h"

/**
 * SFUD demo for the first flash device test.
 *
 * @param addr flash start address
 * @param size test flash size
 * @param size test flash data buffer
 */

#define SFUD_DEMO_TEST_BUFFER_SIZE 1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];

void sfud_demo(uint32_t addr, size_t size, uint8_t *data)
{
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    size_t i;
    /* prepare write data */
    for (i = 0; i < size; i++)
    {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS)
    {
        printf("Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
               size);
    }
    else
    {
        printf("Erase the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* write test */
    result = sfud_write(flash, addr, size, data);
    if (result == SFUD_SUCCESS)
    {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
               size);
    }
    else
    {
        printf("Write the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* read test */
    result = sfud_read(flash, addr, size, data);
    if (result == SFUD_SUCCESS)
    {
        printf("Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", flash->name, addr,
               size);
        printf("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++)
        {
            if (i % 16 == 0)
            {
                printf("[%08X] ", addr + i);
            }
            printf("%02X ", data[i]);
            if (((i + 1) % 16 == 0) || i == size - 1)
            {
                printf("\r\n");
            }
        }
        printf("\r\n");
    }
    else
    {
        printf("Read the %s flash data failed.\r\n", flash->name);
    }
    /* data check */
    for (i = 0; i < size; i++)
    {
        if (data[i] != i % 256)
        {
            printf("Read and check write data has an error. Write the %s flash data failed.\r\n", flash->name);
            break;
        }
    }
    if (i == size)
    {
        printf("The %s flash test is success.\r\n", flash->name);
    }
}

void flash_read(uint32_t addr, size_t size)
{
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    size_t i;
    
    result = sfud_read(flash, addr, size, sfud_demo_test_buf);
    if (result == SFUD_SUCCESS)
    {
        printf("Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", flash->name, addr,
               size);
        printf("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++)
        {
            if (i % 16 == 0)
            {
                printf("[%08X] ", addr + i);
            }
            printf("%02X ", sfud_demo_test_buf[i]);
            if (((i + 1) % 16 == 0) || i == size - 1)
            {
                printf("\r\n");
            }
        }
        printf("\r\n");
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), read, flash_read, read flash [addr] [size]);

void flash_write(uint32_t addr, uint8_t data)
{
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    
    result = sfud_write(flash, addr, 1, &data);
    if (result == SFUD_SUCCESS)
    {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr, 1);
    }
    else
    {
        printf("Write the %s flash data failed.\r\n", flash->name);
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), write, flash_write, write one byte to flash [addr] [data]);

void flash_erase(uint32_t addr, size_t size)
{
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;

    
    result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS)
    {
        printf("Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
               size);
    }
    else
    {
        printf("Erase the %s flash data failed.\r\n", flash->name);
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), erase, flash_erase, erase flash [addr] [size]);

struct data_t {
    int a;
    int b;
    uint8_t c[16];
};

void flash_write_data_t(uint32_t addr)
{
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    struct data_t t = {.a = 0x1234, .b = 0x6789, .c = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};

    result = sfud_write(flash, addr, sizeof(t), (uint8_t *)&t);
    if (result == SFUD_SUCCESS)
    {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr, sizeof(t));
    }
    else
    {
        printf("Write the %s flash data failed.\r\n", flash->name);
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), write_struct, flash_write_data_t, write data_t to flash [addr] [data]);


void flash_read_data_t(uint32_t addr)
{
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    struct data_t tmp;
    
    result = sfud_read(flash, addr, sizeof(tmp), (uint8_t *)&tmp);
    if (result == SFUD_SUCCESS)
    {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr, sizeof(tmp));
        printf("tmp.a = %d, tmp.b = %d\r\n", tmp.a, tmp.b);
        for (int i = 0; i < 16; i++) {
            printf("tmp.c[%d] = %d ", i, tmp.c[i]);
        }
        printf("\r\n");

    }
    else
    {
        printf("Write the %s flash data failed.\r\n", flash->name);
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), read_struct, flash_read_data_t, read data_t from flash [addr]);
