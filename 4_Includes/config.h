#ifndef __CONFIG_H
#define __CONFIG_H

/* 管理设备对象是否使用 */
#define USE_GPIO_DEVICE     (1)
#define USE_UART_DEVICE     (1)
#define USE_SPI_DEVICE      (1)
#define USE_I2C_DEVICE      (1)
#define USE_TIMER_DEVICE    (1)
#define USE_PWM_DEVICE      (1)

/* 管理模块设备对象是否使用 */
#define USE_W25QX_MODULE    (1)
#define USE_DISPLAY_MODULE  (1)
#define USE_AT24CX_MODULE   (1)
#define USE_MOTOR_MODULE    (1)
#define USE_SERVO_MODULE    (1)
#define USE_IRDA_MODULE     (1)

/* 管理自定义库是否使用 */
#define USE_PRINTF_LIB      (1)
#define USE_RINGBUFFER_LIB  (1)
#define USE_DELAY_LIB       (1)
#define USE_FONT_LIB        (1)


#endif /* __CONFIG_H */
