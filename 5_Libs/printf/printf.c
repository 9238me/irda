#include "devices.h"
#include <stdio.h>

struct __FILE{
    int handle;
};

FILE __stdout;

int fputc(int ch, FILE *f)
{
    (void)f;
    UartDevice *ptdev = UartDeviceFind("Debug UART");
    if(NULL == ptdev)   return 0;
    
    if(ptdev->Write(ptdev, (unsigned char*)&ch, 1) == 1)
        return ch;
    
    return 0;
}
