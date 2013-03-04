#ifndef DONT_INCLUDE_CONSISTENTHASH12   //circular dependency
    #ifndef CONSISTANTHESH12_H_INCLUDED
    #define CONSISTANTHESH12_H_INCLUDED

    #include <murmur/MurmurHash3.h>
    #include "consistentHash11.h"


    class ConsistentHash12: public ConsistentHash11{
        public:
            ConsistentHash12(TransportFactory &tf);
            using ConsistentHash11::get_transport;
    };


    #endif
#endif