#include "devices.h"
#include "mytype.h"

void app_led_test(void)
{
    IODevicesRegister();
    
    GPIODevice *pD1 = IODeviceFind("D1");
    if(NULLDEV == pD1) return;
    GPIODevice *pD2 = IODeviceFind("D2");
    if(NULLDEV == pD2) return;
    GPIODevice *pD3 = IODeviceFind("D3");
    if(NULLDEV == pD2) return;
    GPIODevice *pD4 = IODeviceFind("D4");
    if(NULLDEV == pD2) return;
    
    pD1->Init(pD1);
    pD2->Init(pD2);
    pD3->Init(pD3);
    pD4->Init(pD4);
    while(1)
    {

    }
}
