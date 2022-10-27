#pragma once
#include "string.h"
#include "assert.h"
#include "stdio.h"
#include "ssz_tick_time.h" 
/************************************************
* Declaration
************************************************/


#define ssz_is_result_ok(result) ((result) == kSuccess)
#define ssz_is_result_fail(result) ((result) != kSuccess)
#define ssz_min(a,b)            (((a) < (b)) ? (a) : (b))
#define ssz_max(a,b)            (((a) > (b)) ? (a) : (b))

typedef enum {
	kTempAllocBackupBuffWhenWriteFlash,
	kTempAllocCommonBuff,
	kTempAllocBuffTypeMax,
}TempAllocBuffType;

#ifdef __cplusplus
extern "C" {
#endif

//check if the tmp buff is >=alloc_size
void ssz_tmp_buff_check_size(TempAllocBuffType alloc_type, int alloc_size);
//alloc tmp buff, once can only one use it
void* ssz_tmp_buff_alloc(TempAllocBuffType alloc_type, int alloc_size);
//free tmp buff
void ssz_tmp_buff_free(void* alloc_buff);

 
#define ssz_array_size(arr)	((sizeof(arr))/(sizeof(arr[0])))
#define ssz_array_node_size(arr)	sizeof(arr[0])
#define ssz_array_at(arr, index) (ssz_assert((index)>=0&&(index)<ssz_array_size(arr)),arr[(index)])
#define ssz_array_set(arr, index, value) (ssz_assert((index)>=0&&(index)<ssz_array_size(arr)),arr[(index)]=value)
#define ssz_array_font(arr) arr[0]
#define ssz_array_back(arr) arr[ssz_array_size(arr)-1]

#ifdef __cplusplus
}
#endif
