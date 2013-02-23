#ifndef CONSISTENTHASH10_H_INCLUDED
#define CONSISTENTHASH10_H_INCLUDED

#include "transportFactory.h"
#include "transport.h"

class TransportFactory;
class Transport;

class ConsistentHash10{

    public:
        TransportFactory &transportFactory;
        // Codec10();
        ConsistentHash10(TransportFactory &tf);
        Transport *get_transport(const std::string *key);
        int find_index_of_transport(int key_hash);

};


#endif