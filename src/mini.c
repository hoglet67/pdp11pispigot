typedef unsigned char   uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

void outc(char c);
void outhex(uint8_t i);
void outhex16(uint16_t pp);
void outhex32(uint32_t pp);
void outstr(char *c);
void outnl();


int program() {
      outstr("Test 1\r\n");
      //outhex16(0x12345);
   //   outnl();
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

void outhex16(uint16_t i) {
   for (uint8_t d = 0; d < 4; d++) {
      outhex(i >> 12);
      i <<= 4;
   }
}

void outhex32(uint32_t i) {
   for (uint8_t d = 0; d < 8; d++) {
      outhex(i >> 28);
      i <<= 4;
   }
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
