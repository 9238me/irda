#include "devices.h"
#include "errno.h"
#include "libs.h"

void app_spi_test(void)
{
    IODevicesRegister();
    SPIDevicesRegister();
    UartDevicesRegister();
    
    GPIODevice *pW25QCS = IODeviceFind("W25Q CS");
    if(NULL == pW25QCS) return;
    
    SPIDevice *pSPI1 = SPIDeviceFind("SPI1");
    if(NULL == pSPI1)   return;
    
    UartDevice *pDbgUart = UartDeviceFind("Debug UART");
    if(NULL == pDbgUart)    return;
    
    if(ESUCCESS != pW25QCS->Init(pW25QCS))  return;
    if(ESUCCESS != pSPI1->Init(pSPI1))  return;
    if(ESUCCESS != pDbgUart->Init(pDbgUart))  return;
    
    xprintf("SPI Test\r\n");
    
    unsigned char wbuf[8] = {1, 2, 3, 4, 5, 6, 7};
    unsigned char rbuf[8] = {0};
    
    pW25QCS->Write(pW25QCS, 0); // Enable W25Q64
    int ret = pSPI1->Write(pSPI1, wbuf, 8);
    if(8 != ret)
    {
        xprintf("SPI1 Transmit Data Failed!\r\n");
        return;
    }
    
    ret = pSPI1->Read(pSPI1, rbuf, 8);
    if(8 != ret)
    {
        xprintf("SPI1 Receive Data Failed!\r\n");
        return;
    }
    
    
    ret = pSPI1->WriteRead(pSPI1, wbuf, rbuf, 8);
    if(8 != ret)
    {
        xprintf("SPI1 Transmit Data Failed!\r\n");
        return;
    }
    pW25QCS->Write(pW25QCS, 1); // Disable W25Q64
}