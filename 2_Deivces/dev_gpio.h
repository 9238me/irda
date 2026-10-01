#ifndef __DEV_GPIO_H
#define __DEV_GPIO_H

typedef struct GPIODev{
    char *name;
    void *port;
    unsigned int pin;
    unsigned char value;
    int (*Init)(struct GPIODev *ptdev);
    int (*Write)(struct GPIODev *ptdev, unsigned char status);
    int (*Read)(struct GPIODev *ptdev);
    void (*IrqHandler)(void);
    struct GPIODev *next;
}GPIODevice;

void IODevicesRegister(void);
void IODeviceInsert(GPIODevice *ptdev);
GPIODevice *IODeviceFind(const char *name);

#endif /* __DEV_GPIO_H */
