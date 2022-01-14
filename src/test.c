void oswrch(char c);
void print(short i);
int startup() {
   volatile long d = 1000323;
   volatile short s = 1000;
   print(d/s);
   oswrch(10);
   oswrch(13);
   print(d%s);
   oswrch(10);
   oswrch(13);
   print(s);
   oswrch(10);
   oswrch(13);
   print(1234);
   oswrch(10);
   oswrch(13);
   print(9876);
   oswrch(10);
   oswrch(13);
   while (1);
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
      oswrch(digit);
   } while (p);
}

void oswrch(char c) {
   asm("mov 4(r5), r0");
   asm("emt 4");
}
