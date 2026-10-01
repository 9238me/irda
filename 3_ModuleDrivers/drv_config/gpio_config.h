#ifndef __GPIO_CONFIG_H
#define __GPIO_CONFIG_H

#define PB1             \
{                       \
    .name   = "PB1",    \
    .port   = GPIOB,    \
    .pin    = GPIO_PIN_1,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PC2             \
{                       \
    .name   = "PC2",    \
    .port   = GPIOC,    \
    .pin    = GPIO_PIN_2,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PC3             \
{                       \
    .name   = "PC3",    \
    .port   = GPIOC,    \
    .pin    = GPIO_PIN_3,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PC4             \
{                       \
    .name   = "PC4",    \
    .port   = GPIOC,    \
    .pin    = GPIO_PIN_4,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PC5             \
{                       \
    .name   = "PC5",    \
    .port   = GPIOC,    \
    .pin    = GPIO_PIN_5,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PF6             \
{                       \
    .name   = "PF6",    \
    .port   = GPIOF,    \
    .pin    = GPIO_PIN_6,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PF7             \
{                       \
    .name   = "PF7",    \
    .port   = GPIOF,    \
    .pin    = GPIO_PIN_7,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead , \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PF8             \
{                       \
    .name   = "PF8",    \
    .port   = GPIOF,    \
    .pin    = GPIO_PIN_8,   \
    .Init   = GPIODrvInit,  \
    .Write  = NULL,     \
    .Read   = GPIODrvRead,  \
    .IrqHandler = NULL,     \
    .next   = NULL      \
}

#define PE2          \
{                   \
    .name   = "PE2", \
    .port   = GPIOE,\
    .pin    = GPIO_PIN_2,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PE3          \
{                   \
    .name   = "PE3", \
    .port   = GPIOE,\
    .pin    = GPIO_PIN_3,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PE4          \
{                   \
    .name   = "PE4", \
    .port   = GPIOE,\
    .pin    = GPIO_PIN_4,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PE5          \
{                   \
    .name   = "PE5", \
    .port   = GPIOE,\
    .pin    = GPIO_PIN_5,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG6          \
{                   \
    .name   = "PG6", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_6,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG7          \
{                   \
    .name   = "PG7", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_7,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG8          \
{                   \
    .name   = "PG8", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_8,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG9          \
{                   \
    .name   = "PG9", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_9,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG11          \
{                   \
    .name   = "PG11", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_11,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG12          \
{                   \
    .name   = "PG12", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_12,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG13          \
{                   \
    .name   = "PG13", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_13,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define PG14          \
{                   \
    .name   = "PG14", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_14,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define W25Q64CS          \
{                   \
    .name   = "W25Q64 CS", \
    .port   = GPIOA,\
    .pin    = GPIO_PIN_15,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define LCDCS          \
{                   \
    .name   = "LCD CS", \
    .port   = GPIOD,\
    .pin    = GPIO_PIN_3,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define LCDDCX          \
{                   \
    .name   = "LCD DCX", \
    .port   = GPIOF,\
    .pin    = GPIO_PIN_9,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define LCDRST          \
{                   \
    .name   = "LCD RESET", \
    .port   = GPIOG,\
    .pin    = GPIO_PIN_15,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define LCDBACK          \
{                   \
    .name   = "LCD BACK", \
    .port   = GPIOE,\
    .pin    = GPIO_PIN_6,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = NULL,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define D1          \
{                   \
    .name   = "D1", \
    .port   = GPIOB,\
    .pin    = GPIO_PIN_12,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define D2          \
{                   \
    .name   = "D2", \
    .port   = GPIOB,\
    .pin    = GPIO_PIN_13,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define D3          \
{                   \
    .name   = "D3", \
    .port   = GPIOB,\
    .pin    = GPIO_PIN_14,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define D4          \
{                   \
    .name   = "D4", \
    .port   = GPIOB,\
    .pin    = GPIO_PIN_15,  \
    .Init   = GPIODrvInit,   \
    .Write  = GPIODrvWrite,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define K1          \
{                   \
    .name   = "K1", \
    .port   = GPIOA,\
    .pin    = GPIO_PIN_0,  \
    .Init   = GPIODrvInit,   \
    .Write  = NULL,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define K2          \
{                   \
    .name   = "K2", \
    .port   = GPIOA,\
    .pin    = GPIO_PIN_1,  \
    .Init   = GPIODrvInit,   \
    .Write  = NULL,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define K3          \
{                   \
    .name   = "K3", \
    .port   = GPIOA,\
    .pin    = GPIO_PIN_2,  \
    .Init   = GPIODrvInit,   \
    .Write  = NULL,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}

#define K4          \
{                   \
    .name   = "K4", \
    .port   = GPIOA,\
    .pin    = GPIO_PIN_3,  \
    .Init   = GPIODrvInit,   \
    .Write  = NULL,  \
    .Read   = GPIODrvRead,   \
    .IrqHandler = NULL,     \
    .next   = NULL  \
}


#endif /* __GPIO_CONFIG_H */
