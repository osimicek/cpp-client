#ifndef CODEC10_H_INCLUDED
#define CODEC10_H_INCLUDED

#include "transport.h"
#include <murmur/MurmurHash3.h>
#include <limits.h>
#include "constants.h"
#include <sys/time.h>
#include <iostream>

class Transport;

class Codec10{

    public:
        Transport &transport;
        // Codec10();
        Codec10(Transport &t);
        void write_header(char op_code, char version);
        void write_header(char op_code);
        int read_header();
        int check_for_errors_in_response_status(char status);
        int read_new_topology_if_present();
        void update_transport_bank();
};

#endif

