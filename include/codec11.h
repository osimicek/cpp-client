#ifndef CODEC11_H_INCLUDED
#define CODEC11_H_INCLUDED

#include <murmur/MurmurHash3.h>

/**
 * A Hot Rod protocol v 1.1 encoder/decoder.
 *
 * @author ondrejsimicek@seznam.cz
 */
class Codec11  : public Codec10{
    protected:
        void read_new_topology_with_hash_space();
        void read_new_topology_without_hash_space();
    public:
        Codec11(Transport &t);
        using Codec::write_header;
        void write_header(char op_code, const std::string *cache_name, int flags);
        void update_transport_bank();
};




#endif