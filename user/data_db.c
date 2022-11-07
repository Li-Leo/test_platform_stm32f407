#include <stdio.h>
#include <flashdb.h>
#include <stm32f4xx_hal.h>
#include "shell.h"
#include "stdlib.h"
#include "string.h"
#include "ssz_tick_time.h" 

#define FDB_LOG_TAG "[data_db]"
#ifdef FDB_USING_TIMESTAMP_64BIT
#define __PRITS "ld"
#else
#define __PRITS "d"
#endif

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};
/* default KV nodes */
static struct fdb_default_kv_node default_kv_table[] = {
        {"username", "armink", 0}, /* string KV */
        {"password", "123456", 0}, /* string KV */
        {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
        {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
/* KVDB object */
static struct fdb_kvdb g_kvdb = { 0 };
static struct fdb_kvdb g_kvdb_nor_flash = { 0 };
/* TSDB object */
struct fdb_tsdb tsdb = { 0 };
struct fdb_tsdb g_tsdb_nor = { 0 };
/* counts for simulated timestamp */
static int counts = 0;

extern void kvdb_basic_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_string_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_blob_sample(fdb_kvdb_t kvdb);
extern void tsdb_sample(fdb_tsdb_t tsdb);

static void lock(fdb_db_t db)
{
    __disable_irq();
}

static void unlock(fdb_db_t db)
{
    __enable_irq();
}

static fdb_time_t get_time(void)
{
    /* Using the counts instead of timestamp.
     * Please change this function to return RTC time.
     */
    return ++counts;
}

static fdb_time_t get_time1(void)
{
    /* Using the counts instead of timestamp.
     * Please change this function to return RTC time.
     */
    return (int32_t)ssz_tick_time_now();
}

int flash_db_test(void)
{
    fdb_err_t result;

#ifdef FDB_USING_KVDB
    { /* KVDB Sample */
        struct fdb_default_kv default_kv;

        default_kv.kvs = default_kv_table;
        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
        /* set the lock and unlock function if you want */
        fdb_kvdb_control(&g_kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
        fdb_kvdb_control(&g_kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);
        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_kvdb_init(&g_kvdb, "env", "fdb_kvdb1", &default_kv, NULL);

        if (result != FDB_NO_ERR) {
            return -1;
        }

        result = fdb_kvdb_init(&g_kvdb_nor_flash, "env", "fdb_kvdb2", NULL, NULL);

        if (result != FDB_NO_ERR) {
            return -1;
        }

        /* run basic KV samples */
        // kvdb_basic_sample(&g_kvdb);
        /* run string KV samples */
        // kvdb_type_string_sample(&g_kvdb);
        /* run blob KV samples */
        // kvdb_type_blob_sample(&g_kvdb);
    }
#endif /* FDB_USING_KVDB */

#ifdef FDB_USING_TSDB
    { /* TSDB Sample */
        /* set the lock and unlock function if you want */
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, (void *)lock);
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, (void *)unlock);
        /* Time series database initialization
         *
         *       &tsdb: database object
         *       "log": database name
         * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         *    get_time: The get current timestamp function.
         *         128: maximum length of each log
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_tsdb_init(&tsdb, "log", "fdb_tsdb1", get_time, 128, NULL);
        /* read last saved time for simulated timestamp */
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_GET_LAST_TIME, &counts);

        if (result != FDB_NO_ERR) {
            return -1;
        }

        result = fdb_tsdb_init(&g_tsdb_nor, "log", "fdb_tsdb2", get_time1, 128, NULL);
        if (result != FDB_NO_ERR) {
            return -1;
        }


        /* run TSDB sample */
        tsdb_sample(&tsdb);
    }
#endif /* FDB_USING_TSDB */
    return 0;
}

void kvdb_get_int(const char *key)  // fdb_kvdb_t kvdb,
{
    struct fdb_blob blob;
    int tmp = 0;


    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&g_kvdb, key, fdb_blob_make(&blob, &tmp, sizeof(tmp)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the value is %d\n", tmp);
    } else {
        FDB_INFO("get the failed\n");
    }
}

void kvdb_set_int(const char *key, int value)   // fdb_kvdb_t kvdb, 
{
    struct fdb_blob blob;

    /* change the "boot_count" KV's value */
    fdb_kv_set_blob(&g_kvdb, key, fdb_blob_make(&blob, &value, sizeof(value)));
    FDB_INFO("set the value to %d\n", value);
}

void kvdb_get_string(const char *key)  // fdb_kvdb_t kvdb,
{
    char *return_value, s[10] = { 0 };

    /* Get the "temp" KV value.
        * NOTE: The return value saved in fdb_kv_get's buffer. Please copy away as soon as possible.
        */
    return_value = fdb_kv_get(&g_kvdb, key);
    /* the return value is NULL when get the value failed */
    if (return_value != NULL) {
        strncpy(s, return_value, sizeof(s));
        FDB_INFO("get the value is: %s\n", s);
    }
}

void kvdb_set_string(const char *key, const char *s)   // fdb_kvdb_t kvdb, 
{
    /* It will create new KV node when "temp" KV not in database. */
    fdb_kv_set(&g_kvdb, key, s);
    FDB_INFO("create the string KV, value is: %s\n", s);
}

void kvdb_del(const char *key)   // fdb_kvdb_t kvdb, 
{
    fdb_kv_del(&g_kvdb, key);
    FDB_INFO("delete the %s finish\n", key);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kvdb_get_int, kvdb_get_int, get int value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kvdb_set_int, kvdb_set_int, set int value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kvdb_get_string, kvdb_get_string, get string value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kvdb_set_string, kvdb_set_string, set string value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kvdb_del, kvdb_del, del kv);

void kvdb_tarversal_sample(fdb_kvdb_t kvdb)
{
    struct fdb_kv_iterator iterator;
    fdb_kv_t cur_kv;
    struct fdb_blob blob;
    size_t data_size;
    uint8_t *data_buf;

    fdb_kv_iterator_init(&iterator);

    while (fdb_kv_iterate(kvdb, &iterator)) {
        cur_kv = &(iterator.curr_kv);
        data_size = (size_t) cur_kv->value_len;
        data_buf = (uint8_t *) malloc(data_size);
        if (data_buf == NULL) {
            FDB_INFO("Error: malloc failed.\n");
            break;
        }
        fdb_blob_read((fdb_db_t) kvdb, fdb_kv_to_blob(cur_kv, fdb_blob_make(&blob, data_buf, data_size)));
        FDB_INFO("kv name is %s\n", cur_kv->name);
        /*
         * balabala do what ever you like with blob...
         */
        free(data_buf);
    }
}
void kvdb_tarversal(void)
{
    kvdb_tarversal_sample(&g_kvdb);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kvdb_tarversal, kvdb_tarversal, kvdb_tarversal kv);


void kvdb_get_int1(const char *key)  // fdb_kvdb_t kvdb,
{
    struct fdb_blob blob;
    int tmp = 0;


    /* get the "boot_count" KV value */
    fdb_kv_get_blob(&g_kvdb_nor_flash, key, fdb_blob_make(&blob, &tmp, sizeof(tmp)));
    /* the blob.saved.len is more than 0 when get the value successful */
    if (blob.saved.len > 0) {
        FDB_INFO("get the value is %d\n", tmp);
    } else {
        FDB_INFO("get the failed\n");
    }
}

void kvdb_set_int1(const char *key, int value)   // fdb_kvdb_t kvdb, 
{
    struct fdb_blob blob;

    /* change the "boot_count" KV's value */
    fdb_kv_set_blob(&g_kvdb_nor_flash, key, fdb_blob_make(&blob, &value, sizeof(value)));
    FDB_INFO("set the value to %d\n", value);
}

void kvdb_del1(const char *key)   // fdb_kvdb_t kvdb, 
{
    fdb_kv_del(&g_kvdb_nor_flash, key);
    FDB_INFO("delete the %s finish\n", key);
}

void kvdb_tarversal_nor(void)
{
    kvdb_tarversal_sample(&g_kvdb_nor_flash);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), nor_get_int, kvdb_get_int1, get int value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), nor_set_int, kvdb_set_int1, set int value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), nor_del, kvdb_del1, del kv);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), nor_tarversal, kvdb_tarversal_nor, kvdb_tarversal_nor);


static bool query_cb(fdb_tsl_t tsl, void *arg);
struct env_status {
    int temp;
    int humi;
    char s[20];
};

void tsdb_add(fdb_tsdb_t tsdb)
{
    struct fdb_blob blob;

    FDB_INFO("==================== tsdb_add ====================\n");

    { /* APPEND new TSL (time series log) */
        struct env_status status;

        /* append new log to TSDB */
        status.temp = 36;
        status.humi = 85;
        memcpy(status.s, "first", strlen("first"));

        fdb_tsl_append(tsdb, fdb_blob_make(&blob, &status, sizeof(status)));
        FDB_INFO("append the new status.temp (%d) and status.humi (%d), %s\n", status.temp, status.humi, status.s);

        status.temp = 38;
        status.humi = 90;
        memcpy(status.s, "second", strlen("second"));
        fdb_tsl_append(tsdb, fdb_blob_make(&blob, &status, sizeof(status)));
        FDB_INFO("append the new status.temp (%d) and status.humi (%d), %s\n", status.temp, status.humi, status.s);
    }
}

void tsdb_iter(fdb_tsdb_t tsdb)
{
    fdb_tsl_iter(tsdb, query_cb, tsdb);
}


static bool query_cb(fdb_tsl_t tsl, void *arg)
{
    struct fdb_blob blob;
    struct env_status status;
    fdb_tsdb_t db = arg;

    fdb_blob_read((fdb_db_t) db, fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &status, sizeof(status))));
    FDB_INFO("[query_cb] queried a TSL: time: %" __PRITS ", temp: %d, humi: %d, %s\n", tsl->time, status.temp, status.humi, status.s);

    return false;
}

void tsdb_add_cmd(void)
{
    tsdb_add(&g_tsdb_nor);
}

void tsdb_iter_cmd(void)
{
    tsdb_iter(&g_tsdb_nor);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), tsdb_add_cmd, tsdb_add_cmd, tsdb_add tsl);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), tsdb_iter_cmd, tsdb_iter_cmd, tsdb_iter tsl);
