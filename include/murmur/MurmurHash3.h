#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

#include <stdint.h>
#include <iostream>
int MurmurHash3_x64_32(const char* key, int length, int seed);
unsigned long int MurmurHash3_x64_64(const char* key,int length, int seed);

#endif // _MURMURHASH3_H_