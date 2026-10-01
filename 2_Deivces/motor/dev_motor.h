#ifndef __DEV_MOTOR_H
#define __DEV_MOTOR_H

#define MOTOR_NUM           (4)
#define MOTOR_START         (1)
#define MOTOR_STOP          (0)
#define MOTOR_CR            (1)
#define MOTOR_CCR           (0)

typedef struct MotorDev{
    unsigned char channel;
    unsigned char status;
    unsigned char dir;
    int (*Init)(struct MotorDev *ptdev);
    int (*Start)(struct MotorDev *ptdev);
    int (*Stop)(struct MotorDev *ptdev);
    int (*SetSpeed)(struct MotorDev *ptdev, unsigned int speed);
    int (*SetDirection)(struct MotorDev *ptdev, unsigned int set_dir);
}MotorDevice;

MotorDevice *GetMotorDevice(unsigned char channel);

#endif /* __DEV_MOTOR_H */
