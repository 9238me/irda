#include "devices.h"
#include "errno.h"
#include "libs.h"
#include "mytype.h"
#include "stm32f4xx_hal.h"
#include <string.h>

void app_uart_test(void)
{
    IODevicesRegister();
    UartDevicesRegister();
        
    GPIODevice *pK1 = IODeviceFind("K1");
    if(NULLDEV == pK1) return;
    GPIODevice *pD1 = IODeviceFind("D1");
    if(NULLDEV == pD1) return;
    
    if(pK1->Init(pK1) != ESUCCESS)  return;
    if(pD1->Init(pD1) != ESUCCESS)  return;
    
    UartDevice *pDbgUart = UartDeviceFind("Debug UART");
    if(NULLDEV == pDbgUart) return;
    if(pDbgUart->Init(pDbgUart) != ESUCCESS)  return;
    
    xprintf("Hello World!\r\n");
    
    while(1)
    {
        char c = 0;
        if(pDbgUart->Read(pDbgUart, (unsigned char*)&c, 1) == 1)
        {
            xprintf("%c\r\n", c);
            if(c == 'a' || c == 'A')
            {
                pD1->Write(pD1, 0);
            }
            else if(c == 'b' || c == 'B')
            {
                pD1->Write(pD1, 1);
            }
        }
        if(pK1->Read(pK1) == ESUCCESS)
        {
            HAL_Delay(150);
            if(pK1->Read(pK1) == ESUCCESS)
            {
                xprintf("K1 is pressed!\r\n");
            }
        }
    }
}
