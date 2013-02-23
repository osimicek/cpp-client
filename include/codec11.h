#ifndef DONT_INCLUDE_CODEC11  //circular dependency
    #ifndef CODEC11_H_INCLUDED
    #define CODEC11_H_INCLUDED

    #include "codec10.h"

    //class Codec10;

    class Codec11  : public Codec10{
        public:
            Codec11(Transport &t);
            void write_header(short op_code);
    };




    #endif
#endif