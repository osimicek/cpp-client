#include "codec.h"

Codec12::Codec12(Transport &t):Codec11(t){

}

/**
    * Writes a request header with the given parameters to the transport.
*/
void Codec12::write_header(char op_code, const std::string *cache_name, int flags){
   write_header(op_code, cache_name, flags, VERSION_12);
}

