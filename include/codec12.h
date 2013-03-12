    #ifndef CODEC12_H_INCLUDED
    #define CODEC12_H_INCLUDED

    #include "codec.h"




    class Codec12  : public Codec11{
        public:
            Codec12(Transport &t);
            using Codec::write_header;
            void write_header(char op_code, const std::string *cache_name, int flags);
    };




    #endif