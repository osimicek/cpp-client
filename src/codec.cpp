#define DONT_INCLUDE_CODEC11  //circular dependency
#include "codec.h"
#undef DONT_INCLUDE_CODEC11  //circular dependency




template <class T>
void Codec<T>::write_header(short op_code)
{
  codec->write_header(op_code);
}

