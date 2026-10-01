#include "dev_uart.h"
#include "./drv_config/uart_config.h"
#include "usart.h"
#include "errno.h"
#include "libs.h"
#include "stm32f4xx_hal.h"

static int UartDrvInit(struct UartDev *ptdev);
static int UartDrvWrite(struct UartDev *ptdev, const unsigned char *buf, unsigned short length);
static int UartDrvRead(struct UartDev *ptdev, unsigned char *buf, unsigned short length);
static void USART1DrvWaitTxCplt(void);

static UartDevice gUartDevices[] = {
    DGBUART
};

static RingBuffer *gUsart1Buffer = NULL;
static volatile unsigned char gRxData = 0;
static volatile unsigned char gUsart1TxCpltFlag = 0;

void UartDevicesCreate(void)
{
    unsigned int num = sizeof(gUartDevices) / sizeof(struct UartDev);
    for(unsigned int i = 0; i<num; i++)
        UartDeviceInsert(&gUartDevices[i]);
}

static int UartDrvInit(struct UartDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            gUsart1Buffer = RingBufferNew(sizeof(unsigned char)*256);
            if(NULL == gUsart1Buffer)   return -ENOMEM;
            HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart1, (unsigned char*)&gRxData, 1);
            if(HAL_OK != status)        return -EIO;
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        default:break;
    }
    
    return ESUCCESS;
}

static int UartDrvWrite(struct UartDev *ptdev, const unsigned char *buf, unsigned short length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&huart1, buf, length);
            if(HAL_OK != status)    return -EIO;
            USART1DrvWaitTxCplt();
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        default:break;
    }
    
    return (int)length;
}

static int UartDrvRead(struct UartDev *ptdev, unsigned char *buf, unsigned short length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            if(NULL != gUsart1Buffer)
            {
                int ret = gUsart1Buffer->Read(gUsart1Buffer, buf, length);
                if(ret != length)   return ret;
            }
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        default:break;
    }
    
    return (int)length;
}

static void USART1DrvWaitTxCplt(void)
{
    while(gUsart1TxCpltFlag != 1);
    gUsart1TxCpltFlag = 0;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        gUsart1TxCpltFlag = 1;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(NULL != gUsart1Buffer)
        {
            gUsart1Buffer->Write(gUsart1Buffer, (unsigned char*)&gRxData, 1);
            HAL_UART_Receive_IT(&huart1, (unsigned char*)&gRxData, 1);
        }
    }
}

