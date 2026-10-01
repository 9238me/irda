#include "devices.h"
#include "errno.h"
#include "mytype.h"

void app_key_test(void)
{
    IODevicesRegister();
    
    GPIODevice *pK1 = IODeviceFind("K1");
    if(NULLDEV == pK1) return;
    GPIODevice *pD1 = IODeviceFind("D1");
    if(NULLDEV == pD1) return;
    
    if(pK1->Init(pK1) != ESUCCESS)  return;
    if(pD1->Init(pD1) != ESUCCESS)  return;
    while(1)
    {
        int status = pK1->Read(pK1);
        if(ESUCCESS == status)
            pD1->Write(pD1, pK1->value);
    }
}
