
    #ifndef CONSISTANTHESH12_H_INCLUDED
    #define CONSISTANTHESH12_H_INCLUDED

    #include <murmur/MurmurHash3.h>


    class ConsistentHash12: public ConsistentHash11{
        public:
            ConsistentHash12(TransportFactory &tf);
    };


    #endif
