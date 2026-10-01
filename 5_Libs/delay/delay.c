#include "devices.h"
#include "errno.h"
#include "mytype.h"

static int TimerDelay(unsigned int timeout);

int delay(unsigned int sec)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULLDEV == ptdev)   return -ENODEV;
    if(ptdev->status == 1)
    {
        int ret = ptdev->Stop(ptdev);
        if(ESUCCESS != ret) return -EIO;
    }
    ptdev->Control(ptdev, SET_PERIOD_VALUE, 1000); // 1000us counter
    unsigned int timeout = sec * 1000;
    while(timeout)
    {
        unsigned int tick = 0;
        if(timeout > 0xFFFF)
            tick = 0xFFFF;
        else
            tick = timeout;
        timeout = timeout - tick;
        if(TimerDelay(tick) != ESUCCESS)    return -EIO;
    }
    
    return ESUCCESS;
}

int mdelay(unsigned int msec)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULLDEV == ptdev)   return -ENODEV;
    if(ptdev->status == 1)
    {
        int ret = ptdev->Stop(ptdev);
        if(ESUCCESS != ret) return -EIO;
    }
    ptdev->Control(ptdev, SET_PERIOD_VALUE, 1000); // 1000us counter
    unsigned int timeout = msec;
    while(timeout)
    {
        unsigned int tick = 0;
        if(timeout > 0xFFFF)
            tick = 0xFFFF;
        else
            tick = timeout;
        timeout = timeout - tick;
        if(TimerDelay(tick) != ESUCCESS)    return -EIO;
    }
    
    return ESUCCESS;
}

int udelay(unsigned int usec)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULLDEV == ptdev)   return -ENODEV;
    if(ptdev->status == 1)
    {
        int ret = ptdev->Stop(ptdev);
        if(ESUCCESS != ret) return -EIO;
    }
    ptdev->Control(ptdev, SET_PERIOD_VALUE, 1); // 1us counter
    unsigned int timeout = usec;
    while(timeout)
    {
        unsigned int tick = 0;
        if(timeout > 0xFFFF)
            tick = 0xFFFF;
        else
            tick = timeout;
        timeout = timeout - tick;
        if(TimerDelay(tick) != ESUCCESS)    return -EIO;
    }
    
    return ESUCCESS;
}

static int TimerDelay(unsigned int timeout)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULLDEV == ptdev)   return -ENODEV;
    if(ptdev->status == 0)
    {
        int ret = ptdev->Start(ptdev);
        if(ESUCCESS != ret) return -EIO;
    }
    unsigned int tick = ptdev->GetCount(ptdev);
    unsigned int wait = tick + timeout;
    if(wait < 0xFFFFFFFF)
        wait = wait + 1;
    else
        return -EINVAL;
    while(ptdev->GetCount(ptdev) < wait);

    return ptdev->Stop(ptdev);
}
