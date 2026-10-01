#include "dev_spi.h"
#include "mytype.h"
#include "drivers.h"
#include <string.h>

static SPIDevice *gHeadDevice = NULL;

void SPIDevicesRegister(void)
{
    SPIDevicesCreate();
}

void SPIDeviceInsert(SPIDevice *ptdev)
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

SPIDevice *SPIDeviceFind(const char *name)
{    
    SPIDevice *ptdev = gHeadDevice;
    while(NULLDEV != ptdev)
    {
        if(strstr(ptdev->name, name))
            return ptdev;
        ptdev = ptdev->next;
    }
    
    return NULLDEV;
}
