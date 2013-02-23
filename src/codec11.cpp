#include "codec11.h"


Codec11::Codec11(Transport &t):Codec10(t){
    //transport = t;
    // std::cout << "CODEC 11"<< std::endl;
}

void Codec11::write_header(short op_code){
    std::cout << "WRITE 11"<< std::endl;
    Codec10::write_header(op_code, 11);
}