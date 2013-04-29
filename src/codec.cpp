#include "codec.h"

Codec::Codec(Transport &t):transport(t){}
void Codec::write_header(char op_code, const std::string *cache_name, int flags){}   
int Codec::read_header(){return 1;}

void Codec::write_header(char op_code, const std::string *cache_name, int flags, char version){
    struct timeval begin;
    gettimeofday(&begin, NULL);
    long long the_time;  
    the_time = begin.tv_sec;
    the_time <<= 32;
    the_time += begin.tv_usec;

    transport.packet.clear(); //clear packet 
    transport.write_char(REQUEST_MAGIC); //HotRod request
    transport.write_varlong(transport.transportFactory.get_and_inc_message_id());//encode_varlong(the_time); //msg ID



    transport.write_char(version); //version 10 or 11
    transport.write_char(op_code); //op_code
    transport.write_array(cache_name); //cache name length
    transport.write_char(flags); //flags
    transport.write_char(transport.transportFactory.get_intelligence()); //intelligenc
    transport.write_varint(transport.transportFactory.get_topology_id()); //topology
    //todo change once TX support is added
    transport.write_char(0x00); //transaction type
}

