
#include "ssz_common.h"
//#include "string.h"
#include "ssz_check.h"

/************************************************
* Declaration
************************************************/
//define the temp buff size, refer ssz_tmp_buff_alloc
#define SSZ_COMMON_TMP_BUFF_MAX_SIZE 512
/************************************************
* Variable 
************************************************/ 
// static uint8_t g_common_tmp_buff[SSZ_COMMON_TMP_BUFF_MAX_SIZE];
// static uint8_t g_backup_tmp_buff_when_write_flash[1];
// static bool g_tmp_buff_is_used[kTempAllocBuffTypeMax] = {0};
/************************************************
* Function 
************************************************/

//check if the tmp buff is >=alloc_size
// void ssz_tmp_buff_check_size(TempAllocBuffType alloc_type, int alloc_size)
// {
// 	switch (alloc_type) {
// 	case kTempAllocBackupBuffWhenWriteFlash:
// 		//ssz_check(ssz_array_size(g_backup_tmp_buff_when_write_flash) >= alloc_size);
// 		break;
// 	case kTempAllocCommonBuff:
// 		//ssz_check(ssz_array_size(g_common_tmp_buff) >= alloc_size);
// 		break;
// 	default:
// 		ssz_assert_fail();
// 		break;
// 	}
// }
//alloc tmp buff, once can only one use it
// void* ssz_tmp_buff_alloc(TempAllocBuffType alloc_type, int alloc_size)
// {
// 	//ssz_check(g_tmp_buff_is_used[alloc_type] == false);
// 	g_tmp_buff_is_used[alloc_type] = true;
// 	switch (alloc_type) {
// 	case kTempAllocBackupBuffWhenWriteFlash:
// 		//ssz_check(ssz_array_size(g_backup_tmp_buff_when_write_flash) >= alloc_size);
// 		return g_backup_tmp_buff_when_write_flash;
// 		//break;
// 	case kTempAllocCommonBuff:
// 		//ssz_check(ssz_array_size(g_common_tmp_buff) >= alloc_size);
// 		return g_common_tmp_buff;
// 		//break;
// 	}
// 	ssz_assert_fail();
// 	return NULL;
// }
//free tmp buff
// void ssz_tmp_buff_free(void* alloc_buff) {
// 	TempAllocBuffType alloc_type = kTempAllocCommonBuff;

// 	if (alloc_buff == g_backup_tmp_buff_when_write_flash) {
// 		alloc_type = kTempAllocBackupBuffWhenWriteFlash;
// 	}
// 	else if (alloc_buff == g_common_tmp_buff) {
// 		alloc_type = kTempAllocCommonBuff;
// 	}else{
// 		//ssz_check(0);
// 	}
// 	g_tmp_buff_is_used[alloc_type] = false;
// }
