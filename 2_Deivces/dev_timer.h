#ifndef __DEV_TIMER_H
#define __DEV_TIMER_H

typedef enum{
    SET_PRESCALER_VALUE = 1<<1,
    SET_CLKDIV_VALUE    = 1<<2,
    SET_PERIOD_VALUE    = 1<<3, // us unit
    GET_PERIOD_VALUE    = 1<<4,
    SET_PULSE_VALUE     = 1<<5,
    GET_PULSE_VALUE     = 1<<6,
}GeneralTimerCtrlItems;

typedef struct TimerDev{
    char *name;
    void *port;
    unsigned char channel;
    unsigned char status;
    int (*Init)(struct TimerDev *ptdev);
    int (*Start)(struct TimerDev *ptdev);
    int (*Stop)(struct TimerDev *ptdev);
    int (*GetCount)(struct TimerDev *ptdev);
    int (*Control)(struct TimerDev *ptdev, unsigned int cmd, unsigned int cfg);
    void (*IrqHandler)(void *arg);
    struct TimerDev *next;
}TimerDevice;

void TimerDevicesRegister(void);
void TimerDeviceInsert(TimerDevice *ptdev);
TimerDevice *TimerDeviceFind(const char *name);

#endif /* __DEV_TIMER_H */
