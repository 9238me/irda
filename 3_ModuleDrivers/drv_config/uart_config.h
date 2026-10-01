#ifndef __UART_CONFIG_H
#define __UART_CONFIG_H

#define DGBUART     \
{                   \
    .name       = "Debug UART", \
    .channel    = 1,            \
    .Init       = UartDrvInit,  \
    .Write      = UartDrvWrite, \
    .Read       = UartDrvRead,  \
    .next       = NULL,         \
}

#endif /* __UART_CONFIG_H */
