#include "tube.h"

#define NDIGITS 1000

#define N (7*NDIGITS/2)


outnl();
print();


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

outnl() {
   outc(10);
   outc(13);
}
