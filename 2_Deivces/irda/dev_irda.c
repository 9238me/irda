#include "devices.h"
#include "libs.h"
#include "errno.h"

static int IrdaInit(struct IrdaDev *ptdev);
static int IrdaRead(struct IrdaDev *ptdev);
static int IrdaDecode(struct IrdaDev *ptdev);

/* 20键遥控器码表 */
static const unsigned char KeyCode[20] = {
0x45, 0x47, 0x44, 0x40, 0x43, 
0x07, 0x15, 0x09, 0x16, 0x19, 
0x0D, 0x0C, 0x18, 0x5E, 0x08, 
0x1C, 0x5A, 0x42, 0x52, 0x4A
};
static const char KeyName[20][10] = {
"Open",   "Menu",    "Test",    "+", "Return", 
"Back",   "Suspend", "Forward", "0", "-", 
"Cancle", "1",       "2",       "3", "4", 
"5",      "6",       "7",       "8", "9"
};

static TimerDevice *pTimer = NULL;
static RingBuffer *pBuffer = NULL;
static IrdaDevice gIRDA = {
    .name = "NEC IRDA",
    .key  = NULL,
    .Init = IrdaInit,
    .Read = IrdaRead
};

IrdaDevice *GetIrdaDevice(void)
{
    return &gIRDA;
}

static void GetIrdaTrigerTime(void)
{
    if(pTimer == NULL)      return;
    if(pBuffer == NULL)     return;
    volatile unsigned int tick = pTimer->GetCount(pTimer);
    pBuffer->Write(pBuffer, (unsigned char *)&tick, sizeof(unsigned int));
}

static int IrdaInit(struct IrdaDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    GPIODevice *pIO = IODeviceFind("PB1");
    if(NULL == pIO)     return -ENODEV;
    pIO->IrqHandler = GetIrdaTrigerTime;
    int ret = pIO->Init(pIO);
    if(ESUCCESS != ret) return ret;
    
    pTimer = TimerDeviceFind("TIM10");
    if(NULL == pTimer)     return -ENODEV;
    ret = pTimer->Init(pTimer);
    if(ESUCCESS != ret) return ret;
    ret = pTimer->Control(pTimer, SET_PERIOD_VALUE, 10000);
    if(ESUCCESS != ret) return ret;
    ret = pTimer->Start(pTimer);
    if(ESUCCESS != ret) return ret;
    
    pBuffer = RingBufferNew(sizeof(unsigned int) << 8);
    if(NULL == pBuffer) return -ENOMEM;
    
    return ESUCCESS;
}

static int IrdaRead(struct IrdaDev *ptdev)
{
    if(NULL == ptdev)   return -EIO;
    int code = IrdaDecode(ptdev);
    if(code < 0)        return -EIO;
    for(unsigned char i=0; i<20; i++)
    {
        if(code == KeyCode[i])
        {
            ptdev->key = (char*)&KeyName[i][0];
            return ESUCCESS;
        }
    }
    return -EIO;
}

static int IrdaDecode(struct IrdaDev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    
    unsigned char pQ = 0;
    unsigned int duty = 0;
    unsigned int tick[2] = {0};
    unsigned char step = 0;
    unsigned int timeout = 1000;
    unsigned char i = 0;
    unsigned int value = 0;
    unsigned char cnt = 0;
    unsigned int tmp_buf[64] = {0};
    while(timeout)
    {
        timeout--;
        int ret= pBuffer->Read(pBuffer, (unsigned char*)&tick[pQ], sizeof(unsigned int));
        if(ret != sizeof(unsigned int))
        {
            mdelay(1);
            continue;
        }
        pQ++;
        if(pQ == 2)
        {
            duty = (tick[1] - tick[0]) * 10;
            tick[0] = tick[1];
            pQ = 1;
        }
        switch(step)
        {
            case 0:
            {
                if(duty >= 8500 && duty <= 9500)    // 引导码
                {
                    step++;
                }
                break;
            }
            case 1:
            {
                if(duty >= 4000 && duty <= 5000)    // 指令码
                {
                    step++;
                }
                else if(duty >= 2000 && duty <= 2500)   // 连发码
                {
                    step = 0;
                }
                else
                {
                    step = 0;
                }
                break;
            }
            case 2:
            {
                timeout = 1000;
                tmp_buf[i] = duty;
                i++;
                if(i == 64)
                {
                    i = 0;
                    step++;
                }
                break;
            }
            case 3:
            {
                for(unsigned char n=0; n<64; n+=2)
                {
                    if(tmp_buf[n]>=500 && tmp_buf[n] <= 600)
                    {
                        if(tmp_buf[n+1]>=500 && tmp_buf[n+1] <= 600)    // 数据0
                        {
                        }
                        else if(tmp_buf[n+1]>=1000 && tmp_buf[n+1] <= 2000) // 数据1
                        {
                            value = value + (1<<cnt);
                        }
                    }
                    cnt++;
                }
                step++;
                break;
            }
            case 4:
            {
                timeout = 1000;
                unsigned char code = (value>>16)&0xFF;
                step = 0;
                pQ = 0;
                return code;
            }
            default:break;
        }
    }
    return -EIO;
}
