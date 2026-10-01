#include "devices.h"
#include "libs.h"
#include "errno.h"

static int WriteRegister(unsigned char reg);
static int WriteData(unsigned char *buf, unsigned int length);
static int ReadData(unsigned char *buf, unsigned int length);
static int HWReset(void);
static int SWReset(void);
static int ReadDisplayID(void);
static int ReadDisplayStatus(void);
static int ReadDisplayPixelFormat(void);
static int SleepIn(void);
static int SleepOut(void);
static int SetPartialModeON(void);
static int SetNormalModeON(void);
static int SetInversionOFF(void);
static int SetInversionON(void);
static int ST7789VDisplayOFF(struct DisplayDevice *ptDev);
static int ST7789VDisplayON(struct DisplayDevice *ptDev);
static int SetMemDataAddrCtrl(unsigned char value);
static int SetPixelFormat(unsigned char bits);


static int ST7789VInit(struct DisplayDevice *ptDev);   /* 硬件初始化 */
static int ST7789VDisplayON(struct DisplayDevice *ptDev);   /* 开启显示 */
static int ST7789VDisplayOFF(struct DisplayDevice *ptDev);   /* 关闭显示 */
static int ST7789VSetDisplayWindow(struct DisplayDevice* ptDev, \
                                    unsigned short wXs, unsigned short wYs, \
                                    unsigned short wXe, unsigned short wYe);
/* 设置FBBase中的数据, 把(iX,iY)的像素设置为颜色dwColor
 * dwColor的格式:0x00RRGGBB
 */
static int ST7789VSetPixel(struct DisplayDevice *ptDev, \
                            unsigned short wX, unsigned short wY, \
                            unsigned short wColor);
static int ST7789VFlush(struct DisplayDevice *ptDev); /* 把FBBase的数据刷到LCD的显存里 */
                            
static DisplayDevice gST7789v = {
    .name   = "ST7789V",
    .FBBase = NULL,
    .wXres  = 240,
    .wYres  = 320,
    .wBpp   = 16,
    .dwSize = 0,
    .Init       = ST7789VInit,
    .DisplayON  = ST7789VDisplayON,
    .DisplayOFF = ST7789VDisplayOFF,
    .SetDisplayWindow = ST7789VSetDisplayWindow,
    .SetPixel   = ST7789VSetPixel,
    .Flush      = ST7789VFlush
};

DisplayDevice *GetST7789Device(void)
{
    return &gST7789v;
}

static int ST7789VInit(struct DisplayDevice *ptDev)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)    return -ENODEV;
    if(ESUCCESS != pCS->Init(pCS))  return -EIO;
    
    GPIODevice *pDCX = IODeviceFind("LCD DCX");
    if(NULL == pDCX)    return -ENODEV;
    if(ESUCCESS != pDCX->Init(pDCX))  return -EIO;
    
    GPIODevice *pReset = IODeviceFind("LCD RESET");
    if(NULL == pReset)    return -ENODEV;
    if(ESUCCESS != pReset->Init(pReset))  return -EIO;
    
    GPIODevice *pBack = IODeviceFind("LCD BACK");
    if(NULL == pBack)    return -ENODEV;
    if(ESUCCESS != pBack->Init(pBack))  return -EIO;
    pBack->Write(pBack, 0);
    
    SPIDevice *pSPI = SPIDeviceFind("SPI1");
    if(NULL == pSPI)    return -ENODEV;
    if(ESUCCESS != pSPI->Init(pSPI))  return -EIO;
    
    // Get Display ID
    unsigned int id = ReadDisplayID();
    xprintf("ST7789v ID: 0x%X\r\n", id);
    
    int ret = HWReset();
    if(ESUCCESS != ret) return ret;
    
    ret = SleepOut();
    if(ESUCCESS != ret) return ret;
    
    ret = SetNormalModeON();
    if(ESUCCESS != ret) return ret;
    
    ret = SetInversionOFF();
    if(ESUCCESS != ret) return ret;
    
    ret = SetMemDataAddrCtrl(0x00);
    if(ESUCCESS != ret) return ret;
    
    ret = SetPixelFormat(16);
    if(ESUCCESS != ret) return ret;
    
    ret = ST7789VDisplayON(ptDev);
    if(ESUCCESS != ret) return ret;
    
    return ESUCCESS;
}

static int WriteRegister(unsigned char reg)
{
    GPIODevice *pDCX = IODeviceFind("LCD DCX");
    if(NULL == pDCX)    return -ENODEV;
    SPIDevice *pSPI = SPIDeviceFind("SPI1");
    if(NULL == pSPI)    return -ENODEV;
    
    pDCX->Write(pDCX, 0);
    if(pSPI->Write(pSPI, &reg, 1) != 1)
    {
        pDCX->Write(pDCX, 1);
        return -EIO;
    }
    pDCX->Write(pDCX, 1);
    return ESUCCESS;
}

static int WriteData(unsigned char *buf, unsigned int length)
{
    SPIDevice *pSPI = SPIDeviceFind("SPI1");
    if(NULL == pSPI)    return -ENODEV;
    
    return pSPI->Write(pSPI, buf, length);
}

static int ReadData(unsigned char *buf, unsigned int length)
{
    SPIDevice *pSPI = SPIDeviceFind("SPI1");
    if(NULL == pSPI)    return -ENODEV;
    
    return pSPI->Read(pSPI, buf, length);
}

static int HWReset(void)
{
    GPIODevice *pReset = IODeviceFind("LCD RESET");
    if(NULL == pReset)    return -ENODEV;
    pReset->Write(pReset, 0);
    mdelay(150);
    pReset->Write(pReset, 1);
    mdelay(5);
    
    return ESUCCESS;
}

static int SWReset(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x01);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        mdelay(5);
        return ESUCCESS;
    }
    return ret;
}

static int ReadDisplayID(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x04);
    if(ret != ESUCCESS)    return ret;
    unsigned char data[4] = {0};
    ret = ReadData(data, 4);
    pCS->Write(pCS, 1);
    
    if(ret != 4)    return ret;
    unsigned int id = (data[1]<<16) | (data[2]<<8) | (data[3]);
    return id;
}

static int ReadDisplayStatus(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x09);
    if(ret != ESUCCESS)    return ret;
    unsigned char data[5] = {0};
    ret = ReadData(data, 5);
    pCS->Write(pCS, 1);
    
    if(ret != 5)    return ret;
    unsigned int *pStatus = (unsigned int *)&data[1];
    return (int)(*pStatus);
}

static int ReadDisplayPixelFormat(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x0C);
    if(ret != ESUCCESS)    return ret;
    unsigned char data[2] = {0};
    ret = ReadData(data, 2);
    pCS->Write(pCS, 1);
    
    if(ret != 2)    return ret;
    return (int)(data[1]);
}

static int SleepIn(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x10);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}

static int SleepOut(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x11);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        mdelay(5);
        return ESUCCESS;
    }
    return ret;
}

static int SetPartialModeON(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x12);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}

static int SetNormalModeON(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x13);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}

static int SetInversionOFF(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x20);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}

static int SetInversionON(void)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x21);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}


/* 关闭显示 */
static int ST7789VDisplayOFF(struct DisplayDevice *ptDev)
{
    if(NULL == ptDev)   return -ENODEV;
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x28);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}

/* 开启显示 */
static int ST7789VDisplayON(struct DisplayDevice *ptDev)
{
    if(NULL == ptDev)   return -ENODEV;
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x29);
    pCS->Write(pCS, 1);
    if(ret == ESUCCESS)
    {
        return ESUCCESS;
    }
    return ret;
}

static int ST7789VSetDisplayWindow(struct DisplayDevice* ptDev, 
                                   unsigned short wXs, unsigned short wYs, 
                                   unsigned short wXe, unsigned short wYe)
{
    if(NULL == ptDev)   return -ENODEV;
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    unsigned short xe = wXe - 1;
    unsigned short ye = wYe - 1;
    
    pCS->Write(pCS, 0);
    // Column Address Command: 0x2A
    // Data[4] = (xs[15:8], xs[7:0], xe[15:8], xe[7:0])
    unsigned char xdata[4] = {wXs>>8, wXs, xe>>8, xe};
    int ret = WriteRegister(0x2A);
    if(ret != ESUCCESS)    return ret;
    ret = WriteData(xdata, 4);
    if(ret != 4)
    {
        pCS->Write(pCS, 1);
        return ret;
    }
    // Row Address Command: 0x2B
    // Data[4] = (ys[15:8], ys[7:0], ye[15:8], ye[7:0])
    unsigned char ydata[4] = {wYs>>8, wYs, ye>>8, ye};
    ret = WriteRegister(0x2B);
    if(ret != ESUCCESS)    return ret;
    ret = WriteData(ydata, 4);
    if(ret != 4)
    {
        pCS->Write(pCS, 1);
        return ret;
    }
    pCS->Write(pCS, 1);
    // 重新计算显示区域的分辨率和字节数
    ptDev->wXres = wXe - wXs;
    ptDev->wYres = wYe - wYs;
    ptDev->dwSize = ptDev->wXres * ptDev->wYres * ptDev->wBpp / 8;
    
    return ESUCCESS;
}

/* 把FBBase的数据刷到LCD的显存里 */
static int ST7789VFlush(struct DisplayDevice *ptDev)
{
    if(NULL == ptDev)   return -EINVAL;
    if(NULL == ptDev->FBBase)   return -ENOMEM;
    
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x2C);
    if(ret != ESUCCESS)
    {
        pCS->Write(pCS, 1);
        return ret;
    }
    ret = WriteData(ptDev->FBBase, ptDev->dwSize);
    pCS->Write(pCS, 1);
    
    if(ret != ptDev->dwSize)    return ret;
    return ESUCCESS;
}

static int SetMemDataAddrCtrl(unsigned char value)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x36);
    if(ret != ESUCCESS)
    {
        pCS->Write(pCS, 1);
        return ret;
    }
    ret = WriteData(&value, 1);
    pCS->Write(pCS, 1);
    
    if(ret != 1)    return ret;
    return ESUCCESS;
}

static int SetPixelFormat(unsigned char bits)
{
    GPIODevice *pCS = IODeviceFind("LCD CS");
    if(NULL == pCS)     return -ENODEV;
    
    pCS->Write(pCS, 0);
    int ret = WriteRegister(0x3A);
    if(ret != ESUCCESS)
    {
        pCS->Write(pCS, 1);
        return ret;
    }
    unsigned char para = 0x55;
    if(12 == bits)
    {
        para = 0x53;
    }
    else if(16 == bits)
    {
        para = 0x55;
    }
    else if(18 == bits)
    {
        para = 0x66;
    }
    ret = WriteData(&para, 1);
    pCS->Write(pCS, 1);
    
    if(ret != 1)    return ret;
    return ESUCCESS;
}

static int ST7789VSetPixel(struct DisplayDevice *ptDev,
                            unsigned short wX, unsigned short wY,
                            unsigned short wColor)
{
    if(NULL == ptDev)           return -EINVAL;
    if(NULL == ptDev->FBBase)   return -ENOMEM;
    
    unsigned short *pbuf = (unsigned short *)ptDev->FBBase;
    pbuf = pbuf + (wY * ptDev->wXres + wX);
    if(NULL == pbuf)            return -ENOMEM;
    
    *pbuf = wColor;
    return ESUCCESS;
}
