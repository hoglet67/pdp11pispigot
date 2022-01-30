#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Uncomment this to truncate the columns during the calculation
//#define TRUNCATE_COLUMNS

typedef uint64_t thing1;
typedef uint64_t thing2;

short extra = 1;

thing1 num_cols(thing1 nchunks, thing1 chunk) {
   return (nchunks * chunk) * 10 / 3 + extra;
}

int main(int argc, char *argv[]) {
   short chunk = 1;
   thing1 ndigits = 800;
   thing1 i, k, b, c, e, l, n, q, x;
   thing1 *r;
   thing2 d;
   char format[10];
   thing1 nines;
   thing1 predigit;
   thing1 nchunks;

   if (argc > 1) {
      ndigits = atoi(argv[1]);
   }
   if (argc > 2) {
      chunk = atoi(argv[2]);
   }
   if (argc > 3) {
      extra = atoi(argv[3]);
   }
   e = 1;
   for (i = 0; i < chunk; i++) {
      e *= 10;
   }
   // Round up to an integer multiple of chunk size
   if (ndigits % chunk) {
      ndigits = ndigits + chunk - (ndigits % chunk);
   }

   // Calculate the number of chunks
   nchunks = ndigits / chunk;

   // Calculate the number of columns
   n = num_cols(nchunks, chunk);

   sprintf(format, "%%.%dd", chunk);

#ifdef DEBUG
   printf("format = %s, ndigits = %d; chunk = %d; extra = %d; e = %d; n = %d\n",
          format, ndigits, chunk, extra, e, n);
#endif

   // Allocate memory
   r = (thing1 *)calloc(n + 1, sizeof(thing1));

   // Initialize
   q = 2 * e / 10;
   for (i = 1; i <= n; i++) {
      r[i] = q;
   }
   nines = 0;
   predigit = 0;

   // Run the spigot
   c = 0;
   for (k = nchunks; k > 0; k -= 1) {
      d = 0;
#ifdef TRUNCATE_COLUMNS
      i = num_cols(k, chunk);
#else
      i = num_cols(nchunks, chunk);
#endif
      for(;;) {
         d += r[i]*e;
         b = i*2 - 1;
         r[i] = d%b;
         d /= b;
         i--;
         if (i == 0) break;
         d *= i;
      }
      x = c + d/e;
      c = d%e;
#ifdef XDEBUG
      if (x > e) {
         for (int z = 0; z < chunk; z++) {
            putchar('#');
         }
      } else if (x == e) {
         for (int z = 0; z < chunk; z++) {
            putchar('*');
         }
      } else {
         printf(format, x);
      }
#else
      if (x == e - 1) {
         nines++;
      } else if (x == e) {
         printf(format, predigit + 1);
         for (i = 0; i < nines; i++) {
            printf(format, 0);
         }
         predigit = 0;
         nines = 0;
      } else {
         // Suppress the first chunk
         if (k < nchunks) {
            printf(format, predigit);
         }
         for (i = 0; i < nines; i++) {
            printf(format, e - 1);
         }
         predigit = x;
         nines = 0;
      }
#endif
   }
   printf(format, predigit);
   for (i = 0; i < nines; i++) {
      printf(format, e - 1);
   }
   printf("\n");
}
