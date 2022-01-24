#include "tube.h"

#define NDIGITS 1000

#define N (7*NDIGITS/2)


outlong();
outstr();
outnl();
print();

write_time(time)
long time;
{
   char block[5];
   int i;
   for (i = 0; i < sizeof(block); i++) {
      block[i] = time & 0xff;
      time >>= 8;
   }
   osword(WRITE_TIME, block);
}

read_time() {
   char block[5];
   short i;
   long time = 0;
   osword(READ_TIME, block);
   for (i = sizeof(block) - 1; i >= 0; i--) {
      time = (time << 8) + block[i];
   }
   outlong(time);
   outstr(" centi-seconds");
   outnl();
}


#ifdef LINUX
outc(c)
char c;
{
   putchar(c);
}
main() {
short r[N + 1];
#else
program() {
short *r = (short *)0x1FFE;
#endif

   short i, k, b, c;
   long d;
   write_time(0);
   read_time();
   c = 0;
   for (i = 1; i <= N; i++)
      r[i] = 2000;
   for (k = N; k > 0; k -= 14) {
      d = 0;
      i = k;
      for(;;) {
         d += r[i]*10000L;
         b = i*2 - 1;
         r[i] = d%b;
         d /= b;
         i--;
         if (i == 0) break;
         d *= i;
      }
#ifdef DEBUG
      printf("\nK = %d\n", k);
      for (int x = 1; x <= N; x++) {
         printf("%04x %04x\n", 0x0FFE + 2 * x, r[x]);
      }
#endif
      i = d/10000 + c;
      c = d%10000;
      print(i);
   }
   outnl();
   read_time();
}

print(i)
   short i;
{
   short p = 1000;
   do {
      char digit = '0';
      while (i >= p) {
         i = i - p;
         digit++;
      }
      p /= 10;
      outc(digit);
   } while (p);
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
