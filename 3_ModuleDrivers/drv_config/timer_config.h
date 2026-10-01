#ifndef __TIMER_CONFIG_H
#define __TIMER_CONFIG_H

#define SYSTICKDEV      \
{                       \
    .name = "Systick",  \
    .port = SysTick,    \
    .channel = 0,       \
    .status = 0,        \
    .Init = TimerDrvInit,   \
    .Start = TimerDrvStart, \
    .Stop = TimerDrvStop,   \
    .GetCount = TimerDrvGetCount,   \
    .Control = TimerDrvControl,     \
    .IrqHandler = NULL,      \
    .next = NULL,       \
}

#define TIM2DEV         \
{                       \
    .name = "TIM2",     \
    .port = TIM2,       \
    .channel = 0,       \
    .status = 0,        \
    .Init = TimerDrvInit,   \
    .Start = TimerDrvStart, \
    .Stop = TimerDrvStop,   \
    .GetCount = TimerDrvGetCount,   \
    .Control = TimerDrvControl,     \
    .IrqHandler = NULL,      \
    .next = NULL,       \
}

#define TIM4DEV         \
{                       \
    .name = "TIM4",     \
    .port = TIM4,       \
    .channel = 0,       \
    .status = 0,        \
    .Init = TimerDrvInit,   \
    .Start = TimerDrvStart, \
    .Stop = TimerDrvStop,   \
    .GetCount = TimerDrvGetCount,   \
    .Control = TimerDrvControl,     \
    .IrqHandler = NULL,      \
    .next = NULL,       \
}

#define TIM10DEV         \
{                       \
    .name = "TIM10",     \
    .port = TIM10,       \
    .channel = 0,       \
    .status = 0,        \
    .Init = TimerDrvInit,   \
    .Start = TimerDrvStart, \
    .Stop = TimerDrvStop,   \
    .GetCount = TimerDrvGetCount,   \
    .Control = TimerDrvControl,     \
    .IrqHandler = NULL,      \
    .next = NULL,       \
}

#endif /* __TIMER_CONFIG_H */
