#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// The purpose of this tool is to remove the first 0x100 code bytes
// from the a.out file, so that everything lines up correctly when
// it's loaded to 0x100 by the PDP-11 Client ROM.

#define HDR_SIZE     0x10
#define PAD_SIZE    0x100
#define HDR_MAGIC   0x107
#define MAX_SIZE  0x10000

#define V7_SYM_SIZE     8

uint8_t buffer[MAX_SIZE];

// Old V7 Symbol Table Format
//
// An array of these structures

typedef struct {
   char      n_name[V7_SYM_SIZE]; /* symbol name */
   uint16_t  n_type;              /* type flag */
   uint16_t  n_value;             /* value */
} old_nlist_type;

// New V10 Symbol Table Format
//
// An array of these structures followed by a string table
//

typedef struct {
   uint16_t n_pad;   /* padding */
   uint16_t n_str ;  /* index into file string table */
   uint16_t n_type;  /* type flag; see below */
   uint16_t n_value; /* value of this symbol (or struct offset) */
} new_nlist_type;

uint16_t read_word(uint8_t *ptr) {
   return *ptr + (*(ptr+1) << 8);
}

void write_word(uint8_t *ptr, uint16_t data) {
   *ptr++ = data & 0xff;
   *ptr++ = (data >> 8) & 0xff;
}

int main(int argc, char *argv[]) {
   int usage = 0;
   int depad = 0;

   if (argc == 4) {
      if (strcmp(argv[3], "-d")) {
         usage = 1;
      } else {
         depad = 1;
      }
   } else if (argc < 3 || argc > 4) {
      usage = 1;
   }

   if (usage) {
      fprintf(stderr, "usage: %s <infile> <outfile> [ -d ]\n", argv[0]);
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

   int tsize = read_word(buffer + 2);     // text size
   int dsize = read_word(buffer + 4);     // data size
   int usize = read_word(buffer + 6);     // uninitialized data size
   int ssize = read_word(buffer + 8);     // symbol table size
   int rflag = read_word(buffer + 14);    // relocation present flag
   int rsize = rflag ? 0 : tsize + dsize; // relocation info size

   if (size != HDR_SIZE + tsize + dsize + rsize + ssize) {
      fprintf(stderr, "%s: %s: Inconsistent length fields; is the a.out stripped?\n", argv[0], infile);
      return 1;
   }

   if (tsize < PAD_SIZE) {
      fprintf(stderr, "%s: %s: File too short to be padded\n", argv[0], infile);
      return 1;
   }

   // Reformat the V7 symbol table so it's compatible with the pdp11-aout-xxx tools
   int num_syms = ssize / sizeof(old_nlist_type);

   old_nlist_type *old_nlist = (old_nlist_type *)(buffer + HDR_SIZE + tsize + dsize + rsize);
   new_nlist_type *new_nlist = (new_nlist_type *)calloc(num_syms, sizeof(new_nlist_type));

   char *string_table = (char *) malloc(num_syms * (V7_SYM_SIZE + 1));
   char *ptr = string_table;
   for (int i = 0; i < num_syms ; i++) {
      // Copy the entry and type
      new_nlist[i].n_value = old_nlist[i].n_value;
      new_nlist[i].n_type  = old_nlist[i].n_type;

      // Copy the string
      old_nlist[i].n_type  = 0; // Ensures the old string is terminated
      strcpy(ptr, old_nlist[i].n_name);
      new_nlist[i].n_pad = 0;
      new_nlist[i].n_str = 4 + ptr - string_table;

#ifdef DEBUG
      printf("str=%04x, addr=%04x type=%04x name=%s\n",
             new_nlist[i].n_str,
             new_nlist[i].n_value,
             new_nlist[i].n_type,
             ptr);
#endif

      ptr += strlen(ptr) + 1;
   }

   uint16_t pad = 0;
   uint16_t string_table_len = 4 + ptr - string_table;

   // Eliminate the padding
   // TODO - this doesn't currently work if relocation info present
   if (depad) {
      tsize -= PAD_SIZE;
      write_word(buffer + 2, tsize);
      for (int i = 0; i < tsize + dsize; i++) {
         buffer[HDR_SIZE + i] = buffer[HDR_SIZE + i + PAD_SIZE];
      }
   }

   // Update the length of the symbol table
   write_word(buffer + 8, num_syms * sizeof(new_nlist_type));

   // Set exec address to 0x100
   write_word(buffer + 10, 0x100);

   // Mark the Relocation Info as reloved
   write_word(buffer + 14, 0x0001);

   // Write the file
   fp = fopen(outfile, "wb");
   if (!fp) {
      fprintf(stderr, "%s: %s: Permission denied\n", argv[0], outfile);
      return 1;
   }

   // Write the header, text and data sections
   fwrite(buffer, HDR_SIZE + tsize + dsize, 1, fp);

   // Write the new symbol table
   fwrite(new_nlist, num_syms * sizeof(new_nlist_type), 1, fp);

   // Write the lenth of the string table
   fwrite(&pad             , sizeof(uint16_t), 1, fp);
   fwrite(&string_table_len, sizeof(uint16_t), 1, fp);

   // Write the string table
   fwrite(string_table, string_table_len, 1, fp);

   fclose(fp);
}
