#ifndef __SPI_CONFIG_H
#define __SPI_CONFIG_H

#define SPI1Dev \
{   \
    .name = "SPI1", \
    .channel = 1,   \
    .Init = SPIDrvInit, \
    .Write = SPIDrvWrite,   \
    .Read = SPIDrvRead, \
    .WriteRead = SPIDrvWriteRead,   \
    .next = NULL,   \
}

#endif /* __SPI_CONFIG_H */
