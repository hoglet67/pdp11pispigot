//#define LINUX

#ifdef LINUX
#include <stdio.h>
#endif

#define NDIGITS 1000

#define N (7*NDIGITS/2)

#define ARRAY 0x1FFE

void outnl();
void outc(char c);
void print(short i);

#ifdef LINUX
short r[N + 1];
int main() {
#else
int program() {
   short *r = (short *)ARRAY;
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
      print(c + d/10000);
      c = d%10000;
   }
   outnl();
}

void print(short i) {
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

void outnl() {
   outc(10);
   outc(13);
}

void outc(char c) {
#ifdef LINUX
   putchar(c);
   fflush(stdout);
#else
   asm("mov %0, r0" :  : "r" (c) : "r0");
   asm("emt 4");
#endif
}
