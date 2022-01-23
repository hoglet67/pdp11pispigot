#include "tube.h"

outc();
outhex();
outhex32();
outnl();
outshort();
outlong();
outstr();
pass();
fail();

#define S1 12345
#define S2  1000
#define SDIV  12
#define SMOD 345

#define L1 1234567890L
#define L2      10000L
#define LDIV   123456L
#define LMOD     7890L

program() {

   short s1 = S1;
   short s2 = S2;
   short sr;

   unsigned short us1 = S1;
   unsigned short us2 = S2;
   unsigned short usr;

   long l1 = L1;
   long l2 = L2;
   long lr;

   outstr("Basic tests\r\n");

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

   outstr("Short\r\n");

   sr = s1 / s2;
   outshort(s1);
   outc('/');
   outshort(s2);
   outc('=');
   outshort(sr);
   if (sr == SDIV) {
      pass();
   } else {
      fail();
   }

   sr = s1 % s2;
   outshort(s1);
   outc('%');
   outshort(s2);
   outc('=');
   outshort(sr);
   if (sr == SMOD) {
      pass();
   } else {
      fail();
   }

   outstr("Unsigned short\r\n");

   usr = us1 / us2;
   outshort(us1);
   outc('/');
   outshort(us2);
   outc('=');
   outshort(usr);
   if (usr == SDIV) {
      pass();
   } else {
      fail();
   }

   usr = us1 % us2;
   outshort(us1);
   outc('%');
   outshort(us2);
   outc('=');
   outshort(usr);
   if (usr == SMOD) {
      pass();
   } else {
      fail();
   }

   outstr("Long\r\n");

   lr = l1 / l2;
   outlong(l1);
   outc('/');
   outlong(l2);
   outc('=');
   outlong(lr);
   if (lr == LDIV) {
      pass();
   } else {
      fail();
   }

   lr = l1 % l2;
   outlong(l1);
   outc('%');
   outlong(l2);
   outc('=');
   outlong(lr);
   if (lr == LMOD) {
      pass();
   } else {
      fail();
   }

}

pass() {
   outstr(" pass\r\n");
}

fail() {
   outstr(" fail\r\n");
}

outshort(i)
short i;
{
   short p = 10000;
   if (i < 0) {
      outc('-');
      i = -i;
   }
   while (p) {
      char digit = '0';
      while (i >= p) {
         i = i - p;
         digit++;
      }
      outc(digit);
      p /= 10;
   };
}

outhex(i)
   char i;
{
   i &= 15;
   if (i > 9) {
      outc(i + ('A' - 10));
   } else {
      outc(i + '0');
   }
}

outhex32(i)
long i;
{
   int d;
   for (d = 0; d < 8; d++) {
      /* Note: this uses ASHC which is a signed shift */
      outhex(i >> 28);
      i <<= 4;
   }
}

outlong(i)
long i;
{
   long p = 1000000000L;
   if (i < 0) {
      outc('-');
      i = -i;
   }
   while (p) {
      char digit = '0';
      while (i >= p) {
         i = i - p;
         digit++;
      }
      outc(digit);
      p /= 10L;
   };
}

outstr(c)
char *c;
{
   while (*c) {
      outc(*c++);
   }
}

outnl() {
   outc(10);
   outc(13);
}
