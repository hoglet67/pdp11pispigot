#include "tube.h"

outhex();
outhex32();
outnl();
long udivmodsi4();

program() {
   long a = 0x12345678;
   long b = 0x100;
   long c = udivmodsi4(a, b, 0);
   long d = udivmodsi4(a, b, 1);
   outhex32(a);
   outnl();
   outhex32(b);
   outnl();
   outhex32(c);
   outnl();
   outhex32(d);
   outnl();
}

long udivmodsi4(num, den, modwanted)
     long num;
     long den;
     int modwanted;
{
  long bit = 1;
  long res = 0;

  while (den < num && bit && !(den & (1L<<31)))
    {
      den <<=1;
      bit <<=1;
    }
  while (bit)
    {
      if (num >= den)
	{
	  num -= den;
	  res |= bit;
	}
      bit >>=1;
      den >>=1;
    }
  if (modwanted) return num;
  return res;
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
      outhex((char) (i >> 28));
      i <<= 4;
   }
}

outnl() {
   outc(10);
   outc(13);
}
