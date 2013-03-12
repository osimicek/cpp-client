    #ifndef CODEC11_H_INCLUDED
    #define CODEC11_H_INCLUDED

    #include <murmur/MurmurHash3.h>

    //class Codec10;

    class Codec11  : public Codec10{
        public:
            Codec11(Transport &t);
            using Codec::write_header;
            void write_header(char op_code, const std::string *cache_name, int flags);
            void read_new_topology_if_present();
            void update_transport_bank();
    };




    #endif