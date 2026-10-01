#include "devices.h"
#include "libs.h"
#include "errno.h"
#include "string.h"

static int AT24C02Init(struct AT24CXDev *ptdev);
static int AT24C02Write(struct AT24CXDev *ptdev, unsigned short addr, unsigned char *buf, unsigned short length);
static int AT24C02Read(struct AT24CXDev *ptdev, unsigned short addr, unsigned char *buf, unsigned short length);

static AT24CXDevice gAT24C02 = {
    .name = "AT24C02",
    .Init = AT24C02Init,
    .Write = AT24C02Write,
    .Read = AT24C02Read
};

AT24CXDevice *GetAT24C02Device(void)
{
    return &gAT24C02;
}

static int AT24C02Init(struct AT24CXDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    I2CDevice *pI2C = I2CDeviceFind("I2C1");
    if(NULL == pI2C)    return -ENODEV;
    if(ESUCCESS != pI2C->Init(pI2C))    return -EIO;
    pI2C->slave_addr = 0x50;
    return ESUCCESS;
}

static int AT24C02ByteWrite(unsigned char addr, unsigned char data)
{
    I2CDevice *pI2C = I2CDeviceFind("I2C1");
    if(NULL == pI2C)    return -ENODEV;
    unsigned char buf[2] = {addr, data};
    int ret = pI2C->Write(pI2C, buf, 2);
    if(ret != 2)    return -EIO;
    mdelay(5);
    return 2;
}

static int AT24C02PageWrite(unsigned char addr, unsigned char *data, unsigned char length)
{
    if(NULL == data)    return -EINVAL;
    if(0 == length)     return -EINVAL;
    I2CDevice *pI2C = I2CDeviceFind("I2C1");
    if(NULL == pI2C)    return -ENODEV;
    // 组合数据，第一个字节是数据地址
    // 一页的数据大小是8-byte，加上一个数据地址，就是9-byte
    unsigned char buf[9] = {addr};
    // 从第二个字节开始是数据
    memcpy(&buf[1], data, length);
    // 发送length个数据以及一个addr，总共就是发送length+1个字节
    int ret = pI2C->Write(pI2C, buf, length + 1);
    if(ret != (length+1))   return -EIO;
    // 如果发送成功了，就等待5ms，等待从机AT24C02将数据写入EEPROM
    mdelay(5);
    return (int)length;
}

static int AT24C02Write(struct AT24CXDev *ptdev, unsigned short addr, unsigned char *buf, unsigned short length)
{
    if(NULL == ptdev)       return -EINVAL;
    if(NULL == buf)         return -EINVAL;
    if(addr >= MEM_SIZE_MAX)    return -EINVAL;
    if(0 == length)         return -EINVAL;
    unsigned short init_len = length;
    // 判断addr是否页对齐
    unsigned char naddr = addr % PAGE_SIZE;
    if(naddr != 0)
    {
        unsigned char size = PAGE_SIZE - naddr;
        if(length > size)
            size = size;
        else
            size = length;
        int ret = AT24C02PageWrite(addr, buf, size);
        if(ret != size) return -EIO;
        addr += size;
        buf += size;
        length -= size;
    }
    while(length)
    {
        unsigned char size = 0;
        if(length > PAGE_SIZE)
            size = PAGE_SIZE;
        else
            size = length;
        int ret = AT24C02PageWrite(addr, buf, size);
        if(ret != size) return -EIO;
        addr += size;
        buf += size;
        length -= size;
    }
    
    return (int)init_len;
}

static int AT24C02Read(struct AT24CXDev *ptdev, unsigned short addr, unsigned char *buf, unsigned short length)
{
    if(NULL == ptdev)       return -EINVAL;
    if(NULL == buf)         return -EINVAL;
    if(addr >= MEM_SIZE_MAX)    return -EINVAL;
    if(0 == length)         return -EINVAL;
    I2CDevice *pI2C = I2CDeviceFind("I2C1");
    if(NULL == pI2C)    return -ENODEV;
    // Dummy Write，把数据地址写到AT24C02，让它知道MCU从何处开始读取数据
    int ret = pI2C->Write(pI2C, (unsigned char *)&addr, 1);
    if(ret != 1)    return -EIO;
    // Read Data
    ret = pI2C->Read(pI2C, buf, length);
    if(ret != length)   return -EIO;
    return (int)length;
}