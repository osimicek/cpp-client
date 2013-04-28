#ifndef DONT_INCLUDE_CONSISTENTHASH
#define DONT_INCLUDE_CONSISTENTHASH

#include "transportFactory.h"
#include "transport.h"
#include "consistentHash.h"

class TransportFactory;
class Transport;

class ConsistentHash{
    public:
        TransportFactory &transportFactory;
        ConsistentHash(TransportFactory &tf);
        Transport *get_transport();
        virtual Transport *get_transport(const std::string *key);
};

#include "consistentHash10.h"
#include "consistentHash11.h"
#include "consistentHash12.h"

#endif