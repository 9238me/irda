#ifndef __DEV_SPI_H
#define __DEV_SPI_H

typedef struct SPIDev{
    char *name;
    unsigned char channel;
    int (*Init)(struct SPIDev *ptdev);
    int (*Write)(struct SPIDev *ptdev, unsigned char *wbuf, unsigned int length);
    int (*Read)(struct SPIDev *ptdev, unsigned char *rbuf, unsigned int length);
    int (*WriteRead)(struct SPIDev *ptdev, unsigned char *wbuf, unsigned char *rbuf, unsigned int length);
    struct SPIDev *next;
}SPIDevice;

void SPIDevicesRegister(void);
void SPIDeviceInsert(SPIDevice *ptdev);
SPIDevice *SPIDeviceFind(const char *name);

#endif /* __DEV_SPI_H */
