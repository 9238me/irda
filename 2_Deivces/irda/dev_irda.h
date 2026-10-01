#ifndef __DEV_IRDA_H
#define __DEV_IRDA_H

typedef struct IrdaDev{
    char *name;
    char *key;
    int (*Init)(struct IrdaDev *ptdev);
    int (*Read)(struct IrdaDev *ptdev);
}IrdaDevice;

IrdaDevice *GetIrdaDevice(void);

#endif /* __DEV_IRDA_H */
