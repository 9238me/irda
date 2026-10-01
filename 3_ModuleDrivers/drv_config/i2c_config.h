#ifndef __I2C_CONFIG_H
#define __I2C_CONFIG_H

#define I2C1Dev \
{       \
    .name = "I2C1", \
    .channel = 1,   \
    .own_addr = 0,  \
    .slave_addr = 0,    \
    .Init = I2CDrvInit, \
    .Write = I2CDrvWrite,   \
    .Read = I2CDrvRead, \
    .next   = NULL, \
}

#endif /* __I2C_CONFIG_H */
