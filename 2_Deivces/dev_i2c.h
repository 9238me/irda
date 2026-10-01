#ifndef __DEV_I2C_H
#define __DEV_I2C_H

typedef struct I2CDev{
    char *name;
    unsigned char channel;
    unsigned short own_addr;
    unsigned short slave_addr;
    
    int (*Init)(struct I2CDev *ptdev);
    int (*Write)(struct I2CDev *ptdev, unsigned char *buf, unsigned int length);
    int (*Read)(struct I2CDev *ptdev, unsigned char *buf, unsigned int length);
    struct I2CDev *next;
}I2CDevice;

void I2CDevicesRegister(void);
void I2CDeviceInsert(I2CDevice *ptdev);
I2CDevice *I2CDeviceFind(const char *name);

#endif /* __DEV_I2C_H */
