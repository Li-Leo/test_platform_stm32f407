/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: DemoProc.c                                                **/
/** Author: Polarix                                                     **/
/** Description: User operation interface.                              **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "screen_proc.h"
#include "SGUI_Typedef.h"
#include "SGUI_Basic.h"

#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
#include "SDKInterface.h"
#include "SGUI_FontResource.h"
#else
#include "oled_1306.h"
// #include "usart.h"
// #include "rtc.h"
// #include "base_timer.h"
#include "DemoActions.h"
#include "trigger_flags.h"
#include "screen.h"
// #include "keyboard.h"
#endif

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
SGUI_SCR_DEV                g_stDeviceInterface;
HMI_SCREEN_OBJECT*          g_arrpstScreenObjs[] =
                            {
                                &g_stHMIDemo_List,
                                &g_stHMIDemo_TextPaint,
                                &g_stHMIDemo_VariableBox,
                                &g_stHMIDemo_RealtimeGraph,
                                &g_stHMIDemo_Menu,
                                &g_stHMIDemo_Notice,
                                &g_stHMIDemo_BasicPaint,
                                &g_stHMI_extended_List,
                                &g_stHMI_settings_List,
                            };
HMI_ENGINE_OBJECT           g_stDemoEngine;

#ifndef _SIMPLE_GUI_IN_VIRTUAL_SDK_
static unsigned int         s_uiKeyValue;
#endif

SGUI_BYTE                   s_pBmpDataBuffer[128 *8];

//=======================================================================//
//= Static function declare.                                            =//
//=======================================================================//
static void                 KeyPressEventProc(void);
static void                 RTCEventProc(void);
static void                 SysTickTimerEventProc(void);
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
static bool                 CheckEventFlag(ENV_FLAG_INDEX eIndex);
#endif // _SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//

/*****************************************************************************/
/** Function Name:  InitializeHMIEngineObj                                  **/
/** Purpose:        Simple GUI HMI engine and interface demo process.       **/
/** Parameters:     None.                                                   **/
/** Return:         HMI_ENGINE_RESULT.                                      **/
/** Notice:         This function demonstrates how to use the interface and **/
/**                 HMI engine of Simple GUI.                               **/
/*****************************************************************************/
HMI_ENGINE_RESULT InitializeHMIEngineObj(void)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    int                         iIndex;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    /* Clear structure. */
    SGUI_SystemIF_MemorySet(&g_stDeviceInterface, 0x00, sizeof(SGUI_SCR_DEV));
    SGUI_SystemIF_MemorySet(&g_stDemoEngine, 0x00, sizeof(HMI_ENGINE_OBJECT));
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    /* Initialize display size. */
    g_stDeviceInterface.stSize.iWidth = 128;
    g_stDeviceInterface.stSize.iHeight = 64;
    /* Initialize interface object. */
    g_stDeviceInterface.fnSetPixel = SGUI_SDK_SetPixel;
    g_stDeviceInterface.fnGetPixel = SGUI_SDK_GetPixel;
    g_stDeviceInterface.fnClear = SGUI_SDK_ClearDisplay;
    g_stDeviceInterface.fnSyncBuffer = SGUI_SDK_RefreshDisplay;
#else
    /* Initialize display size. */
    g_stDeviceInterface.stSize.iWidth = 128;
    g_stDeviceInterface.stSize.iHeight = 64;
    g_stDeviceInterface.stBuffer.pBuffer = s_pBmpDataBuffer;
    g_stDeviceInterface.stBuffer.sSize = 128*8;
    /* Initialize interface object. */
    g_stDeviceInterface.fnSetPixel = OLED_DrawPoint_ex;
    // g_stDeviceInterface.fnGetPixel = OLED_GetPoint;
    g_stDeviceInterface.fnClear = OLED_Clear;
    g_stDeviceInterface.fnSyncBuffer = OLED_Update;
#endif
    do
    {
        /* Prepare HMI engine object. */
        g_stDemoEngine.ScreenCount = sizeof(g_arrpstScreenObjs)/sizeof(*g_arrpstScreenObjs);
        g_stDemoEngine.ScreenObjPtr = g_arrpstScreenObjs;
        g_stDemoEngine.Interface = &g_stDeviceInterface;

        /* Initialize all screen object. */
        if(NULL != g_stDemoEngine.ScreenObjPtr)
        {
            for(iIndex=0; iIndex<g_stDemoEngine.ScreenCount; iIndex++)
            {
                if( (NULL != g_stDemoEngine.ScreenObjPtr[iIndex])
                    && (NULL != g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions)
                    && (NULL != g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions->Initialize)
                    )
                {
                    g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions->Initialize(&g_stDeviceInterface);
                    g_stDemoEngine.ScreenObjPtr[iIndex]->pstPrevious = NULL;
                }
            }
        }
        else
        {

        }
        /* Active engine object. */
        eProcessResult = HMI_ActiveEngine(&g_stDemoEngine, HMI_SCREEN_ID_DEMO_LIST);
        //eProcessResult = HMI_ActiveEngine(&g_stDemoEngine, HMI_SCREEN_ID_DEMO_ITEMS_BASE);
        if(HMI_PROCESS_FAILED(eProcessResult))
        {
            /* Active engine failed. */
            break;
        }
        /* Start engine process. */
        eProcessResult = HMI_StartEngine(NULL);
        if(HMI_PROCESS_FAILED(eProcessResult))
        {
            /* Start engine failed. */
            break;
        }
    }while(0);

    return eProcessResult;
}

#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
/*****************************************************************************/
/** Function Name:  CheckEventFlag                                          **/
/** Purpose:        Check SimpleGUI virtual SDK event trigger flag and      **/
/**                 reset for next trigger and check.                       **/
/** Parameters:                                                             **/
/** @ eIndex[in]:   Checked flag index.                                     **/
/** Return:         true for event is trigger, false for not.               **/
/** Notice:         This function only used in SimpleGUI virtual SDK        **/
/*****************************************************************************/
bool CheckEventFlag(ENV_FLAG_INDEX eIndex)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    bool                    bReturn;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    bReturn = SGUI_SDK_GetEventSyncFlag(eIndex);

    if(true == bReturn)
    {
        SGUI_SDK_SetEvnetSyncFlag(eIndex, false);
    }

    return bReturn;
}
#endif // _SIMPLE_GUI_VIRTUAL_ENVIRONMENT_SIMULATOR_

/*****************************************************************************/
/** Function Name:  DemoMainProcess                                         **/
/** Purpose:        It is a dummy main function for SimpleGUI Virtual SDK,  **/
/**                 or run demo process in demo process.                    **/
/** Parameters:     None.                                                   **/
/** Return:         Terminal code, seam as main function return code.       **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
void DemoMainProcess(void)
{
    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    // Initialize HMI Engine.
    InitializeHMIEngineObj();

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    while(1)
    {
        // Check and process heart-beat timer event.
        // if(true == SysTickTimerTriggered())
        // {
        //     SysTickTimerEventProc();
        // }
        // Check and process key press event.
        if(true == UserEventTriggered())
        {
            KeyPressEventProc();
        }

        osDelay(50);
        // Check and process RTC event.

        // if(true == RTCTimerTriggered())
        // {
        //     RTCEventProc();
        // }
    }
}

/*****************************************************************************/
/** Function Name:  KeyPressEventProc                                       **/
/** Purpose:        Deal with user key process in demo process.             **/
/** Parameters:     None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         The button is just abstract, and the message may come   **/
/**                 from a touch screen, a serial port, and so on.          **/
/*****************************************************************************/
#if 1
void KeyPressEventProc(void)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    KEY_PRESS_EVENT     stEvent;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    HMI_EVENT_INIT(stEvent);

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // stEvent.Head.iType = EVENT_TYPE_ACTION;
    stEvent.Head.iID = EVENT_ID_KEY_PRESS;
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    stEvent.Data.uiKeyValue = SGUI_SDK_GetKeyEventData();
#else
    stEvent.Data.uiKeyValue = s_uiKeyValue;
#endif
    // Post key press event.
    HMI_ProcessEvent((HMI_EVENT_BASE*)(&stEvent));
}

#endif

/*****************************************************************************/
/** Function Name:  SysTickTimerEventProc                                   **/
/** Purpose:        Deal with heartbeat timer event in demo process.        **/
/** Parameters:     None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         It is usually used to simulate ADC or other sensors.    **/
/*****************************************************************************/
void SysTickTimerEventProc(void)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    DATA_EVENT              stEvent;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    HMI_EVENT_INIT(stEvent);

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // stEvent.Head.iType =    EVENT_TYPE_DATA;
    stEvent.Head.iID =      EVENT_ID_TIMER;
    stEvent.Data.iValue = (rand() % 200)-100;
    // Post timer event.
    HMI_ProcessEvent((HMI_EVENT_BASE*)&stEvent);
}

/*****************************************************************************/
/** Function Name:  RTCEventProc                                            **/
/** Purpose:        Deal with RTC timer event in demo process.              **/
/** Parameters:     None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         It is usually used to simulate ADC or other sensors.    **/
/*****************************************************************************/
#if 0
void RTCEventProc(void)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_GENERAL_EVENT       stEvent;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    HMI_EVENT_INIT(stEvent);

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    stEvent.Head.iType =    EVENT_TYPE_DATA;
    stEvent.Head.iID =      EVENT_ID_RTC;
    // Post RTC update message to a screen.
    HMI_ProcessEvent((HMI_EVENT_BASE*)&stEvent);
}

#endif

/*****************************************************************************/
/** Function Name:  SysTickTimerTriggered                                   **/
/** Purpose:        Check sys-tick timer interrupt is triggered.            **/
/** Parameters:     None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
bool SysTickTimerTriggered(void)
{
#ifndef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    bool                bIsTriggered;
#endif
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    return CheckEventFlag(ENV_FLAG_IDX_SDK_TIM_EVENT);
#else
    // Dummy sys-tick Timer interrupt triggered process.
    bIsTriggered = BaseTimerIsTrigger();
    if(true == bIsTriggered)
    {
        BaseTimerTriggerReset();
    }

    return bIsTriggered;
#endif
}

/*****************************************************************************/
/** Function Name:  RTCTimerTriggered                                       **/
/** Purpose:        Check RTC timer interrupt is triggered.                 **/
/** Parameters:     None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
#if 0
bool RTCTimerTriggered(void)
{
#ifndef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    bool                bIsTriggered;
#endif
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    return CheckEventFlag(ENV_FLAG_IDX_SDK_RTC_EVENT);
#else
    // RTC interrupt triggered process.
    bIsTriggered = RTCTimerIsTrigger();
    if(true == bIsTriggered)
    {
        RTCTimerTriggerReset();
    }

    return bIsTriggered;
#endif
}

#endif

/*****************************************************************************/
/** Function Name:  UserEventTriggered                                      **/
/** Purpose:        Check user event is triggered.                          **/
/** Parameters:     None.                                                   **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
bool UserEventTriggered(void)
{
#ifndef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    bool                bIsTriggered;

#endif
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    return CheckEventFlag(ENV_FLAG_IDX_SDK_KEY_EVENT);
#else
    // User event triggered process.
    bIsTriggered = UsartIsReceived();
    if(true == bIsTriggered)
    {
        s_uiKeyValue = GetReceivedCode();
        UsartTriggerReset();
    }
    return bIsTriggered;
#endif
}

/*****************************************************************************/
/** Function Name:  SysTickTimerEnable                                      **/
/** Purpose:        Enable or disable sys-tick timer.                       **/
/** Parameters:                                                             **/
/** @ bEnable[in]:  True for enable sys-tick timer, false for disable.      **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/
void SysTickTimerEnable(bool bEnable)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    (void)SGUI_SDK_ConfigHearBeatTimer(bEnable?DEMO_HEART_BEAT_INTERVAL_MS:0);
#else
    // Add Dummy sys-tick timer interrupt enable change operation here.
    // BASE_TIMER_ActiveInterrupt(TIM3, bEnable);
#endif
}

/*****************************************************************************/
/** Function Name:  RTCTimerEnable                                          **/
/** Purpose:        Enable or disable RTC timer.                            **/
/** Parameters:                                                             **/
/** @ bEnable[in]:  True for enable RTC timer, false for disable.           **/
/** Return:         None.                                                   **/
/** Notice:         None.                                                   **/
/*****************************************************************************/

#if 0
void RTCTimerEnable(bool bEnable)
{

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
    (void)SGUI_SDK_EnableRTCInterrupt(bEnable);
#else
    // Add RTC interrupt enable change operation here.
    RTC_WaitForSynchro();
    RTC_ITConfig(RTC_IT_SEC, bEnable?ENABLE:DISABLE);
    RTC_WaitForLastTask();
#endif
}

#endif


#include "log.h"

void key_value(uint16_t key)
{
    USARTReceiveProc(USART_INT_REASON_REV, key);
}

#include "cmsis_os.h"

osThreadId_t ui_handle = NULL;
const osThreadAttr_t ui_attributes = {
    .name = "ui",
    .stack_size = 128 * 24,
    .priority = (osPriority_t) osPriorityHigh1,
};

void demo_ui(void *argument)
{
    DemoMainProcess();
}

void update(void *argument)
{
    OLED_Update();
}

void demo_ui_thread(void)
{
    ui_handle = osThreadNew(demo_ui, NULL, &ui_attributes);
}


#include "font_data.h"
#include <stdio.h>
#include <stdint.h>

void print_unicode(const char *utf8_string)
{
    const unsigned char *p = (const unsigned char *)utf8_string;

    while (*p) {
        uint32_t codepoint = 0;
        int bytes = 0;

        // Determine the number of bytes for the UTF-8 character
        if ((*p & 0x80) == 0) {
            // 1-byte character (ASCII)
            codepoint = *p;
            bytes = 1;
        } else if ((*p & 0xE0) == 0xC0) {
            // 2-byte character
            codepoint = *p & 0x1F;
            bytes = 2;
        } else if ((*p & 0xF0) == 0xE0) {
            // 3-byte character
            codepoint = *p & 0x0F;
            bytes = 3;
        } else if ((*p & 0xF8) == 0xF0) {
            // 4-byte character
            codepoint = *p & 0x07;
            bytes = 4;
        } else {
            // Invalid UTF-8 byte
            printf("Invalid UTF-8 sequence\n");
            return;
        }

        // Read the remaining bytes
        for (int i = 1; i < bytes; ++i) {
            p++;
            if ((*p & 0xC0) != 0x80) {
                printf("Invalid UTF-8 sequence\n");
                return;
            }
            codepoint = (codepoint << 6) | (*p & 0x3F);
        }

        // Print the Unicode code point
        printf("U+%04X\n", codepoint);
        p++;
    }
}

void draw_text()
{
    SGUI_RECT stDisplayArea = {0, 0, 128, 64};
    SGUI_POINT stInnerPos = {0,0};

    g_stDeviceInterface.stSize.iWidth = 128;
    g_stDeviceInterface.stSize.iHeight = 64;
    g_stDeviceInterface.stBuffer.pBuffer = s_pBmpDataBuffer;
    g_stDeviceInterface.stBuffer.sSize = 128;
    /* Initialize interface object. */
    g_stDeviceInterface.fnSetPixel = OLED_DrawPoint_ex;
    // g_stDeviceInterface.fnGetPixel = OLED_GetPoint;
    g_stDeviceInterface.fnClear = OLED_Clear;
    g_stDeviceInterface.fnSyncBuffer = OLED_Update;


    stDisplayArea.iX = 0;
    stDisplayArea.iY = 0;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = 18;
    set_display_font(LXGW_REGULAR_18);
    const char s[] = "\x61\x62\xe4\xbd\x8e\xe9\x98\x88\xe5\x80\xbc\x0a";
    // SGUI_Text_DrawText(&g_stDeviceInterface, s, &font_lxgw_regular_18, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);


    stDisplayArea.iX = 0;
    stDisplayArea.iY = 18;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = 20;
    set_display_font(DENGXIAN_BOLD_20);
    const char s1[] = "\x31\x32\x33\x34\x35\x36\x37\x38\x39\x30\x0a";
    // SGUI_Text_DrawText(&g_stDeviceInterface, s1, &font_dengxian_bold_20, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);

    // stDisplayArea.iX = 0;
    // stDisplayArea.iY = 38;
    // stDisplayArea.iWidth = 128;
    // stDisplayArea.iHeight = 16;
    // set_display_font(DENGXIAN_REGULAR_16);
    // const char s2[] = "16\xe5\x93\x88\xe5\x93\x88\xe7\xad\x89\xe7\xba\xbf";
    // SGUI_Text_DrawText(&g_stDeviceInterface, s2, &font_dengxian_regular_16, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);


    stDisplayArea.iX = 0;
    stDisplayArea.iY = 38;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = 16;
    set_display_font(DENGXIAN_REGULAR_22);
    const char s3[] = "\xe9\xa4\x90\xe5\x89\x8d\xe8\xae\xbe\xe7\xbd\xae\x0a";
    // SGUI_Text_DrawText(&g_stDeviceInterface, s3, &font_dengxian_regular_22, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);

    stDisplayArea.iX = 60;
    stDisplayArea.iY = 48;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = 16;
    set_display_font(INCONSOLATA_REGULAR_16);
    const char s4[] = "\xe8\x8d\xaf\xe9\x87\x8f\xe9\x98\x88\xe5\x80\xbc\x0a";
    // SGUI_Text_DrawText(&g_stDeviceInterface, s4, &font_inconsolata_regular_16, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);


    stDisplayArea.iX = 0;
    stDisplayArea.iY = 48;
    stDisplayArea.iWidth = 128;
    stDisplayArea.iHeight = 12;
    set_display_font(INCONSOLATA_REGULAR_12);
    SGUI_Text_DrawText(&g_stDeviceInterface, "Realtime graph", &font_inconsolata_regular_12, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);

    OLED_Update();
}



void chinese()
{
    SGUI_RECT stDisplayArea1 = {0, 0, 100, 64};
    SGUI_POINT stInnerPos1 = {2,2};


    g_stDeviceInterface.stSize.iWidth = 128;
    g_stDeviceInterface.stSize.iHeight = 64;
    g_stDeviceInterface.stBuffer.pBuffer = s_pBmpDataBuffer;
    g_stDeviceInterface.stBuffer.sSize = 128;
    /* Initialize interface object. */
    g_stDeviceInterface.fnSetPixel = OLED_DrawPoint_ex;
    // g_stDeviceInterface.fnGetPixel = OLED_GetPoint;
    g_stDeviceInterface.fnClear = OLED_Clear;
    g_stDeviceInterface.fnSyncBuffer = OLED_Update;

    // const char s[] = "\xe4\xb8\xad\xe5\x9b\xbd\xe5\xa4\xa7\xe6\x88\x91\xe6\x98\xaf\xe6\x9c\x89\xe8\xb7\x83\xe8\xbf\x9b";
    const char s[] = "\xe4\xb8\xad\x36\xe5\x9b\xbd\xe5\xa4\xa7\xe6\x88\x91\xe6\x98\xaf\xe6\x9c\x89\xe8\xb7\x83\x56\xe8\xbf\x9b";
    // SGUI_Text_DrawText(&g_stDeviceInterface, s, &font_15, &stDisplayArea1, &stInnerPos1, SGUI_DRAW_NORMAL);

    OLED_Update();
}

void oledclear()
{
    
    OLED_Clear();
}

#include "image_data.h"

extern const unsigned char font_56fd[];

void draw_pic()
{
    SGUI_RECT stDisplayArea = {0, 0, 128, 64};
    SGUI_POINT stInnerPos = {0,0};
    SGUI_BMP_RES bmp;
    // char a = '\x8fdb';


    g_stDeviceInterface.stSize.iWidth = 128;
    g_stDeviceInterface.stSize.iHeight = 64;
    g_stDeviceInterface.stBuffer.pBuffer = s_pBmpDataBuffer;
    g_stDeviceInterface.stBuffer.sSize = 128*8;
    /* Initialize interface object. */
    g_stDeviceInterface.fnSetPixel = OLED_DrawPoint_ex;
    // g_stDeviceInterface.fnGetPixel = OLED_GetPoint;
    g_stDeviceInterface.fnClear = OLED_Clear;
    g_stDeviceInterface.fnSyncBuffer = OLED_Update;

    OLED_Clear();

    bmp.iWidth = get_image(E_IMG_PAUSE)->width;
    bmp.iHeight = get_image(E_IMG_PAUSE)->height;
    bmp.pData = get_image(E_IMG_PAUSE)->data;
    
    SGUI_Basic_DrawBitMap(&g_stDeviceInterface, &stDisplayArea, &stInnerPos, &bmp, SGUI_DRAW_NORMAL);

    stInnerPos.iX = 20;
    stInnerPos.iY = 0;
    bmp.iWidth = get_image(E_IMG_REFILL)->width;
    bmp.iHeight = get_image(E_IMG_REFILL)->height;
    bmp.pData = get_image(E_IMG_REFILL)->data;
    SGUI_Basic_DrawBitMap(&g_stDeviceInterface, &stDisplayArea, &stInnerPos, &bmp, SGUI_DRAW_NORMAL);


    stInnerPos.iX = 0;
    stInnerPos.iY = 36;
    bmp.iWidth = get_image(E_IMG_REPLACE_PULL)->width;
    bmp.iHeight = get_image(E_IMG_REPLACE_PULL)->height;
    bmp.pData = get_image(E_IMG_REPLACE_PULL)->data;
    SGUI_Basic_DrawBitMap(&g_stDeviceInterface, &stDisplayArea, &stInnerPos, &bmp, SGUI_DRAW_NORMAL);


    stInnerPos.iX = 60;
    stInnerPos.iY = 40;
    bmp.iWidth = get_image(E_IMG_REPLACE_PUSH)->width;
    bmp.iHeight = get_image(E_IMG_REPLACE_PUSH)->height;
    bmp.pData = get_image(E_IMG_REPLACE_PUSH)->data;
    SGUI_Basic_DrawBitMap(&g_stDeviceInterface, &stDisplayArea, &stInnerPos, &bmp, SGUI_DRAW_NORMAL);


    OLED_Update();
}

void draw_ch(void)
{
    SGUI_RECT stDisplayArea = {5, 10, 128, 64};
    SGUI_POINT stInnerPos = {0,0};
    
    const char s1[] = "123\xe4\xb8\xad\x36\xe5\x9b\xbd\xe5\xa4\xa7\xe6\x88\x91\xe6\x98\xaf\xe6\x9c\x89\xe8\xb7\x83\x56\xe8\xbf\x9b";

    // SGUI_Text_DrawText(&g_stDeviceInterface, s1, &font_15, &stDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);

    OLED_Update();
}


SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), kb, key_value, key_value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), update, update, key_value);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), chinese, chinese, );
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), oled_clear, oledclear, );

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), draw_text, draw_text, key_value);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), draw_pic, draw_pic, key_value);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), draw_ch, draw_ch, key_value);




