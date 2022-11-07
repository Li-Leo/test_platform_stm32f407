/*
 * Copyright (c) 2020, Armink, <armink.ztl@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#define FAL_DEBUG 1
#define FAL_PART_HAS_TABLE_CFG
#define FAL_USING_SFUD_PORT

#define NOR_FLASH_DEV_NAME             "norflash0"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32_onchip_flash;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash,                                           \
    &nor_flash0,                                                     \
}

/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                \
{                                                                                     \
    {FAL_PART_MAGIC_WORD,  "fdb_tsdb1",    "stm32_onchip",   256*1024,  256*1024, 0},  \
    {FAL_PART_MAGIC_WORD,  "fdb_kvdb1",    "stm32_onchip",   512*1024,  256*1024, 0},  \
    {FAL_PART_MAGIC_WORD, "fdb_tsdb2", NOR_FLASH_DEV_NAME,         0,   1024*1024*4, 0}, \
    {FAL_PART_MAGIC_WORD, "fdb_kvdb2", NOR_FLASH_DEV_NAME, 4*1024*1024, 4*1024*1024, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
