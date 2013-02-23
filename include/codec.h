#ifndef CODEC_H_INCLUDED
#define CODEC_H_INCLUDED
#include "codec10.h"
#include "codec11.h"
#include "transport.h"
#include <iostream>

template <class T>
class Codec {
    T *codec;
  public:
    Codec (Transport &t)
      { 
        std::cout << "CODEC" << std::endl;
        codec = new T(t);
    }   
    void write_header(short op_code);
};





#endif