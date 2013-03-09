    #ifndef CODEC12_H_INCLUDED
    #define CODEC12_H_INCLUDED

    #include "codec.h"




    class Codec12  : public Codec11{
        public:
            Codec12(Transport &t);
            virtual void write_header(short op_code, const std::string *cache_name, int flags);
            using Codec11::read_new_topology_if_present;
            using Codec11::update_transport_bank;
    };




    #endif