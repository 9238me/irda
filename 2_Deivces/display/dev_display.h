#ifndef __DEV_DISPLAY_H
#define __DEV_DISPLAY_H

#define USE_ST7789_DISPLAY  (1)

typedef struct DisplayDevice{
    char *name;
    void *FBBase; /* CPU能直接读写的显存 */
    unsigned short wXres;    /* X方向分辨率 */
    unsigned short wYres;    /* Y方向分辨率 */
    unsigned short wBpp;     /* 每个像素使用多少个像素 */
    unsigned int   dwSize;
    int (*Init)(struct DisplayDevice *ptDev);   /* 硬件初始化 */
    int (*DisplayON)(struct DisplayDevice *ptDev);   /* 开启显示 */
    int (*DisplayOFF)(struct DisplayDevice *ptDev);   /* 关闭显示 */
    int (*SetDisplayWindow)(struct DisplayDevice* ptDev, \
                            unsigned short wXs, unsigned short wYs, \
                            unsigned short wXe, unsigned short wYe);
    /* 设置FBBase中的数据, 把(iX,iY)的像素设置为颜色dwColor
     * dwColor的格式:0x00RRGGBB
     */
    int (*SetPixel)(struct DisplayDevice *ptDev, \
                    unsigned short wX, unsigned short wY, \
                    unsigned short wColor);
    int (*Flush)(struct DisplayDevice *ptDev); /* 把FBBase的数据刷到LCD的显存里 */
}DisplayDevice;

#ifdef USE_ST7789_DISPLAY
    #include "dev_st7789v.h"
    DisplayDevice *GetST7789Device(void);
#endif /* USE_ST7789_DISPLAY */

#endif /* __DEV_DISPLAY_H */
