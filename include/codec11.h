    #ifndef CODEC11_H_INCLUDED
    #define CODEC11_H_INCLUDED

    #include <murmur/MurmurHash3.h>

    //class Codec10;

    class Codec11  : public Codec10{
        public:
            Codec11(Transport &t);
            void write_header(char op_code, const std::string *cache_name, int flags);
            int read_new_topology_if_present();
            void update_transport_bank();
    };




    #endif