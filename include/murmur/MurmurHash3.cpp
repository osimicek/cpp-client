/**
 * MurmurHash3 implementation in C++, based on Austin Appleby's <a href=
 * "https://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp"
 * >original in C</a>
 *
 * Only implementing x64 version, because this should always be faster on 64 bit
 * native processors, even 64 bit being ran with a 32 bit OS; this should also
 * be as fast or faster than the x86 version on some modern 32 bit processors.
 *
 * @author Patrick McFarland
 * @see <a href="http://sites.google.com/site/murmurhash/">MurmurHash website</a>
 * @see <a href="http://en.wikipedia.org/wiki/MurmurHash">MurmurHash entry on Wikipedia</a>
 * @since 5.0
 */
#include <inttypes.h>
#include "MurmurHash3.h"


unsigned long int getblock(const char* key, int i) {
      return
           (((unsigned long int) key[i + 0] & 0x00000000000000FFL))
         | (((unsigned long int) key[i + 1] & 0x00000000000000FFL) << 8)
         | (((unsigned long int) key[i + 2] & 0x00000000000000FFL) << 16)
         | (((unsigned long int) key[i + 3] & 0x00000000000000FFL) << 24)
         | (((unsigned long int) key[i + 4] & 0x00000000000000FFL) << 32)
         | (((unsigned long int) key[i + 5] & 0x00000000000000FFL) << 40)
         | (((unsigned long int) key[i + 6] & 0x00000000000000FFL) << 48)
         | (((unsigned long int) key[i + 7] & 0x00000000000000FFL) << 56);
   }


   static void bmix(unsigned long int *k1, unsigned long int *k2, unsigned long int *h1, unsigned long int *h2, unsigned long int *c1, unsigned long int *c2) {
      *k1 *= *c1;
      //print("bmix ", k1,k2,h1,h2,c2,c1);
      *k1 = (*k1 << 23) | (*k1 >> (64 - 23));
      //print("bmix ", k1,k2,h1,h2,c2,c1);
      *k1 *= *c2;
      *h1 ^= *k1;
      *h1 += *h2;
      //print("bmix ", k1,k2,h1,h2,c2,c1);
      *h2 = (*h2 << 41) | (*h2 >> (64 - 41));

      *k2 *= *c2;
      *k2 = (*k2 << 23) | (*k2 >> (64 - 23));
      *k2 *= *c1;
      *h2 ^= *k2;
      *h2 += *h1;

      *h1 = *h1 * 3 + 0x52dce729;
      *h2 = *h2 * 3 + 0x38495ab5;

      *c1 = *c1 * 5 + 0x7b7d159c;
      *c2 = *c2 * 5 + 0x6bce6396;
   }

unsigned long int ffmix(unsigned long int k) {
      k ^= k >> 33;
      k *= 0xff51afd7ed558ccdL;
      k ^= k >> 33;
      k *= 0xc4ceb9fe1a85ec53L;
      k ^= k >> 33;

      return k;
   }



unsigned long int MurmurHash3_x64_64(const char* key,int length, int seed) {
    unsigned long int h1;
    unsigned long int h2;
    unsigned long int c1;
    unsigned long int c2;
    unsigned long int k1;
    unsigned long int k2;

     h1 = 0x9368e53c2f6af274L ^ seed;
      h2 = 0x586dcd208f7cd3fdL ^ seed;

      c1 = 0x87c37b91114253d5L;
      c2 = 0x4cf5ad432745937fL;
      //print("1 ", &k1,&k2,&h1,&h2,&c2,&c1);
      for (int i = 0; i < length / 16; i++) {
         k1 = getblock(key, i * 2 * 8);
         k2 = getblock(key, (i * 2 + 1) * 8);

         bmix(&k1, &k2, &h1, &h2, &c1, &c2);

      }
      k1 = 0;
      k2 = 0;
      //print("2 ", &k1,&k2,&h1,&h2,&c2,&c1);
      
      int tail;
      tail = (length >> 4) << 4;

      switch (length & 15) {
         case 15: k2 ^= (unsigned long int) key[tail + 14] << 48;
         case 14: k2 ^= (unsigned long int) key[tail + 13] << 40;
         case 13: k2 ^= (unsigned long int) key[tail + 12] << 32;
         case 12: k2 ^= (unsigned long int) key[tail + 11] << 24;
         case 11: k2 ^= (unsigned long int) key[tail + 10] << 16;
         case 10: k2 ^= (unsigned long int) key[tail + 9] << 8;
         case 9:  k2 ^= (unsigned long int) key[tail + 8];

         case 8:  k1 ^= (unsigned long int) key[tail + 7] << 56;
         case 7:  k1 ^= (unsigned long int) key[tail + 6] << 48;
         case 6:  k1 ^= (unsigned long int) key[tail + 5] << 40;
         case 5:  k1 ^= (unsigned long int) key[tail + 4] << 32;
         case 4:  k1 ^= (unsigned long int) key[tail + 3] << 24;
         case 3:  k1 ^= (unsigned long int) key[tail + 2] << 16;
         case 2:  k1 ^= (unsigned long int) key[tail + 1] << 8;
         case 1:  k1 ^= (unsigned long int) key[tail + 0];
             //print("22 ", &k1,&k2,&h1,&h2,&c2,&c1);
            bmix(&k1, &k2, &h1, &h2, &c1, &c2);
      }
      //print("3 ", &k1,&k2,&h1,&h2,&c2,&c1);

      h2 ^= length;

      h1 += h2;
      h2 += h1;
      //print("4 ", &k1,&k2,&h1,&h2,&c2,&c1);
      h1 = ffmix(h1);
      h2 = ffmix(h2);

      h1 += h2;
      h2 += h1;
      //print("5 ", &k1,&k2,&h1,&h2,&c2,&c1);

      return h1;

}


int MurmurHash3_x64_32(const char* key, int length, int seed) {
      return (int) ((unsigned long int)MurmurHash3_x64_64(key, length, seed) >> 32);
   }