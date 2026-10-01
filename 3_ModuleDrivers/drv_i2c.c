#include "dev_i2c.h"
#include "./drv_config/i2c_config.h"
#include "i2c.h"
#include "errno.h"
#include "libs.h"
#include "stm32f4xx_hal.h"

static int I2CDrvInit(struct I2CDev *ptdev);
static int I2CDrvWrite(struct I2CDev *ptdev, unsigned char *buf, unsigned int length);
static int I2CDrvRead(struct I2CDev *ptdev, unsigned char *buf, unsigned int length);

static void I2C1WaitTxCplt(void);
static void I2C1WaitRxCplt(void);

static volatile unsigned char gI2C1TxCpltFlag = 0;
static volatile unsigned char gI2C1RxCpltFlag = 0;

static I2CDevice gI2CDevices[] = {
    I2C1Dev,
};

void I2CDevicesCreate(void)
{
    unsigned int num = sizeof(gI2CDevices) / sizeof(struct I2CDev);
    for(unsigned int i = 0; i<num; i++)
        I2CDeviceInsert(&gI2CDevices[i]);
}

static int I2CDrvInit(struct I2CDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    switch(ptdev->channel)
    {
        case 1:
        {
            ptdev->own_addr = hi2c1.Init.OwnAddress1;
            break;
        }
        case 2:case 3:break;
        default:break;
    }
    
    return ESUCCESS;
}

static int I2CDrvWrite(struct I2CDev *ptdev, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)   return -EINVAL;
    if(0 == length)   return -EINVAL;
    
    unsigned int init_len = length;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            unsigned char *pbuf = buf;
            if(length == 1)
            {
                HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(&hi2c1, ptdev->slave_addr<<1, buf, length);
                if(HAL_OK != status)    return -EIO;
                I2C1WaitTxCplt();
                break;
            }
            while(length)
            {
                unsigned int size = 0;
                if(length >= 65536)
                    size = 65535;
                else
                    size = length;
                HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(&hi2c1, ptdev->slave_addr<<1, pbuf, size);
                if(HAL_OK != status)    return -EIO;
                I2C1WaitTxCplt();
                pbuf += size;
                length -= size;
            }
            break;
        }
        case 2:case 3:break;
        default:break;
    }
    return (int)init_len;
}

static int I2CDrvRead(struct I2CDev *ptdev, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)   return -EINVAL;
    if(0 == length)   return -EINVAL;
    
    unsigned int init_len = length;
    
    switch(ptdev->channel)
    {
        case 1:
        {
            unsigned char *pbuf = buf;
            if(length == 1)
            {
                HAL_StatusTypeDef status = HAL_I2C_Master_Receive_IT(&hi2c1, ptdev->slave_addr<<1, buf, length);
                if(HAL_OK != status)    return -EIO;
                I2C1WaitRxCplt();
                break;
            }
            while(length)
            {
                unsigned int size = 0;
                if(length >= 65536)
                    size = 65535;
                else
                    size = length;
                HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(&hi2c1, ptdev->slave_addr<<1, pbuf, size);
                if(HAL_OK != status)    return -EIO;
                I2C1WaitRxCplt();
                pbuf += size;
                length -= size;
            }
            break;
        }
        case 2:case 3:break;
        default:break;
    }
    return (int)init_len;
}

static void I2C1WaitTxCplt(void)
{
    while(gI2C1TxCpltFlag != 1);
    gI2C1TxCpltFlag = 0;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        gI2C1TxCpltFlag = 1;
    }
}

static void I2C1WaitRxCplt(void)
{
    while(gI2C1RxCpltFlag != 1);
    gI2C1RxCpltFlag = 0;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        gI2C1RxCpltFlag = 1;
    }
}
