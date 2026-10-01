#ifndef __DEV_UART_H
#define __DEV_UART_H

typedef struct UartDev{
    char *name;
    unsigned char channel;
    int (*Init)(struct UartDev *ptdev);
    int (*Write)(struct UartDev *ptdev, const unsigned char *buf, unsigned short length);
    int (*Read)(struct UartDev *ptdev, unsigned char *buf, unsigned short length);
    struct UartDev *next;
}UartDevice;

void UartDevicesRegister(void);
void UartDeviceInsert(UartDevice *ptdev);
UartDevice *UartDeviceFind(const char *name);

#endif /* __DEV_UART_H */
