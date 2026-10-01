#include "devices.h"
#include "errno.h"
#include "libs.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>

unsigned char wbuf[20480] = {0};
unsigned char rbuf[20480] = {0};

void app_w25q64_test(void)
{
    IODevicesRegister();
    SPIDevicesRegister();
    UartDevicesRegister();
    
    W25QDevice *pW25Q64 = GetW25Q64Device();
    if(NULL == pW25Q64) return;
    if(ESUCCESS != pW25Q64->Init(pW25Q64))  return;
    
    for(unsigned int i=0; i<20480; i++)
    {
        wbuf[i] = i;
    }
    // 计算写入时间
    unsigned int tick = HAL_GetTick();
    unsigned int addr = rand()%256;
    xprintf("Test Address: 0x%X\r\n", addr);
    if(pW25Q64->Write(pW25Q64, addr, wbuf, 20480) != 20480)
        return;
    xprintf("Write 20480 bytes by SPI DMA Time: %d\r\n", HAL_GetTick() - tick);
    // 计算读出时间
    tick = HAL_GetTick();
    if(pW25Q64->Read(pW25Q64, addr, rbuf, 20480) != 20480)
        return;
    xprintf("Read 20480 bytes by SPI DMA Time: %d\r\n", HAL_GetTick() - tick);
    for(unsigned int i=0; i<256; i++)
    {
        if( (i%16)==0 && i != 0 )
            xprintf("\r\n");
        xprintf("0x%X\t", rbuf[i]);
    }
    xprintf("\r\n");
}


