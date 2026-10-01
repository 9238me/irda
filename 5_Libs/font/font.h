#ifndef __FONT_LIB_H
#define __FONT_LIB_H

unsigned char *GetASCIIBuffer(const char c);
void ShowStringEN(const char *str, unsigned short x, unsigned short y, unsigned short color);
void ShowChineseHalf(unsigned char *buf, unsigned short x, unsigned short y, unsigned short color);

#endif /* __FONT_LIB_H */
