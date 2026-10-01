#include "dev_timer.h"
#include "drivers.h"
#include "mytype.h"
#include "errno.h"
#include "libs.h"
#include <string.h>

static TimerDevice *gHeadDevice = NULLDEV;

void TimerDevicesRegister(void)
{
    TimerDevicesCreate();
    PWMDevicesCreate();
}

void TimerDeviceInsert(TimerDevice *ptdev)
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

TimerDevice *TimerDeviceFind(const char *name)
{
    TimerDevice *ptdev = gHeadDevice;
    while(NULLDEV != ptdev)
    {
        if(strstr(ptdev->name, name))
            return ptdev;
        ptdev = ptdev->next;
    }
    
    return NULLDEV;
}

