#include "dev_uart.h"
#include "mytype.h"
#include "drivers.h"
#include <string.h>

static UartDevice *gHeadDevice = NULLDEV;

void UartDevicesRegister(void)
{
    UartDevicesCreate();
}

void UartDeviceInsert(UartDevice *ptdev)
{
    if(NULLDEV == gHeadDevice)
    {
        gHeadDevice = ptdev;
    }
    else
    {
        ptdev->next = gHeadDevice;
        gHeadDevice = ptdev;
    }
}

UartDevice *UartDeviceFind(const char *name)
{
    UartDevice *ptdev = gHeadDevice;
    while(NULLDEV != ptdev)
    {
        if(strstr(ptdev->name, name))
            return ptdev;
        ptdev = ptdev->next;
    }
    
    return NULLDEV;
}
