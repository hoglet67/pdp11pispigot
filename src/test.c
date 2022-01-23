#include "tube.h"

void outc(char c);
void outhex(uint8_t i);
void outhex32(uint32_t i);
void outnl();
void outshort(short i);
void outlong(long i);
void outstr(char *c);
void pass();
void fail();

#define S1 12345
#define S2 1000

#define L1 1234567890L
#define L2 10000L

int program() {

   short s1 = S1;
   short s2 = S2;
   short sr;

   unsigned short us1 = S1;
   unsigned short us2 = S2;
   unsigned short usr;

   long l1 = L1;
   long l2 = L2;
   long lr;

   unsigned long ul1 = L1;
   unsigned long ul2 = L2;
   unsigned long ulr;

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

   outstr("Signed short\r\n");

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

   sr = s1 % s2;
   outshort(s1);
   outc('%');
   outshort(s2);
   outc('=');
   outshort(sr);
   if (sr == (S1%S2)) {
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
   if (usr == (S1/S2)) {
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
   if (usr == (S1%S2)) {
      pass();
   } else {
      fail();
   }

   outstr("signed long\r\n");

   lr = l1 / l2;
   outlong(l1);
   outc('/');
   outlong(l2);
   outc('=');
   outlong(lr);
   if (lr == (L1/L2)) {
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
   if (lr == (L1%L2)) {
      pass();
   } else {
      fail();
   }

   outstr("unsigned long\r\n");

   ulr = ul1 / ul2;
   outlong(ul1);
   outc('/');
   outlong(ul2);
   outc('=');
   outlong(ulr);
   if (ulr == (L1/L2)) {
      pass();
   } else {
      fail();
   }

   ulr = ul1 % ul2;
   outlong(ul1);
   outc('%');
   outlong(ul2);
   outc('=');
   outlong(ulr);
   if (ulr == (L1%L2)) {
      pass();
   } else {
      fail();
   }
}

 void pass() {
   outstr(" pass\r\n");
}

void fail() {
   outstr(" fail\r\n");
}

void outshort(short i) {
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

void outhex(uint8_t i) {
   i &= 15;
   if (i > 9) {
      outc(i + ('A' - 10));
   } else {
      outc(i + '0');
   }
}

void outhex32(uint32_t i) {
   int d;
   for (d = 0; d < 8; d++) {
      // Note: this uses ASHC which is a signed shift
      outhex(i >> 28);
      i <<= 4;
   }
}

void outlong(long i) {
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

void outstr(char *c) {
   while (*c) {
      outc(*c++);
   }
}

void outnl() {
   outc(10);
   outc(13);
}
