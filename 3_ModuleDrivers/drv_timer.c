#include "drivers.h"
#include "./drv_config/timer_config.h"
#include "tim.h"
#include "libs.h"
#include "errno.h"
#include "stm32f4xx_hal.h"

static int TimerDrvInit(struct TimerDev *ptdev);
static int TimerDrvStart(struct TimerDev *ptdev);
static int TimerDrvStop(struct TimerDev *ptdev);
static int TimerDrvGetCount(struct TimerDev *ptdev);
static int TimerDrvControl(struct TimerDev *ptdev, unsigned int cmd, unsigned int cfg);

static volatile unsigned int gTIM2Count = 0;
static volatile unsigned int gTIM10Count = 0;

static void (*TimerIrqHandler[15])(void *arg);

static TimerDevice gTimerDevices[] = {
#if USE_SYSTICK_TIMER
    SYSTICKDEV,
#endif
#if USE_GENERAL_TIMER
    TIM2DEV, TIM4DEV, TIM10DEV, 
#endif
};

void TimerDevicesCreate(void)
{
    unsigned int num = sizeof(gTimerDevices) / sizeof(struct TimerDev);
    for(unsigned int i = 0; i<num; i++)
        TimerDeviceInsert(&gTimerDevices[i]);
}

static int TimerDrvInit(struct TimerDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    if(ptdev->port == SysTick)
    {
        ptdev->status = 1;  // Start status
    }
    else if(ptdev->port == TIM2)
    {
        int ret = TimerDrvControl(ptdev, SET_PERIOD_VALUE, 1000);
        if(ESUCCESS != ret) return -EIO;
        TimerIrqHandler[2] = ptdev->IrqHandler;
    }
    else if(ptdev->port == TIM4)
    {
        TimerIrqHandler[4] = ptdev->IrqHandler;
    }
    else if(ptdev->port == TIM10)
    {
        TimerIrqHandler[10] = ptdev->IrqHandler;
    }
    
    return ESUCCESS;
}

static int TimerDrvStart(struct TimerDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(ptdev->status == 1)  return ESUCCESS;
    
    if(ptdev->port == SysTick)
    {
        HAL_ResumeTick();
        ptdev->status = 1;  // Start status
    }
    else
    {
        TIM_HandleTypeDef *htim = NULL;
        if(ptdev->port == TIM2)
        htim = &htim2;
        else if(ptdev->port == TIM4)
            htim = &htim4;
        else if(ptdev->port == TIM10)
            htim = &htim10;
        
        if(NULL == htim)    return -EIO;
        HAL_StatusTypeDef status = HAL_TIM_Base_Start_IT(htim);
        if(HAL_OK != status)    return -EIO;
        ptdev->status = 1;  // Start status
    }

    return ESUCCESS;
}

static int TimerDrvStop(struct TimerDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(ptdev->status == 0)  return ESUCCESS;
    
    if(ptdev->port == SysTick)
    {
        HAL_SuspendTick();
        ptdev->status = 0;  // Stop status
    }
    else
    {
        TIM_HandleTypeDef *htim = NULL;
        if(ptdev->port == TIM2)
        htim = &htim2;
        else if(ptdev->port == TIM4)
            htim = &htim4;
        else if(ptdev->port == TIM10)
            htim = &htim10;
        
        if(NULL == htim)    return -EIO;
        HAL_StatusTypeDef status = HAL_TIM_Base_Stop_IT(htim);
        if(HAL_OK != status)    return -EIO;
        ptdev->status = 0;  // Start status
    }
    
    return ESUCCESS;
}

static int TimerDrvGetCount(struct TimerDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    int count = 0;
    if(ptdev->port == SysTick)
    {
        count = HAL_GetTick();
    }
    else if(ptdev->port == TIM2)
    {
        count = gTIM2Count;
    }
    else if(ptdev->port == TIM4)
    {
    }
    else if(ptdev->port == TIM10)
    {
        count = gTIM10Count;
    }

    return count;
}

static int TimerDrvControl(struct TimerDev *ptdev, unsigned int cmd, unsigned int cfg)
{
    if(NULL == ptdev)   return -EIO;
    TIM_HandleTypeDef *htim = NULL;
    unsigned int freq = 0;
    
    if(ptdev->port == TIM2)
    {
        htim = &htim2;
        freq = HAL_RCC_GetPCLK1Freq() * 2;
    }
    else if(ptdev->port == TIM4)
    {
        htim = &htim4;
        freq = HAL_RCC_GetPCLK1Freq() * 2;
    }
    else if(ptdev->port == TIM10)
    {
        htim = &htim10;
        freq = HAL_RCC_GetPCLK2Freq() * 2;
    }

    if(NULL == htim)    return -EIO;
    freq = freq / 1000000;
    switch(cmd)
    {
        case SET_PRESCALER_VALUE:
        {
            __HAL_TIM_SET_PRESCALER(htim, cfg);
            break;
        }
        case SET_CLKDIV_VALUE:
        {
            if(cfg == 1)
                __HAL_TIM_SET_CLOCKDIVISION(htim, TIM_CLOCKDIVISION_DIV1);
            else if(cfg == 2)
                __HAL_TIM_SET_CLOCKDIVISION(htim, TIM_CLOCKDIVISION_DIV2);
            else if(cfg == 4)
                __HAL_TIM_SET_CLOCKDIVISION(htim, TIM_CLOCKDIVISION_DIV4);
            break;
        }
        case SET_PERIOD_VALUE:
        {
            unsigned int max_input = (0xFFFFFFFF)/freq + freq;
            unsigned int period = 1;
            if(cfg >= max_input)
                period = cfg / 1000 * freq;
            else
                period = cfg * freq / 1000;
            unsigned int prescaler = (period>>16) + 1;
            period = period / prescaler;
            __HAL_TIM_SET_PRESCALER(htim, prescaler - 1);
            __HAL_TIM_SET_AUTORELOAD(htim, period - 1);
            break;
        }
        default:break;
    }
    
    return ESUCCESS;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    void (*irqhandler)(void *arg);
    if(TIM2 == htim->Instance)
    {
        gTIM2Count++;
        irqhandler = TimerIrqHandler[2];
    }
    else if(TIM4 == htim->Instance)
    {
        irqhandler = TimerIrqHandler[4];
    }
    else if(TIM10 == htim->Instance)
    {
        gTIM10Count++;
        irqhandler = TimerIrqHandler[10];
    }
    if(NULL != irqhandler)
        irqhandler(NULL);
}

