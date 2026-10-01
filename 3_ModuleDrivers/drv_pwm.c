#include "./drv_config/pwm_config.h"
#include "devices.h"
#include "libs.h"
#include "errno.h"
#include "tim.h"

static int PWMDrvInit(PWMDevice *ptdev);
static int PWMDrvStart(PWMDevice *ptdev);
static int PWMDrvStop(PWMDevice *ptdev);
static int PWMDrvControl(PWMDevice *ptdev, unsigned int cmd, unsigned int cfg);

static PWMDevice gPWMDevices[] = {
    TIM1_PWM1, TIM3_PWM3, TIM8_PWM1, TIM8_PWM2, TIM8_PWM3, TIM8_PWM4
};

void PWMDevicesCreate(void)
{
    unsigned int num = sizeof(gPWMDevices) / sizeof(PWMDevice);
    for(unsigned int i = 0; i<num; i++)
        PWMDeviceInsert(&gPWMDevices[i]);
}

static int PWMDrvInit(PWMDevice *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    return ESUCCESS;
}

static int PWMDrvStart(PWMDevice *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(1 == ptdev->status)   return ESUCCESS;
    
    TIM_HandleTypeDef *htim = NULL;
    if(ptdev->port == TIM1)
        htim = &htim1;
    else if(ptdev->port == TIM3)
        htim = &htim3;
    else if(ptdev->port == TIM8)
        htim = &htim8;
    if(NULL == htim)    return -EIO;
    unsigned int channel = (ptdev->channel - 1) * 4;
    HAL_StatusTypeDef status = HAL_TIM_PWM_Start(htim, channel);
    if(HAL_OK != status)    return -EIO;
    ptdev->status = 1;
    return ESUCCESS;
}

static int PWMDrvStop(PWMDevice *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(0 == ptdev->status)   return ESUCCESS;
    
    TIM_HandleTypeDef *htim = NULL;
    if(ptdev->port == TIM1)
        htim = &htim1;
    else if(ptdev->port == TIM3)
        htim = &htim3;
    else if(ptdev->port == TIM8)
        htim = &htim8;
    if(NULL == htim)    return -EIO;
    unsigned int channel = (ptdev->channel - 1) * 4;
    HAL_StatusTypeDef status = HAL_TIM_PWM_Stop(htim, channel);
    if(HAL_OK != status)    return -EIO;
    ptdev->status = 0;
    return ESUCCESS;
}

static int PWMDrvControl(PWMDevice *ptdev, unsigned int cmd, unsigned int cfg)
{
    if(NULL == ptdev)   return -EIO;
    TIM_HandleTypeDef *htim = NULL;
    unsigned int freq = 0;
    
    if(ptdev->port == TIM1)
    {
        htim = &htim1;
        freq = HAL_RCC_GetPCLK2Freq() * 2;
    }
    else if(ptdev->port == TIM3)
    {
        htim = &htim3;
        freq = HAL_RCC_GetPCLK1Freq() * 2;
    }
    else if(ptdev->port == TIM8)
    {
        htim = &htim8;
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
            unsigned int period = cfg * freq / 1000;
            unsigned int prescaler = (period>>16) + 1;
            period = period / prescaler;
            __HAL_TIM_SET_PRESCALER(htim, prescaler - 1);
            __HAL_TIM_SET_AUTORELOAD(htim, period - 1);
            break;
        }
        case SET_PULSE_VALUE:
        {
            unsigned int period = (htim->Instance->ARR + 1) * (htim->Instance->PSC + 1) / freq;
            unsigned int pulse = cfg / 1000 * (htim->Instance->ARR + 1) / period;
            unsigned int channel = (ptdev->channel - 1) * 4;
            __HAL_TIM_SET_COMPARE(htim, channel, pulse);
            break;
        }
        default:break;
    }
    
    return ESUCCESS;
}
