#include "devices.h"
#include "libs.h"
#include "errno.h"

void My_IRQHandler(void *arg)
{
    GPIODevice *pD1 = IODeviceFind("D1");
    if(NULL == pD1)   return;
    pD1->Read(pD1);
    pD1->Write(pD1, !pD1->value);
}

void app_timer_irq(void)
{
    IODevicesRegister();
    UartDevicesRegister();
    SPIDevicesRegister();
    TimerDevicesRegister();
    
    GPIODevice *pD1 = IODeviceFind("D1");
    if(NULL == pD1)   return;
    if(ESUCCESS != pD1->Init(pD1))  return;
    
    TimerDevice *pTIM4 = TimerDeviceFind("TIM4");
    if(NULL == pTIM4)   return;
    pTIM4->IrqHandler = My_IRQHandler;
    if(ESUCCESS != pTIM4->Init(pTIM4))  return;
    if(ESUCCESS != pTIM4->Control(pTIM4, SET_PERIOD_VALUE, 500000000))  return;
    if(ESUCCESS != pTIM4->Start(pTIM4)) return;
    while(1)
    {
    }
}
