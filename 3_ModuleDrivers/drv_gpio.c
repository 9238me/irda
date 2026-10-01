#include "dev_gpio.h"
#include "./drv_config/gpio_config.h"
#include "errno.h"
#include "libs.h"
#include "stm32f4xx_hal.h"
#include <string.h>

static int GPIODrvInit(struct GPIODev *ptdev);
static int GPIODrvWrite(struct GPIODev *ptdev, unsigned char status);
static int GPIODrvRead(struct GPIODev *ptdev);

static GPIODevice gGPIODevices[] = {
    PB1,
    PC2, PC3, PC4, PC5,
    PE2, PE3, PE4, PE5,
    PF6, PF7, PF8,
    PG6, PG7, PG8, PG9, PG11, PG12, PG13, PG14,
    W25Q64CS, LCDCS, LCDDCX, LCDRST, LCDBACK,
    D1, D2, D3, D4, K1, K2, K3, K4,
};

EXTI_HandleTypeDef hexti1 = {
    .Line = EXTI_LINE_1,
    .PendingCallback = NULL,
};

EXTI_HandleTypeDef hexti2 = {
    .Line = EXTI_LINE_2,
    .PendingCallback = NULL,
};

EXTI_HandleTypeDef hexti3 = {
    .Line = EXTI_LINE_3,
    .PendingCallback = NULL,
};

EXTI_HandleTypeDef hexti4 = {
    .Line = EXTI_LINE_4,
    .PendingCallback = NULL,
};

EXTI_HandleTypeDef hexti5 = {
    .Line = EXTI_LINE_5,
    .PendingCallback = NULL,
};

void GPIODevicesCreate(void)
{
    unsigned int num = sizeof(gGPIODevices) / sizeof(struct GPIODev);
    for(unsigned int i = 0; i<num; i++)
        IODeviceInsert(&gGPIODevices[i]);
}

static int GPIODrvInit(struct GPIODev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    EXTI_ConfigTypeDef nConfig = {
        .Mode = EXTI_MODE_INTERRUPT,
        .Trigger = EXTI_TRIGGER_RISING_FALLING,
    };
    
    if(strstr(ptdev->name, "PB1"))
    {
        hexti1.PendingCallback = ptdev->IrqHandler;
        nConfig.Line = EXTI_LINE_1;
        nConfig.GPIOSel = EXTI_GPIOB;
        HAL_StatusTypeDef status = HAL_EXTI_SetConfigLine(&hexti1, &nConfig);
        if(HAL_OK != status)    return -EIO;
    }
    else if(strstr(ptdev->name, "PE2"))
    {
        hexti2.PendingCallback = ptdev->IrqHandler;
        nConfig.Line = EXTI_LINE_2;
        nConfig.GPIOSel = EXTI_GPIOE;
        HAL_StatusTypeDef status = HAL_EXTI_SetConfigLine(&hexti2, &nConfig);
        if(HAL_OK != status)    return -EIO;
    }
    else if(strstr(ptdev->name, "PE3"))
    {
        hexti3.PendingCallback = ptdev->IrqHandler;
        nConfig.Line = EXTI_LINE_3;
        nConfig.GPIOSel = EXTI_GPIOE;
        HAL_StatusTypeDef status = HAL_EXTI_SetConfigLine(&hexti3, &nConfig);
        if(HAL_OK != status)    return -EIO;
    }
    else if(strstr(ptdev->name, "PE4"))
    {
        hexti4.PendingCallback = ptdev->IrqHandler;
        nConfig.Line = EXTI_LINE_4;
        nConfig.GPIOSel = EXTI_GPIOE;
        HAL_StatusTypeDef status = HAL_EXTI_SetConfigLine(&hexti4, &nConfig);
        if(HAL_OK != status)    return -EIO;
    }
    else if(strstr(ptdev->name, "PE5"))
    {
        hexti5.PendingCallback = ptdev->IrqHandler;
        nConfig.Line = EXTI_LINE_5;
        nConfig.GPIOSel = EXTI_GPIOE;
        HAL_StatusTypeDef status = HAL_EXTI_SetConfigLine(&hexti5, &nConfig);
        if(HAL_OK != status)    return -EIO;
    }
    
    return ESUCCESS;
}

static int GPIODrvWrite(struct GPIODev *ptdev, unsigned char status)
{
    if(NULL == ptdev)   return -EINVAL;
    if(status !=0 && status != 1)   return -EINVAL;
    HAL_GPIO_WritePin(ptdev->port, ptdev->pin, status);
    ptdev->value = status;
    
    return ESUCCESS;
}

static int GPIODrvRead(struct GPIODev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    ptdev->value = HAL_GPIO_ReadPin(ptdev->port, ptdev->pin);
    
    return ESUCCESS;
}
