#ifndef CODEC12_H_INCLUDED
#define CODEC12_H_INCLUDED

#include "codec.h"



/**
 * A Hot Rod protocol v 1.2 encoder/decoder.
 *
 * @author ondrejsimicek@seznam.cz
 */
class Codec12  : public Codec11{
    public:
        Codec12(Transport &t);
        using Codec::write_header;
        void write_header(char op_code, const std::string *cache_name, int flags);
};




#endif