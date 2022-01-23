typedef unsigned char   uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

void outc(char c);
void outhex(uint8_t i);
void outhex32(uint32_t i);
void outnl();

unsigned long udivmodsi4(unsigned long num, unsigned long den, int modwanted);

#ifdef LINUX
int main() {
#else
int program() {
#endif
   uint32_t a = 0x12345678;
   uint32_t b = 0x100;
   uint32_t c = udivmodsi4(a, b, 0);
   uint32_t d = udivmodsi4(a, b, 1);
   outhex32(c);
   outnl();
   outhex32(d);
   outnl();
}

unsigned long udivmodsi4(unsigned long num, unsigned long den, int modwanted)
{
  unsigned long bit = 1;
  unsigned long res = 0;

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

void outnl() {
   outc(10);
   outc(13);
}

#ifdef LINUX
void outc(char c) {
   putchar(c);
}
#else
void outc(char c) {
   *((char *)(0xfff2)) = c;
}
#endif
