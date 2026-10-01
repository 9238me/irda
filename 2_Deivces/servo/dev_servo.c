#include "devices.h"
#include "libs.h"
#include "errno.h"

static int ServoInit(struct ServoDev *ptdev);
static int ServoStart(struct ServoDev *ptdev);
static int ServoStop(struct ServoDev *ptdev);
static int ServoSetAngle(struct ServoDev *ptdev, int angle);

static ServoDevice gServo = {
    .name   = "Servo 180",
    .status = SERVO_STOP,
    .angle  = 0,
    .Init   = ServoInit,
    .Start  = ServoStart,
    .Stop   = ServoStop,
    .SetAngle = ServoSetAngle
};

ServoDevice *GetServoDevice(void)
{
    return &gServo;
}

static int ServoInit(struct ServoDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    PWMDevice *ptPWM = PWMDeviceFind("TIM3 PWM3");
    if(NULL == ptPWM)   return -ENODEV;
    if(ESUCCESS != ptPWM->Init(ptPWM))  return -EIO;
    int ret = ptPWM->Control(ptPWM, SET_PERIOD_VALUE, 20000000);
    if(ESUCCESS != ret) return ret;
    ret = ptPWM->Control(ptPWM, SET_PULSE_VALUE, 500000);
    if(ESUCCESS != ret) return ret;
    ret = ptPWM->Start(ptPWM);
    if(ESUCCESS != ret) return ret;
    
    ptdev->angle = 0;
    ptdev->status = SERVO_START;
    
    return ESUCCESS;
}

static int ServoStart(struct ServoDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(ptdev->status == SERVO_START)    return ESUCCESS;
    
    PWMDevice *ptPWM = PWMDeviceFind("TIM3 PWM3");
    if(NULL == ptPWM)   return -ENODEV;
    
    int ret = ptPWM->Start(ptPWM);
    if(ESUCCESS != ret) return ret;
    
    ptdev->status = SERVO_START;
    return ESUCCESS;
}

static int ServoStop(struct ServoDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(ptdev->status == SERVO_STOP)    return ESUCCESS;
    
    PWMDevice *ptPWM = PWMDeviceFind("TIM3 PWM3");
    if(NULL == ptPWM)   return -ENODEV;
    
    int ret = ptPWM->Stop(ptPWM);
    if(ESUCCESS != ret) return ret;
    
    ptdev->status = SERVO_STOP;
    return ESUCCESS;
}

static int ServoSetAngle(struct ServoDev *ptdev, int angle)
{
    if(NULL == ptdev)   return -EINVAL;
    if(angle > SERVO_MAX_ANGLE) return -EINVAL;
    
    PWMDevice *ptPWM = PWMDeviceFind("TIM3 PWM3");
    if(NULL == ptPWM)   return -ENODEV;
    if(ptdev->status == SERVO_STOP)
    {
        int ret = ServoStart(ptdev);
        if(ret != ESUCCESS) return ret;
    }
    unsigned int pulse = 500000 + angle * 500000 / 45;
    int ret = ptPWM->Control(ptPWM, SET_PULSE_VALUE, pulse);
    if(ret == ESUCCESS) return ret;
    ptdev->angle = angle;
    
    return ESUCCESS;
}
