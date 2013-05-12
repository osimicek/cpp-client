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

// #include "codec10.h"
// #include "codec11.h"
// #include "codec12.h"

#include "codec.h"

class TransportFactory;

/**
    * Comunicates with one instance of Infinispan cache.
    * Can write header, key, value .... to request header,
    * than send request. Also can read header and data from
    * the response
    * 
    *
    * @author ondrejsimicek@seznam.cz
*/
class Transport{
    private:
        int create_connection();
    public:
        std::string host;
        int port;
        /** Data to get hash code of server */
        int hash;
        /** Only one thread can use this Transport at the same time */
        int used;
        int _socket;
        /** Used to invalidate Transport when topology change */
        int valid;
        Codec *codec;

        TransportFactory &transportFactory;

        std::string packet;
        Transport(std::string, int, TransportFactory &tF);
        ~Transport();
        void write_header(char op_code, const std::string *cache_name, int flags);
        void write_varint(int value);
        void write_varlong(long long value);
        void write_char(char value);
        void write_array(const std::string *arr);
        void write_byte(short value);
        void write_8bytes(long long value);
        int flush();
        int read_varint();
        long long read_varlong();
        int read_byte();
        int read_2bytes();
        int read_4bytes();
        long long read_8bytes();
        void read_array(std::string *arr);
        int read_header();
        int close_connection();
};



#endif