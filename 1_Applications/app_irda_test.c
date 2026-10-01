#include "devices.h"
#include "libs.h"
#include "errno.h"
#include <string.h>

static void clear_screen(unsigned short color);

void app_irda_test(void)
{
    IODevicesRegister();
    UartDevicesRegister();
    SPIDevicesRegister();
    TimerDevicesRegister();
    
    IrdaDevice *pIrda = GetIrdaDevice();
    if(NULL == pIrda)   return;
    int ret = pIrda->Init(pIrda);
    if(ESUCCESS != ret) return;
    
    DisplayDevice *pDisp = GetST7789Device();
    if(NULL == pDisp)   return;
    ret = pDisp->Init(pDisp);
    if(ESUCCESS != ret) return;
    clear_screen(0xFFFF);
    
    while(1)
    {
        if(ESUCCESS == pIrda->Read(pIrda))
        {
            clear_screen(0xFFFF);
            ShowStringEN(pIrda->name, 88, 0, 0x0000);
            ShowStringEN(pIrda->key, 120 - strlen(pIrda->key)*8/2, 16, 0x0000);
        }
    }
}

static void clear_screen(unsigned short color)
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
