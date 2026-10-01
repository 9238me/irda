#include "devices.h"
#include "libs.h"
#include "errno.h"

static char gTrackerName[7][5] = {"PC2", "PC3", "PC4", "PC5", "PF6", "PF7", "PF8"};
static MotorDevice *pLFMotor = NULL;
static MotorDevice *pRFMotor = NULL;
static MotorDevice *pLBMotor = NULL;
static MotorDevice *pRBMotor = NULL;

void motor_track_task(void *arg)
{
    volatile unsigned char tracker = 0;
    for(unsigned char i=0; i<7; i++)
    {
        GPIODevice *pIO = IODeviceFind(gTrackerName[i]);
        if(NULL == pIO)   return;
        pIO->Read(pIO);
        tracker = (tracker<<1) + pIO->value;
    }
    volatile unsigned short tLeft = (tracker&0x70)>>4;
    volatile unsigned short tCenter = (tracker&0x1C)>>2;
    volatile unsigned short tRight = (tracker&0x07);
    
    volatile unsigned char a = (tLeft&0x04)>>2;
    volatile unsigned char b = (tLeft&0x02)>>1;
    volatile unsigned char c = (tLeft&0x01);
    tLeft = (a<<2) + (b<<1) + c;
    
    a = (tCenter&0x02)>>1;
    b = (tCenter&0x04)>>2;
    c = (tCenter&0x01);
    tCenter = (a<<2) + (b<<1) + c;
    
    a = (tRight&0x01);
    b = (tRight&0x02)>>1;
    c = (tRight&0x04)>>2;
    tRight = (a<<2) + (b<<1) + c;
    static volatile unsigned int lost = 0;
    if((tLeft > tCenter) && (tLeft > tRight))
    {
        lost = 0;
        pLFMotor->SetSpeed(pLFMotor, 100);
        pRFMotor->SetSpeed(pRFMotor, 100);
        pLBMotor->SetSpeed(pLBMotor, 100);
        pRBMotor->SetSpeed(pRBMotor, 100);
        
        pLFMotor->SetDirection(pLFMotor, MOTOR_CCR);
        pLBMotor->SetDirection(pLBMotor, MOTOR_CCR);
        pRFMotor->SetDirection(pRFMotor, MOTOR_CR);
        pRBMotor->SetDirection(pRBMotor, MOTOR_CR);
    }
    else if((tRight > tCenter) && (tRight > tLeft))
    {
        lost = 0;
        pLFMotor->SetSpeed(pLFMotor, 100);
        pRFMotor->SetSpeed(pRFMotor, 100);
        pLBMotor->SetSpeed(pLBMotor, 100);
        pRBMotor->SetSpeed(pRBMotor, 100);
        
        pLFMotor->SetDirection(pLFMotor, MOTOR_CR);
        pLBMotor->SetDirection(pLBMotor, MOTOR_CR);
        pRFMotor->SetDirection(pRFMotor, MOTOR_CCR);
        pRBMotor->SetDirection(pRBMotor, MOTOR_CCR);
    }
    else if((tCenter > tRight) && (tCenter > tLeft))
    {
        lost = 0;
        pLFMotor->SetSpeed(pLFMotor, 100);
        pRFMotor->SetSpeed(pRFMotor, 100);
        pLBMotor->SetSpeed(pLBMotor, 100);
        pRBMotor->SetSpeed(pRBMotor, 100);
        
        pLFMotor->SetDirection(pLFMotor, MOTOR_CR);
        pLBMotor->SetDirection(pLBMotor, MOTOR_CR);
        pRFMotor->SetDirection(pRFMotor, MOTOR_CR);
        pRBMotor->SetDirection(pRBMotor, MOTOR_CR);
    }
    else
    {
        lost++;
        if(lost >= 5000)
        {
            lost = 0;
            pLFMotor->SetSpeed(pLFMotor, 0);
            pRFMotor->SetSpeed(pRFMotor, 0);
            pLBMotor->SetSpeed(pLBMotor, 0);
            pRBMotor->SetSpeed(pRBMotor, 0);
        }
    }
}

void app_track_move(void)
{
    IODevicesRegister();
    UartDevicesRegister();
    SPIDevicesRegister();
    TimerDevicesRegister();
    
    for(unsigned char i=0; i<7; i++)
    {
        GPIODevice *pIO = IODeviceFind(gTrackerName[i]);
        if(NULL == pIO)   return;
        if(ESUCCESS != pIO->Init(pIO))  return;
    }
    
    pLFMotor = GetMotorDevice(1);
    if(NULL == pLFMotor)   return;
    if(ESUCCESS != pLFMotor->Init(pLFMotor))  return;
    
    pRFMotor = GetMotorDevice(2);
    if(NULL == pRFMotor)   return;
    if(ESUCCESS != pRFMotor->Init(pRFMotor))  return;
    
    pLBMotor = GetMotorDevice(3);
    if(NULL == pLBMotor)   return;
    if(ESUCCESS != pLBMotor->Init(pLBMotor))  return;
    
    pRBMotor = GetMotorDevice(4);
    if(NULL == pRBMotor)   return;
    if(ESUCCESS != pRBMotor->Init(pRBMotor))  return;
    
    TimerDevice *pTIM4 = TimerDeviceFind("TIM4");
    if(NULL == pTIM4)   return;
    pTIM4->IrqHandler = motor_track_task;
    if(ESUCCESS != pTIM4->Init(pTIM4))  return;
    if(ESUCCESS != pTIM4->Control(pTIM4, SET_PERIOD_VALUE, 100000))  return;
    if(ESUCCESS != pTIM4->Start(pTIM4)) return;
    while(1)
    {
    }
}
