#ifndef CODEC10_H_INCLUDED
#define CODEC10_H_INCLUDED


#include <murmur/MurmurHash2.h>
#include <murmur/MurmurHash3.h>
#include <limits.h>
#include "constants.h"
#include <sys/time.h>
#include <iostream>

class Transport;

class Codec{
public:
    Transport &transport;
    Codec(Transport &t);
    virtual int read_header();
    virtual void write_header(char op_code, const std::string *cache_name, int flags);
};

class Codec10: public Codec{

    public:
        Codec10(Transport &t);
        void write_header(char op_code, const std::string *cache_name, int flags, char version);
         void write_header(char op_code, const std::string *cache_name, int flags);
        virtual int read_header();
        int check_for_errors_in_response_status(char status);
        virtual int read_new_topology_if_present();
        virtual void update_transport_bank();
};

#include "transport.h"
#endif

