#include "devices.h"
#include "libs.h"
#include "errno.h"

typedef struct SpeedData{
    unsigned int count;
    unsigned int tick;
}SpeedData;

static void clear_screen(unsigned short color);

static volatile SpeedData gLFSpeed = {0, 0};
static volatile SpeedData gRFSpeed = {0, 0};
static volatile SpeedData gLBSpeed = {0, 0};
static volatile SpeedData gRBSpeed = {0, 0};

void LeftFrontSpeed(void)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULL == ptdev)   return;
    gLFSpeed.count++;
    gLFSpeed.tick = ptdev->GetCount(ptdev);
}

void RightFrontSpeed(void)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULL == ptdev)   return;
    gRFSpeed.count++;
    gRFSpeed.tick = ptdev->GetCount(ptdev);
}

void LeftBackSpeed(void)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULL == ptdev)   return;
    gLBSpeed.count++;
    gLBSpeed.tick = ptdev->GetCount(ptdev);
}

void RightBackSpeed(void)
{
    TimerDevice *ptdev = TimerDeviceFind("TIM2");
    if(NULL == ptdev)   return;
    gRBSpeed.count++;
    gRBSpeed.tick = ptdev->GetCount(ptdev);
}

void app_pwm_test(void)
{
    IODevicesRegister();
    UartDevicesRegister();
    SPIDevicesRegister();
    TimerDevicesRegister();
    
    GPIODevice *pLFSpeed = IODeviceFind("PE2");
    if(NULL == pLFSpeed)    return;
    pLFSpeed->IrqHandler = LeftFrontSpeed;
    if(ESUCCESS != pLFSpeed->Init(pLFSpeed))    return;
    
    GPIODevice *pRFSpeed = IODeviceFind("PE3");
    if(NULL == pRFSpeed)    return;
    pRFSpeed->IrqHandler = RightFrontSpeed;
    if(ESUCCESS != pRFSpeed->Init(pRFSpeed))    return;
    
    GPIODevice *pLBSpeed = IODeviceFind("PE4");
    if(NULL == pLBSpeed)    return;
    pLBSpeed->IrqHandler = LeftBackSpeed;
    if(ESUCCESS != pLBSpeed->Init(pLBSpeed))    return;
    
    GPIODevice *pRBSpeed = IODeviceFind("PE5");
    if(NULL == pRBSpeed)    return;
    pRBSpeed->IrqHandler = RightBackSpeed;
    if(ESUCCESS != pRBSpeed->Init(pRBSpeed))    return;
    
    DisplayDevice *pDisp = GetST7789Device();
    if(NULL == pDisp)   return;
    if(ESUCCESS != pDisp->Init(pDisp))  return;
    clear_screen(0xFFFF);
    
    MotorDevice *pLFMotor = GetMotorDevice(1);
    if(NULL == pLFMotor)   return;
    if(ESUCCESS != pLFMotor->Init(pLFMotor))  return;
    
    MotorDevice *pRFMotor = GetMotorDevice(2);
    if(NULL == pRFMotor)   return;
    if(ESUCCESS != pRFMotor->Init(pRFMotor))  return;
    
    MotorDevice *pLBMotor = GetMotorDevice(3);
    if(NULL == pLBMotor)   return;
    if(ESUCCESS != pLBMotor->Init(pLBMotor))  return;
    
    MotorDevice *pRBMotor = GetMotorDevice(4);
    if(NULL == pRBMotor)   return;
    if(ESUCCESS != pRBMotor->Init(pRBMotor))  return;
    
    if(ESUCCESS != pLFMotor->SetSpeed(pLFMotor, 100))   return;
    if(ESUCCESS != pRFMotor->SetSpeed(pRFMotor, 80))   return;
    if(ESUCCESS != pLBMotor->SetSpeed(pLBMotor, 60))   return;
    if(ESUCCESS != pRBMotor->SetSpeed(pRBMotor, 40))   return;
    
    while(1)
    {
        SpeedData lfspeed1 = gLFSpeed;
        SpeedData rfspeed1 = gRFSpeed;
        SpeedData lbspeed1 = gLBSpeed;
        SpeedData rbspeed1 = gRBSpeed;
        mdelay(1000);
        SpeedData lfspeed2 = gLFSpeed;
        SpeedData rfspeed2 = gRFSpeed;
        SpeedData lbspeed2 = gLBSpeed;
        SpeedData rbspeed2 = gRBSpeed;
        SpeedData lfspeed;
        SpeedData rfspeed;
        SpeedData lbspeed;
        SpeedData rbspeed;
        // LF
        lfspeed.count = lfspeed2.count - lfspeed1.count;
        lfspeed.tick = lfspeed2.tick - lfspeed1.tick;
        float lfs = lfspeed.count * 25.0 / lfspeed.tick;
        // RF
        rfspeed.count = rfspeed2.count - rfspeed1.count;
        rfspeed.tick = rfspeed2.tick - rfspeed1.tick;
        float rfs = rfspeed.count * 25.0 / rfspeed.tick;
        // LB
        lbspeed.count = lbspeed2.count - lbspeed1.count;
        lbspeed.tick = lbspeed2.tick - lbspeed1.tick;
        float lbs = lbspeed.count * 25.0 / lbspeed.tick;
        // RB
        rbspeed.count = rbspeed2.count - rbspeed1.count;
        rbspeed.tick = rbspeed2.tick - rbspeed1.tick;
        float rbs = rbspeed.count * 25. / rbspeed.tick;
        
        {
            char str[32];
            sprintf(str, "LF:%.2f \t RF:%.2f", lfs, rfs);
            ShowStringEN(str, 0, 0, 0x0000);
        }
        {
            char str[32];
            sprintf(str, "LB:%.2f \t RB:%.2f", lbs, rbs);
            ShowStringEN(str, 0, 16, 0x0000);
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