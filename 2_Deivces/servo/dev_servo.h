#ifndef __DEV_SERVO_H
#define __DEV_SERVO_H

#define SERVO_MAX_ANGLE     (180)
#define SERVO_START         (1)
#define SERVO_STOP          (0)

typedef struct ServoDev{
    char *name;
    unsigned char status;
    int           angle;
    int (*Init)(struct ServoDev *ptdev);
    int (*Start)(struct ServoDev *ptdev);
    int (*Stop)(struct ServoDev *ptdev);
    int (*SetAngle)(struct ServoDev *ptdev, int angle);
}ServoDevice;

ServoDevice *GetServoDevice(void);

#endif /* __DEV_SERVO_H */
