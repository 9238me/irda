#include "devices.h"
#include "libs.h"
#include "errno.h"
#include "string.h"

static int MotorInit(struct MotorDev *ptdev);
static int MotorStart(struct MotorDev *ptdev);
static int MotorStop(struct MotorDev *ptdev);
static int MotorSetSpeed(struct MotorDev *ptdev, unsigned int speed);
static int MotorSetDirection(struct MotorDev *ptdev, unsigned int set_dir);

static unsigned char CRIOs[MOTOR_NUM][5]  = {"PG6", "PG8", "PG11", "PG13"};
static unsigned char CCRIOs[MOTOR_NUM][5] = {"PG7", "PG9", "PG12", "PG14"};
static unsigned char PWMs[MOTOR_NUM][10] = {"TIM8 PWM1", "TIM8 PWM2", "TIM8 PWM3", "TIM8 PWM4"};

static MotorDevice gLFMotor = {
    .channel    = 1,
    .status     = MOTOR_STOP,
    .dir        = MOTOR_CR,
    .Init       = MotorInit,
    .Start      = MotorStart,
    .Stop       = MotorStop,
    .SetSpeed   = MotorSetSpeed,
    .SetDirection = MotorSetDirection
};

static MotorDevice gRFMotor = {
    .channel    = 2,
    .status     = MOTOR_STOP,
    .dir        = MOTOR_CR,
    .Init       = MotorInit,
    .Start      = MotorStart,
    .Stop       = MotorStop,
    .SetSpeed   = MotorSetSpeed,
    .SetDirection = MotorSetDirection
};

static MotorDevice gLBMotor = {
    .channel    = 3,
    .status     = MOTOR_STOP,
    .dir        = MOTOR_CR,
    .Init       = MotorInit,
    .Start      = MotorStart,
    .Stop       = MotorStop,
    .SetSpeed   = MotorSetSpeed,
    .SetDirection = MotorSetDirection
};

static MotorDevice gRBMotor = {
    .channel    = 4,
    .status     = MOTOR_STOP,
    .dir        = MOTOR_CR,
    .Init       = MotorInit,
    .Start      = MotorStart,
    .Stop       = MotorStop,
    .SetSpeed   = MotorSetSpeed,
    .SetDirection = MotorSetDirection
};

static MotorDevice *gMotors[MOTOR_NUM] = {
    &gLFMotor, &gRFMotor, &gLBMotor, &gRBMotor
};

MotorDevice *GetMotorDevice(unsigned char channel)
{
    if(channel > MOTOR_NUM) return NULL;
    channel = channel - 1;
    return gMotors[channel];
}

static int MotorInit(struct MotorDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    unsigned char channel = ptdev->channel - 1;
    GPIODevice *pCR = IODeviceFind((char*)&CRIOs[channel][0]);
    if(NULL == pCR)     return -ENODEV;
    if(ESUCCESS != pCR->Init(pCR))  return -EIO;
    
    GPIODevice *pCCR = IODeviceFind((char*)&CCRIOs[channel][0]);
    if(NULL == pCCR)     return -ENODEV;
    if(ESUCCESS != pCCR->Init(pCCR))  return -EIO;
    
    PWMDevice *ptPWM = PWMDeviceFind((char*)&PWMs[channel][0]);
    if(NULL == ptPWM)   return -ENODEV;
    if(ESUCCESS != ptPWM->Init(ptPWM))  return -EIO;
    int ret = ptPWM->Control(ptPWM, SET_PERIOD_VALUE, 1000000);
    if(ESUCCESS != ret)  return -EIO;
    ret = ptPWM->Start(ptPWM);
    if(ESUCCESS != ret)  return -EIO;
    
    if(ESUCCESS != MotorStop(ptdev))    return -EIO;
    if(ESUCCESS != MotorSetDirection(ptdev, MOTOR_CR))  return -EIO;
    
    return ESUCCESS;
}
    
static int MotorStart(struct MotorDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(ptdev->status == MOTOR_START)    return ESUCCESS;
    
    unsigned char channel = ptdev->channel - 1;
    GPIODevice *pCR = IODeviceFind((char*)&CRIOs[channel][0]);
    if(NULL == pCR)     return -ENODEV;
    
    GPIODevice *pCCR = IODeviceFind((char*)&CCRIOs[channel][0]);
    if(NULL == pCCR)     return -ENODEV;
    
    unsigned char dir = ptdev->dir;
    pCR->Write(pCR, dir);
    pCCR->Write(pCCR, !dir);
    ptdev->status = MOTOR_START;
    
    return ESUCCESS;
}

static int MotorStop(struct MotorDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    if(ptdev->status == MOTOR_STOP)    return ESUCCESS;
    
    unsigned char channel = ptdev->channel - 1;
    GPIODevice *pCR = IODeviceFind((char*)&CRIOs[channel][0]);
    if(NULL == pCR)     return -ENODEV;
    
    GPIODevice *pCCR = IODeviceFind((char*)&CCRIOs[channel][0]);
    if(NULL == pCCR)     return -ENODEV;
    
    pCR->Write(pCR, 0);
    pCCR->Write(pCCR, 0);
    ptdev->status = MOTOR_STOP;
    
    return ESUCCESS;
}

static int MotorSetSpeed(struct MotorDev *ptdev, unsigned int speed)
{
    if(NULL == ptdev)   return -EINVAL;
    if(speed > 100)     return -EINVAL;
    
    unsigned char channel = ptdev->channel - 1;
    PWMDevice *ptPWM = PWMDeviceFind((char*)&PWMs[channel][0]);
    if(NULL == ptPWM)     return -ENODEV;
    if(speed == 0)
    {
        return MotorStop(ptdev);
    }
    unsigned int pulse = speed * 1000000 / 100;
    int ret = ptPWM->Control(ptPWM, SET_PULSE_VALUE, pulse);
    if(ESUCCESS != ret) return ret;
    if(ptdev->status != MOTOR_START)
    {
        int ret = MotorStart(ptdev);
        if(ESUCCESS != ret) return ret;
    }
    
    return ESUCCESS;
}

static int MotorSetDirection(struct MotorDev *ptdev, unsigned int set_dir)
{
    if(NULL == ptdev)   return -EINVAL;
    if(MOTOR_CR!=set_dir && MOTOR_CCR!=set_dir) return -EINVAL;
    
    unsigned char channel = ptdev->channel - 1;
    GPIODevice *pCR = IODeviceFind((char*)&CRIOs[channel][0]);
    if(NULL == pCR)     return -ENODEV;
    
    GPIODevice *pCCR = IODeviceFind((char*)&CCRIOs[channel][0]);
    if(NULL == pCCR)     return -ENODEV;
    
    pCR->Write(pCR, set_dir);
    pCCR->Write(pCCR, !set_dir);
    ptdev->dir = set_dir;
    
    return ESUCCESS;
}
