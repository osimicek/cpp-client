
    #ifndef CONSISTANTHESH11_H_INCLUDED
    #define CONSISTANTHESH11_H_INCLUDED

    #include <murmur/MurmurHash3.h>


    /**
     * A consistent hash algorithm implementation for Hot Rod v1.1
     *
     *
     * @author ondrejsimicek@seznam.cz
     */
    class ConsistentHash11: public ConsistentHash10{
        public:
            ConsistentHash11(TransportFactory &tf);
            using ConsistentHash::get_transport;
            Transport *get_transport(const std::string *key);
    };


    #endif
