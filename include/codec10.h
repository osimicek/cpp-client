#ifndef CODEC10_H_INCLUDED
#define CODEC10_H_INCLUDED


#include <murmur/MurmurHash2.h>
#include <murmur/MurmurHash3.h>


class Codec10: public Codec{

    public:
        Codec10(Transport &t);
        using Codec::write_header;
        void write_header(char op_code, const std::string *cache_name, int flags);
        int read_header();
        int check_for_errors_in_response_status(int status);
        virtual void read_new_topology_if_present();
        virtual void update_transport_bank();
};

#include "transport.h"
#endif

