#ifndef DONT_INCLUDE_CONSISTENTHASH11   //circular dependency
    #ifndef CONSISTANTHESH11_H_INCLUDED
    #define CONSISTANTHESH11_H_INCLUDED

    #include <murmur/MurmurHash3.h>
    #include "consistentHash10.h"


    class ConsistentHash11: public ConsistentHash10{
        public:
            ConsistentHash11(TransportFactory &tf);
            using ConsistentHash10::get_transport;
            Transport *get_transport(const std::string *key);
    };


    #endif
#endif