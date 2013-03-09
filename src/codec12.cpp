#include "codec.h"

Codec12::Codec12(Transport &t):Codec11(t){

}

void Codec12::write_header(short op_code, const std::string *cache_name, int flags){
   std::cout<< "je to OK Header 12" << std::endl;
    Codec10::write_header(op_code, cache_name, flags, VERSION_12);
}

