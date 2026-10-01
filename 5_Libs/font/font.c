#include "devices.h"
#include "libs.h"
#include "errno.h"

static void ShowCharacter(const char c, unsigned short x, unsigned short y, unsigned short color)
{
    DisplayDevice *ptdev = GetST7789Device();
    if(NULL == ptdev)   return;
    ptdev->SetDisplayWindow(ptdev, x, y, x + 8, y + 16);
    unsigned short fbuf[128] = {0};
    ptdev->FBBase = (unsigned short *)fbuf;
    unsigned char *pbuf = GetASCIIBuffer(c);
    for(unsigned short y=0; y<16; y++)
    {
        for(unsigned short x=0; x<8; x++)
        {
            if((pbuf[y]&(1<<x)) != 0)
                ptdev->SetPixel(ptdev, x, y, color);
            else
                ptdev->SetPixel(ptdev, x, y, ~color);
        }
    }
    ptdev->Flush(ptdev);
}

void ShowChineseHalf(unsigned char *buf, unsigned short x, unsigned short y, unsigned short color)
{
    DisplayDevice *ptdev = GetST7789Device();
    if(NULL == ptdev)   return;
    ptdev->SetDisplayWindow(ptdev, x, y, x + 8, y + 16);
    unsigned short fbuf[128] = {0};
    ptdev->FBBase = (unsigned short *)fbuf;
    for(unsigned short y=0; y<16; y++)
    {
        for(unsigned short x=0; x<8; x++)
        {
            if((buf[y]&(1<<x)) != 0)
                ptdev->SetPixel(ptdev, x, y, color);
            else
                ptdev->SetPixel(ptdev, x, y, ~color);
        }
    }
    ptdev->Flush(ptdev);
}

void ShowStringEN(const char *str, unsigned short x, unsigned short y, unsigned short color)
{
    unsigned short dx = x, dy = y;
    while(*str)
    {
        ShowCharacter(*str, dx, dy, color);
        dx = dx + 8;
        if(dx >= 240)
        {
            dx = x;
            dy = dy + 16;
        }
        if(dy >= 320)
        {
            dy = y;
        }
        str++;
    }
}
