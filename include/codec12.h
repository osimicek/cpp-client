#ifndef DONT_INCLUDE_CODEC12  //circular dependency
    #ifndef CODEC12_H_INCLUDED
    #define CODEC12_H_INCLUDED


    #define DONT_INCLUDE_CODEC11  //circular dependency
    #define DONT_INCLUDE_CODEC12  //circular dependency
    #include "codec10.h"
    #undef DONT_INCLUDE_CODEC11  //circular dependency
    #undef DONT_INCLUDE_CODEC12  //circular dependency
    #include "codec11.h"




    class Codec12  : public Codec11{
        public:
            Codec12(Transport &t);
            void write_header(short op_code, const std::string *cache_name, int flags);
            using Codec11::read_new_topology_if_present;
            using Codec11::update_transport_bank;
    };




    #endif
#endif