#include "dev_spi.h"
#include "./drv_config/spi_config.h"
#include "spi.h"
#include "errno.h"
#include "libs.h"
#include "stm32f4xx_hal.h"

static int SPIDrvInit(struct SPIDev *ptdev);
static int SPIDrvWrite(struct SPIDev *ptdev, unsigned char *wbuf, unsigned int length);
static int SPIDrvRead(struct SPIDev *ptdev, unsigned char *rbuf, unsigned int length);
static int SPIDrvWriteRead(struct SPIDev *ptdev, unsigned char *wbuf, unsigned char *rbuf, unsigned int length);

static void SPI1WaitTxCplt(void);
static void SPI1WaitRxCplt(void);
static void SPI1WaitTxRxCplt(void);

static SPIDevice gSPIDevices[] = {
    SPI1Dev,
};

static volatile unsigned char gSPI1TxCplt = 0;  // 0-未完成;1-完成
static volatile unsigned char gSPI1RxCplt = 0;  // 0-未完成;1-完成
static volatile unsigned char gSPI1TxRxCplt = 0;  // 0-未完成;1-完成

void SPIDevicesCreate(void)
{
    unsigned int num = sizeof(gSPIDevices) / sizeof(struct SPIDev);
    for(unsigned int i = 0; i<num; i++)
        SPIDeviceInsert(&gSPIDevices[i]);
}

static int SPIDrvInit(struct SPIDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    switch(ptdev->channel)
    {
        case 1:break;
        case 2:break;
        case 3:break;
        default:break;
    }
    
    return ESUCCESS;
}

static int SPIDrvWrite(struct SPIDev *ptdev, unsigned char *wbuf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == wbuf)    return -EINVAL;
    if(0 == length)     return -EINVAL;
    
    unsigned int init_len = length;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            unsigned char *pbuf = wbuf;
            if(length == 1)
            {
                HAL_StatusTypeDef status = HAL_SPI_Transmit_IT(&hspi1, wbuf, length);
                if(HAL_OK != status)    return -EIO;
                SPI1WaitTxCplt();
                break;
            }
            else
            {
                while(length != 0)
                {
                    unsigned int size = 0;
                    if(length >= 65536)
                        size = 65535;
                    else
                        size = length;
                    HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(&hspi1, pbuf, size);
                    if(HAL_OK != status)    return -EIO;
                    SPI1WaitTxCplt();
                    pbuf = pbuf + size;
                    length = length - size;
                    break;
                }
            }
        }
        case 2:break;
        case 3:break;
        default:break;
    }
    
    return (int)init_len;
}

static int SPIDrvRead(struct SPIDev *ptdev, unsigned char *rbuf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == rbuf)    return -EINVAL;
    if(0 == length)     return -EINVAL;
    
    unsigned int init_len = length;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            unsigned char *pbuf = rbuf;
            if(length == 1)
            {
                HAL_StatusTypeDef status = HAL_SPI_Receive_IT(&hspi1, rbuf, length);
                if(HAL_OK != status)    return -EIO;
                SPI1WaitRxCplt();
                break;
            }
            else
            {
                while(length != 0)
                {
                    unsigned int size = 0;
                    if(length > 65536)
                        size = 65535;
                    else
                        size = length;
                    HAL_StatusTypeDef status = HAL_SPI_Receive_DMA(&hspi1, pbuf, size);
                    if(HAL_OK != status)    return -EIO;
                    SPI1WaitRxCplt();
                    pbuf = pbuf + size;
                    length = length - size;
                    break;
                }
            }
        }
        case 2:break;
        case 3:break;
        default:break;
    }
    
    return (int)init_len;
}

static int SPIDrvWriteRead(struct SPIDev *ptdev, unsigned char *wbuf, unsigned char *rbuf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == wbuf)    return -EINVAL;
    if(NULL == rbuf)    return -EINVAL;
    if(0 == length)     return -EINVAL;
    
    unsigned int init_len = length;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            unsigned char *pwbuf = wbuf;
            unsigned char *prbuf = rbuf;
            if(length == 1)
            {
                HAL_StatusTypeDef status = HAL_SPI_TransmitReceive_IT(&hspi1, wbuf, rbuf, length);
                if(HAL_OK != status)    return -EIO;
                SPI1WaitTxRxCplt();
                break;
            }
            else
            {
                while(length != 0)
                {
                    unsigned int size = 0;
                    if(length > 65536)
                        size = 65535;
                    else
                        size = length;
                    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive_DMA(&hspi1, pwbuf, prbuf, size);
                    if(HAL_OK != status)    return -EIO;
                    SPI1WaitTxRxCplt();
                    pwbuf = pwbuf + size;
                    prbuf = prbuf + size;
                    length = length - size;
                    break;
                }
            }
        }
        case 2:break;
        case 3:break;
        default:break;
    }
    
    return (int)init_len;
}

static void SPI1WaitTxCplt(void)
{
    while(gSPI1TxCplt == 0);
    gSPI1TxCplt = 0;
}

static void SPI1WaitRxCplt(void)
{
    while(gSPI1RxCplt == 0);
    gSPI1RxCplt = 0;
}

static void SPI1WaitTxRxCplt(void)
{
    while(gSPI1TxRxCplt == 0);
    gSPI1TxRxCplt = 0;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(SPI1 == hspi->Instance)
    {
        gSPI1TxCplt = 1;
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(SPI1 == hspi->Instance)
    {
        gSPI1RxCplt = 1;
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(SPI1 == hspi->Instance)
    {
        gSPI1TxRxCplt = 1;
    }
}
