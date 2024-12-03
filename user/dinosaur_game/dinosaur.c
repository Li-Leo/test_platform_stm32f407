
#include "oled12864.h"
#include "img_data.h"

int get_key_val()
{
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == 0)
    {
        return 2;
    }
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == 0)
    {
        return 1;
    }
    return 0;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
// int main(void)
void dino(void *argument)
{
    /* USER CODE BEGIN 1 */
    unsigned char key_num = 0;
    unsigned char cactus_category = 0;
    unsigned char cactus_length = 8;
    unsigned int score = 0;
    unsigned int highest_score = 0;
    int height = 0;
    int cactus_pos = 128;
    unsigned char cur_speed = 30;
    char failed = 0;
    char reset = 0;

    // oled_init();
    oled_show_string(0, 0, "Test", 4);
    // while(1);
    OLED_DrawCover();

    // while (get_key_val() != 2)
    //     ;
    // HAL_Delay(100);
    oled_clear();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        if (failed == 1)
        {
            // OLED_DrawRestart();

            key_num = get_key_val();
            if (key_num == 2)
            {
                if (score > highest_score)
                    highest_score = score;
                score = 0;
                failed = 0;
                height = 0;
                reset = 1;
                OLED_DrawDinoJump(reset);
                OLED_DrawCactusRandom(cactus_category, reset);
                oled_clear();
            }
            osDelay(80);
            continue;
        }

        score++;
        if (height <= 0)
            key_num = get_key_val();

        OLED_DrawGround();
        OLED_DrawCloud();

        if (height > 0 || key_num == 1)
            height = OLED_DrawDinoJump(reset);
        else
            OLED_DrawDino();

        cactus_pos = OLED_DrawCactusRandom(cactus_category, reset);
        if (cactus_category == 0)
            cactus_length = 8;
        else if (cactus_category == 1)
            cactus_length = 16;
        else
            cactus_length = 24;

        if (cactus_pos + cactus_length < 0)
        {
            cactus_category = rand() % 4;
            OLED_DrawCactusRandom(cactus_category, 1);
        }

        if ((height < 16) && ((cactus_pos >= 16 && cactus_pos <= 32) || (cactus_pos + cactus_length >= 16 && cactus_pos + cactus_length <= 32)))
        {
            failed = 1;
        }

        oled_show_string(35, 0, "HI:", 12);
        oled_show_num(58, 0, highest_score, 5, 12);
        oled_show_num(98, 0, score, 5, 12);

        reset = 0;

        cur_speed = score / 20;
        if (cur_speed > 29)
            cur_speed = 29;

        // HAL_Delay(30 - cur_speed);
        osDelay(40);

        //		HAL_Delay(500);
        key_num = 0;
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

#include "log.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

osThreadId_t dinosaur_handle = NULL;
const osThreadAttr_t dinosaur_attributes = {
    .name = "dino",
    .stack_size = 128 * 8,
    .priority = (osPriority_t) osPriorityHigh,
};

void dino_start(void)
{
    if (dinosaur_handle == NULL) {
        dinosaur_handle = osThreadNew(dino, NULL, &dinosaur_attributes);
    } else {
        osThreadResume(dinosaur_handle);
    }
}

void dino_stop(void)
{
    // dinosaur_handle = osThreadNew(dino, NULL, &dinosaur_attributes);
    osThreadSuspend(dinosaur_handle);
    logDebug("dinosaur_handle state: %d", osThreadGetState(dinosaur_handle));
}

void dino_state(void)
{

    logDebug("dinosaur_handle state: %d", osThreadGetState(dinosaur_handle));
}


osThreadId_t play_handle = NULL;
const osThreadAttr_t play_attributes = {
    .name = "play",
    .stack_size = 128 * 8,
    .priority = (osPriority_t) osPriorityHigh,
};

uint8_t g_frame_rate = 25;

void play(void *argument)
{
    uint32_t i = 0;

    while (1)
    {
        
        OLED_DrawBMPFast(&img[i]);

        if (i  + 128 * 8 < FRAME_SIZE * 128 * 8) {
            i += 128 * 8;
        } else {
            i = 0;
        }

        osDelay(1000 / g_frame_rate);

    }
    /* USER CODE END 3 */
}

void play_start(void)
{
    if (play_handle == NULL) {
        play_handle = osThreadNew(play, NULL, &play_attributes);
    } else {
        osThreadResume(play_handle);
    }

}

void play_stop(void)
{
    osThreadSuspend(play_handle);
    logDebug("play_handle state: %d", osThreadGetState(play_handle));
}

void frame_rate(uint8_t rate)
{
    g_frame_rate = rate;
    logDebug("frame rate: %d ", g_frame_rate);
}



SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), dino_start, dino_start, dino_start);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), dino_stop, dino_stop, dino_stop);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), dino_state, dino_state, );
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), play_start, play_start, );
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), play_stop, play_stop, );

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), frame_rate, frame_rate, );



