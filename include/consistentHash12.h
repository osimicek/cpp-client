
    #ifndef CONSISTANTHESH12_H_INCLUDED
    #define CONSISTANTHESH12_H_INCLUDED

    #include <murmur/MurmurHash3.h>

    /**
     * A consistent hash algorithm implementation for Hot Rod v1.2
     *
     *
     * @author ondrejsimicek@seznam.cz
     */
    class ConsistentHash12: public ConsistentHash11{
        public:
            ConsistentHash12(TransportFactory &tf);
    };


    #endif
