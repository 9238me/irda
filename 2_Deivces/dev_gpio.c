#include "dev_gpio.h"
#include "mytype.h"
#include "drivers.h"
#include <string.h>

static GPIODevice *gHeadDevice = NULLDEV;

void IODevicesRegister(void)
{
    GPIODevicesCreate();
}

void IODeviceInsert(GPIODevice *ptdev)
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

GPIODevice *IODeviceFind(const char *name)
{
    GPIODevice *ptdev = gHeadDevice;
    while(NULLDEV != ptdev)
    {
        if(strstr(ptdev->name, name))
            return ptdev;
        ptdev = ptdev->next;
    }
    
    return NULLDEV;
}
