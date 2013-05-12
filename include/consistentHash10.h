#ifndef CONSISTENTHASH10_H_INCLUDED
#define CONSISTENTHASH10_H_INCLUDED

#include <murmur/MurmurHash2.h>

/**
 * A consistent hash algorithm implementation for Hot Rod v1.0
 *
 *
 * @author ondrejsimicek@seznam.cz
 */
class ConsistentHash10: public ConsistentHash{
    protected:
        int find_index_of_transport(int key_hash);
    public:
        ConsistentHash10(TransportFactory &tf);
        Transport *get_transport();
        Transport *get_transport(const std::string *key);
};

#include "transportFactory.h"
#include "transport.h"
#endif