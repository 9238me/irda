#ifndef __DRIVERS_H
#define __DRIVERS_H

#include "devices.h"

#ifdef USE_GPIO_DRIVER
    #include "drv_gpio.h"
#endif /* USE_GPIO_DRIVER */

#ifdef USE_UART_DRIVER
    #include "drv_uart.h"
#endif /* USE_UART_DEVICE */

#ifdef USE_SPI_DRIVER
    #include "drv_spi.h"
#endif /* USE_SPI_DRIVER */

#ifdef USE_I2C_DRIVER
    #include "drv_i2c.h"
#endif /* USE_I2C_DRIVER */

#ifdef USE_TIMER_DRIVER
    #define USE_SYSTICK_TIMER   (1)
    #define USE_GENERAL_TIMER   (1)
    #include "drv_timer.h"
#endif /* USE_I2C_DRIVER */

#ifdef USE_PWM_DRIVER
    #include "drv_pwm.h"
#endif /* USE_I2C_DRIVER */

#endif /* __DRIVERS_H */
