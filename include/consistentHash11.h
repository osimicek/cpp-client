#ifndef DONT_INCLUDE_CONSISTENTHASH11   //circular dependency
    #ifndef CONSISTANTHESH11_H_INCLUDED
    #define CONSISTANTHESH11_H_INCLUDED

    #include "consistentHash10.h"


    class ConsistentHash11: public ConsistentHash10{
        public:
            ConsistentHash11(TransportFactory &tf);
    };


    #endif
#endif