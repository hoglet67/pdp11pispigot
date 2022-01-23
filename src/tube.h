#ifndef _INCLUDE_TUBE_H
#define _INCLUDE_TUBE_H

typedef unsigned char   uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

#ifdef LINUX

#include <stdio.h>

inline void outc(char c) {
   putchar(c);
}

inline int main() {
   program();
}

#else // LINUX

extern void outc(char c);

#endif // LINUX

#endif // _INCLUDE_TUBE_H
