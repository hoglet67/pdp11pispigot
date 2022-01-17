#include <stdint-gcc.h>

void outc(char c);
void outshort(short i);
void outlong(long i);
void outhex32(uint32_t pp);
void outstr(char *c);
void outnl();

void pass();
void fail();

#define S1 12345
#define S2 1000


int program() {
   outhex32(0x12345678);
   outnl();
   outshort(0);
   outnl();
   outshort(12345);
   outnl();
   outshort(-12345);
   outnl();
   outlong(0L);
   outnl();
   outlong(1234567890L);
   outnl();
   outlong(-1234567890L);
   outnl();

   volatile short s1 = S1;
   volatile short s2 = S2;
   volatile short sr;

   sr = s1 / s2;
   outshort(s1);
   outc('/');
   outshort(s2);
   outc('=');
   outshort(sr);
   if (sr == (S1/S2)) {
      pass();
   } else {
      fail();
   }
   while (1);
}

static const short ps[] = {
   10000,
   1000,
   100,
   10,
   1,
   0};

void outshort(short i) {
   if (i < 0) {
      outc('-');
      i = -i;
   }
   const short *pp = &ps[0];
   while (*pp) {
      char digit = '0';
      while (i >= *pp) {
         i = i - *pp;
         digit++;
      }
      outc(digit);
      pp++;
   };
}

static const long pl[]
= {1000000000L,
   100000000L,
   10000000L,
   1000000L,
   100000L,
   10000L,
   1000L,
   100L,
   10L,
   1L,
   0L};

void pass() {
   outstr(" pass\r\n");
}

void fail() {
   outstr(" fail\r\n");
}

void outhex(uint8_t i) {
   if (i > 15) {
      outc('.');
   } else if (i > 9) {
      outc(i + ('A' - 10));
   } else {
      outc(i + '0');
   }
}

void outhex32(uint32_t i) {
   for (uint8_t d = 0; d < 8; d++) {
      outhex(i >> 28);
      i <<= 4;
   }
}

void outlong(long i) {
   if (i < 0) {
      outc('-');
      i = -i;
   }
   const uint32_t *pp = &pl[0];
   while (*pp) {
      outnl();
      outhex32(*pp);
      outnl();
      char digit = '0';
      while (i >= *pp) {
         i = i - *pp;
         digit++;
      }
      outc(digit);
      pp++;
   };
}

void outstr(char *c) {
   while (*c) {
      outc(*c++);
   }
}

void outnl() {
   outc(10);
   outc(13);
}

void outc(char c) {
   asm("mov 4(r5), r0");
   asm("emt 4");
}
