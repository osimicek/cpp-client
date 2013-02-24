#ifndef CONSISTENTHASH10_H_INCLUDED
#define CONSISTENTHASH10_H_INCLUDED

#include <murmur/MurmurHash2.h>
#include "transportFactory.h"
#include "transport.h"

class TransportFactory;
class Transport;

class ConsistentHash10{

    public:
        TransportFactory &transportFactory;
        ConsistentHash10(TransportFactory &tf);
        Transport *get_transport();
        Transport *get_transport(const std::string *key);
        int find_index_of_transport(int key_hash);

};


#endif