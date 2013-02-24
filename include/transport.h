#ifndef TRANSPORT_H_INCLUDED
#define TRANSPORT_H_INCLUDED


#include <iostream>
#include <cstring> //memcpy


#include "transportFactory.h"
#include <sys/types.h>


//network
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "codec10.h"
#include "codec11.h"

#include "codec.h"

class TransportFactory;

class Transport{
    public:
        std::string host;
        int port;
        int hash;
        int used;
        int _socket;
        int valid;
        void *codec;

        TransportFactory &transportFactory;

        std::string packet;
        Transport(std::string, int, TransportFactory &tF);
        void write_header(char op_code, const std::string *cache_name, int flags);
        void write_version(long long value);
        void write_varint(int value);
        void write_varlong(long long value);
        void write_char(char value);
        void write_array(const std::string *arr);
        void write_byte(short byte);
        int flush();
        int read_varint();
        long long read_varlong();
        char read_byte();
        int read_2bytes();
        int read_4bytes();
        void read_array(std::string *arr);
        int read_header();
        int check_for_errors_in_response_status(char status);
        int read_new_topology_if_present();
        int create_connection();
};



#endif