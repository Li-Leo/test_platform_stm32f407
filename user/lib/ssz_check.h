#pragma once

#include "string.h"
#include "assert.h"
#include "stdio.h"

/************************************************
* Declaration
************************************************/
	
//trace message, valid ony when debug
#ifdef DEBUG

   //simple trace
   #define ssz_trace printf  
   //trace with current time
   #define ssz_tracet printf("%s: ", ssz_now_time_str_for_print());printf
   //trace with current time and auto add line break
   #define ssz_traceln(...) printf("%s: ", ssz_now_time_str_for_print());printf(__VA_ARGS__);printf("\n");
	
#else

   #define ssz_trace(...)
   #define ssz_tracet(...)
   #define ssz_traceln(...)
   #define ssz_assert_fail() ((void)0)
   #define ssz_assert(expression) ((void)0)
		
#endif //_DEBUG


#define ssz_printft(is_module_enable_output, ...) //if(is_module_enable_output){printf("%s: ", ssz_now_time_str_for_print());printf(__VA_ARGS__);}
//output with current time and auto add line break
#define ssz_printfln(is_module_enable_output, ...) //if(is_module_enable_output){printf("%s: ", ssz_now_time_str_for_print());printf(__VA_ARGS__);printf("\n");}
#define ssz_printfln_ex(is_module_enable_output, extra_str, ...) //if(is_module_enable_output){printf("%s:[%s]: ", ssz_now_time_str_for_print(), extra_str);printf(__VA_ARGS__);printf("\n");}

//assert if it is true, valid ony when debug
#ifdef _lint
   void exit(int);
   #define ssz_assert assert
   #define ssz_assert_fail() exit(-1)
#elif defined DEBUG
   #define ssz_assert(expression) (void)(                                                       \
               (!!(expression)) ||                                                              \
               (ssz_on_assert_fail(__FILE__, __LINE__, #expression), 0) \
         )
   #define ssz_assert_fail() ssz_on_assert_fail(__FILE__, __LINE__, "")
   //it's difference from ssz_assert is that, it will run at release mode
   #define ssz_check(expression) if (!(expression))ssz_on_check_fail(__FILE__, __LINE__, #expression) 
#else
   #define ssz_assert(expression) ((void)0)
   #define ssz_assert_fail() ((void)0)
   //it's difference from ssz_assert is that, it will run at release mode
   #define ssz_check(expression) if (!(expression))ssz_on_check_fail() 
#endif
