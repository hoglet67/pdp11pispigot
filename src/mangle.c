#include <stdio.h>
#include <stdint.h>

// The purpose of this tool is to remove the first 0x100 code bytes
// from the a.out file, so that everything lines up correctly when
// it's loaded to 0x100 by the PDP-11 Client ROM.

#define HDR_SIZE     0x10
#define PAD_SIZE    0x100
#define HDR_MAGIC   0x107
#define MAX_SIZE  0x10000

uint8_t buffer[MAX_SIZE];

uint16_t read_word(uint8_t *ptr) {
   return *ptr + (*(ptr+1) << 8);
}

void write_word(uint8_t *ptr, uint16_t data) {
   *ptr++ = data & 0xff;
   *ptr++ = (data >> 8) & 0xff;
}

int main(int argc, char *argv[]) {

   if (argc != 3) {
      fprintf(stderr, "usage: %s <infile> <outfile>\n", argv[0]);
      return 1;
   }

   char *infile = argv[1];
   char *outfile = argv[2];

   FILE *fp = fopen(infile, "rb");

   if (!fp) {
      fprintf(stderr, "%s: %s: No such file\n", argv[0], infile);
      return 1;
   }

   int size = fread(buffer, 1, sizeof(buffer), fp);
   fclose(fp);

   if (size < HDR_SIZE || read_word(buffer) != HDR_MAGIC) {
      fprintf(stderr, "%s: %s: Not an a.out file\n", argv[0], infile);
      return 1;
   }

   int tsize = read_word(buffer + 2);
   int dsize = read_word(buffer + 4);

   if (size != HDR_SIZE + tsize + dsize) {
      fprintf(stderr, "%s: %s: Inconsistent length fields; is the a.out stripped?\n", argv[0], infile);
      return 1;
   }

   if (tsize < PAD_SIZE) {
      fprintf(stderr, "%s: %s: File too short to be padded\n", argv[0], infile);
      return 1;
   }

   // Eliminate the padding
   tsize -= PAD_SIZE;
   write_word(buffer + 2, tsize);
   for (int i = 0; i < tsize + dsize; i++) {
      buffer[HDR_SIZE + i] = buffer[HDR_SIZE + i + PAD_SIZE];
   }

   // Set exec address to 0x100
   write_word(buffer + 10, 0x100);

   // Write the file
   fp = fopen(outfile, "wb");
   if (!fp) {
      fprintf(stderr, "%s: %s: Permission denied\n", argv[0], outfile);
      return 1;
   }
   fwrite(buffer, HDR_SIZE + tsize + dsize, 1, fp);
   fclose(fp);
}
