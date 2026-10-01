#ifndef __DEVICES_H
#define __DEVICES_H

#include "config.h"

#if USE_GPIO_DEVICE
    #include "dev_gpio.h"
    #define USE_GPIO_DRIVER
#endif

#if USE_UART_DEVICE
    #include "dev_uart.h"
    #define USE_UART_DRIVER
#endif

#if USE_SPI_DEVICE
    #include "dev_spi.h"
    #define USE_SPI_DRIVER
#endif

#if USE_I2C_DEVICE
    #include "dev_i2c.h"
    #define USE_I2C_DRIVER
#endif

#if USE_TIMER_DEVICE
    #include "dev_timer.h"
    #define USE_TIMER_DRIVER
#endif

#if USE_PWM_DEVICE
    #include "dev_pwm.h"
    #define USE_PWM_DRIVER
#endif

#if USE_W25QX_MODULE
    #include "./w25q/dev_w25qx.h"
#endif

#if USE_DISPLAY_MODULE
    #include "./display/dev_display.h"
#endif

#if USE_AT24CX_MODULE
    #include "./at24c/dev_at24c.h"
#endif

#if USE_MOTOR_MODULE
    #include "./motor/dev_motor.h"
#endif

#if USE_SERVO_MODULE
    #include "./servo/dev_servo.h"
#endif

#if USE_IRDA_MODULE
    #include "./irda/dev_irda.h"
#endif

#endif /* __DEVICES_H */
