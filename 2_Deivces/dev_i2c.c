#include "dev_i2c.h"
#include "mytype.h"
#include "drivers.h"
#include <string.h>

static I2CDevice *gHeadDevice = NULL;

void I2CDevicesRegister(void)
{
    I2CDevicesCreate();
}

void I2CDeviceInsert(I2CDevice *ptdev)
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

I2CDevice *I2CDeviceFind(const char *name)
{    
    I2CDevice *ptdev = gHeadDevice;
    while(NULLDEV != ptdev)
    {
        if(strstr(ptdev->name, name))
            return ptdev;
        ptdev = ptdev->next;
    }
    
    return NULLDEV;
}