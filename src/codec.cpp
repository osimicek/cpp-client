#include "codec.h"

Codec::Codec(Transport &t):transport(t){}

/**
    * Writes a request header with the given parameters to the transport.
*/
void Codec::write_header(char op_code, const std::string *cache_name, int flags){}   

/**
    * Writes a request header with the given parameters to the transport.
*/
void Codec::write_header(char op_code, const std::string *cache_name, int flags, char version){}

/**
    * Reads a response header from the transport and returns the status
    * of the response.
*/
int Codec::read_header(){return 1;}