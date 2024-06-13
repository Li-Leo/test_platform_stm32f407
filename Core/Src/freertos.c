/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "log.h"
#include "shell.h"
#include "shell_port.h"
#include "key.h"
#include "drv_gpio.h"
#include "flash.h"
#include "thread.h"
#include "rtc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#undef LOG_ENABLE
#ifndef     LOG_ENABLE
    #define LOG_ENABLE         1                /**< 使能log */
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osTimerId_t timer_id;
uint32_t g_click_interval_s = 10;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void timer_callback(void *parameter);
void key2_pressed(KeyID key, int repeat_count);
void key3_pressed(KeyID key, int repeat_count);
void key4_released(KeyID key, int repeat_count);
int flash_db_init(void);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  key_init();
  sfud_init();
  flash_db_init();
  userShellInit();
  drv_gpio_set_pin_high(LED2_GPIO_Port, LED2_Pin);
  drv_gpio_set_pin_high(LED1_GPIO_Port, LED1_Pin);
  drv_gpio_set_pin_high(LED0_GPIO_Port, LED0_Pin);
  key_set_handler(kKey2, kKeyEventPressed, key2_pressed);
  key_set_handler(kKey3, kKeyEventPressed, key3_pressed);
  key_set_handler(kKey4, kKeyEventReleased, key4_released);

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  osTimerId_t key_scan_timer_id = osTimerNew(key_start_scan, osTimerPeriodic, NULL, NULL);
  osTimerStart(key_scan_timer_id, KEY_SCAN_TIME_MS);

  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  start_thread();
  start_sem_thread();
  start_event_thread();
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  osTimerId_t timer_id = osTimerNew(timer_callback, osTimerOnce, NULL, NULL);
  /* Infinite loop */
  for(;;)
  {
    osDelay(g_click_interval_s * 1000ul);
    // drv_gpio_set_pin_low(LED0_GPIO_Port, LED0_Pin);
    drv_gpio_set_pin_high(PB_11_GPIO_Port, PB_11_Pin);
    osTimerStart(timer_id, 50);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void timer_callback(void *argument)
{
    // drv_gpio_set_pin_high(LED0_GPIO_Port, LED0_Pin);
    drv_gpio_set_pin_low(PB_11_GPIO_Port, PB_11_Pin);
}

void key2_pressed(KeyID key, int repeat_count)
{
    drv_gpio_toggle_pin(LED1_GPIO_Port, LED1_Pin);
    osThreadFlagsSet(thread_handle, KEY2_FLAG);
    osEventFlagsSet(event_thread_handle, KEY2_FLAG);
    osSemaphoreRelease(sem);
}

void key3_pressed(KeyID key, int repeat_count)
{
    drv_gpio_toggle_pin(LED2_GPIO_Port, LED2_Pin);
    osThreadFlagsSet(thread_handle, KEY3_FLAG);
    osEventFlagsSet(event_thread_handle, KEY3_FLAG);
    osSemaphoreRelease(sem);
}

void key4_released(KeyID key, int repeat_count)
{
    drv_gpio_toggle_pin(LED2_GPIO_Port, LED2_Pin);
    osThreadFlagsSet(thread_handle, KEY4_FLAG);
    osEventFlagsSet(event_thread_handle, KEY4_FLAG);
}


void time(void)
{
  if (HAL_RTC_GetTime(&hrtc, &ctime, RTC_FORMAT_BIN) == HAL_OK) {
    logDebug("time: %02d:%02d:%02d", ctime.Hours, ctime.Minutes, ctime.Seconds);
  }

  if (HAL_RTC_GetDate(&hrtc, &date_buff, RTC_FORMAT_BIN) == HAL_OK) {
    logDebug("date: %02d/%02d/%02d/%02d", 2000 + date_buff.Year, date_buff.Month, date_buff.Date,date_buff.WeekDay);
  }
}

void print_tick(void)
{
  // logDebug("tick = %d", HAL_GetTick());
  logDebug("tick = %dms, %d:%d:%d:%d", HAL_GetTick(), HAL_GetTick() / 1000 / 3600, 
    HAL_GetTick() / 1000 % 3600 / 60, HAL_GetTick() / 1000 % 60, HAL_GetTick() % 1000);
  time();
}


void set_time(uint8_t hour, uint8_t minute, uint8_t second)
{
  RTC_TimeTypeDef sTime = {0};
  
  sTime.Hours = hour;
  sTime.Minutes = minute;
  sTime.Seconds = second;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  logDebug("set time done!");
}

void set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week_day)
{
  RTC_DateTypeDef sDate = {0};
  
  sDate.WeekDay = week_day;
  sDate.Month = month;
  sDate.Date = date;
  sDate.Year = year;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  logDebug("set date done!");
}

void set_datetime(uint8_t year, uint8_t month, uint8_t date, uint8_t week_day, uint8_t hour, uint8_t minute, uint8_t second)
{
  set_time(hour, minute, second);
  set_date(year, month, date, week_day);
  logDebug("datetime set done!");
}

void write_rtc_backup_year(uint8_t year)
{
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, year);
}

void kernel_ver(void)
{
    char kernel_id[40];
    
    osKernelGetInfo(NULL, kernel_id, 40);
    logDebug("kernel_ver = %s", kernel_id);
}

void click_interval_set(uint32_t second)
{
  g_click_interval_s = second;
  logDebug("set g_click_interval_s=%lu", g_click_interval_s);
}

void click_interval_read(void)
{
  logDebug("g_click_interval_s=%lu", g_click_interval_s);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), tick, print_tick, print current tick);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), time, time, datetime);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_time, set_time, set time [h] [m] [s]);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_date, set_date, set date [y] [m] [d] [w]);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_datetime, set_datetime, set date [y] [m] [d] [w] [h] [m] [s]);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), write_rtc_backup_year, write_rtc_backup_year, write_rtc_backup_year);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), click_interval_set, click_interval_set, click_interval_set);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), click_interval_read, click_interval_read, click_interval_read);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kernel_ver, kernel_ver, kernel_ver);

/* USER CODE END Application */

