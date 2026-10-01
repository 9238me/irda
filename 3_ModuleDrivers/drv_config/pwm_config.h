#ifndef __PWM_CONFIG_H
#define __PWM_CONFIG_H

#define TIM1_PWM1           \
{                           \
    .name = "TIM1 PWM1",    \
    .port = TIM1,           \
    .channel = 1,           \
    .status = 0,            \
    .Init = PWMDrvInit,     \
    .Start = PWMDrvStart,   \
    .Stop = PWMDrvStop,     \
    .GetCount = NULL,       \
    .Control = PWMDrvControl,   \
    .next = NULL,           \
}

#define TIM3_PWM3           \
{                           \
    .name = "TIM3 PWM3",    \
    .port = TIM3,           \
    .channel = 3,           \
    .status = 0,            \
    .Init = PWMDrvInit,     \
    .Start = PWMDrvStart,   \
    .Stop = PWMDrvStop,     \
    .GetCount = NULL,       \
    .Control = PWMDrvControl,   \
    .next = NULL,           \
}

#define TIM8_PWM1           \
{                           \
    .name = "TIM8 PWM1",    \
    .port = TIM8,           \
    .channel = 1,           \
    .status = 0,            \
    .Init = PWMDrvInit,     \
    .Start = PWMDrvStart,   \
    .Stop = PWMDrvStop,     \
    .GetCount = NULL,       \
    .Control = PWMDrvControl,   \
    .next = NULL,           \
}

#define TIM8_PWM2           \
{                           \
    .name = "TIM8 PWM2",    \
    .port = TIM8,           \
    .channel = 2,           \
    .status = 0,            \
    .Init = PWMDrvInit,     \
    .Start = PWMDrvStart,   \
    .Stop = PWMDrvStop,     \
    .GetCount = NULL,       \
    .Control = PWMDrvControl,   \
    .next = NULL,           \
}

#define TIM8_PWM3           \
{                           \
    .name = "TIM8 PWM3",    \
    .port = TIM8,           \
    .channel = 3,           \
    .status = 0,            \
    .Init = PWMDrvInit,     \
    .Start = PWMDrvStart,   \
    .Stop = PWMDrvStop,     \
    .GetCount = NULL,       \
    .Control = PWMDrvControl,   \
    .next = NULL,           \
}

#define TIM8_PWM4           \
{                           \
    .name = "TIM8 PWM4",    \
    .port = TIM8,           \
    .channel = 4,           \
    .status = 0,            \
    .Init = PWMDrvInit,     \
    .Start = PWMDrvStart,   \
    .Stop = PWMDrvStop,     \
    .GetCount = NULL,       \
    .Control = PWMDrvControl,   \
    .next = NULL,           \
}

#endif /* __PWM_CONFIG_H */
