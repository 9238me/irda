#include "devices.h"
#include "libs.h"
#include "errno.h"

void clear_screen(unsigned short color);

unsigned char wbuf[256] = {0};
unsigned char rbuf[256] = {0};
unsigned short fbuf[240*16] = {0};
void app_i2c_test(void)
{
    IODevicesRegister();
    UartDevicesRegister();
    SPIDevicesRegister();
    I2CDevicesRegister();
    TimerDevicesRegister();
    
    DisplayDevice *ptDisplay = GetST7789Device();
    if(NULL == ptDisplay)   return;
    if(ESUCCESS != ptDisplay->Init(ptDisplay))  return;
    clear_screen(0xFFFF);
    ptDisplay->FBBase = (unsigned short*)fbuf;
    ptDisplay->SetDisplayWindow(ptDisplay, 0, 0, 240, 16);
    
    AT24CXDevice *ptAT24C02 = GetAT24C02Device();
    if(NULL == ptAT24C02)   return;
    if(ESUCCESS != ptAT24C02->Init(ptAT24C02))  return;
    
    unsigned int line = 0;
    for(unsigned short i=0; i<256; i++)
    {
        wbuf[i] = i;
    }
    int ret = ptAT24C02->Write(ptAT24C02, 13, wbuf, 256);
    if(ret != 256)
    {
        char str[36] = {0};
        sprintf(str, "AT24C02 Write Error");
        ShowStringEN(str, 0, line, 0xF800);
        line += 16;
    }
    ret = ptAT24C02->Read(ptAT24C02, 13, rbuf, 256);
    if(ret != 256)
    {
        char str[36] = {0};
        sprintf(str, "AT24C02 Read Error");
        ShowStringEN(str, 0, line, 0x8800);
        line += 16;
    }
    unsigned int count = 0;
    for(unsigned short i=0; i<256; i++)
    {
        if(wbuf[i] != rbuf[i])
        {
            char str[64] = {0};
            sprintf(str, "The %d - Write:%d - Read%d", i, wbuf[i], rbuf[i]);
            ShowStringEN(str, 0, line, 0x4500);
            line += 16;
            count++;
        }
    }
    if(count == 0)
    {
        char str[36] = {0};
        sprintf(str, "AT24C02 Test OK");
        ShowStringEN(str, 0, line, 0x2500);
    }
}

void clear_screen(unsigned short color)
{
    DisplayDevice *ptdev = GetST7789Device();
    if(NULL == ptdev)   return;
    unsigned short nbuf[240] = {0};
    for(unsigned short i = 0; i<240; i++)
    {
        nbuf[i] = color;
    }

    ptdev->FBBase = (unsigned short *)nbuf;
    for(unsigned short i = 0; i<320; i++)
    {
        ptdev->SetDisplayWindow(ptdev, 0, i, 240, i+1);
        ptdev->Flush(ptdev);
    }
}