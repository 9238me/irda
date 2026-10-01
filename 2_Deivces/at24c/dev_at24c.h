#ifndef __DEV_AT24C_H
#define __DEV_AT24C_H

typedef struct AT24CXDev{
    char *name;
    int (*Init)(struct AT24CXDev *ptdev);
    int (*Write)(struct AT24CXDev *ptdev, unsigned short addr, unsigned char *buf, unsigned short length);
    int (*Read)(struct AT24CXDev *ptdev, unsigned short addr, unsigned char *buf, unsigned short length);
}AT24CXDevice;

#define USE_AT24C02_MODULE      (1)

#define PAGE_SIZE               (8)

#ifdef USE_AT24C01A_MODULE
    #define MEM_SIZE_MAX        (128)
#endif /* USE_AT24C01A_MODULE */

#ifdef USE_AT24C02_MODULE
    #define MEM_SIZE_MAX        (256)
    AT24CXDevice *GetAT24C02Device(void);
#endif /* USE_AT24C02_MODULE */

#ifdef USE_AT24C04_MODULE
    #define MEM_SIZE_MAX        (512)
#endif /* USE_AT24C04_MODULE */

#ifdef USE_AT24C08A_MODULE
    #define MEM_SIZE_MAX        (1024)
#endif /* USE_AT24C08A_MODULE */

#ifdef USE_AT24C16A_MODULE
    #define MEM_SIZE_MAX        (2048)
#endif /* USE_AT24C16A_MODULE */

#endif /* __DEV_AT24C_H */
