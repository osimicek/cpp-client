#include <string>
#include <iostream>

#include "constants.h"




class Marshaller{
public:
    Marshaller();
    int dump(std::string* dest, int src);
    int dump(std::string* dest, double src);
    int dump(std::string* dest, std::string *src);

    int load(int* dest, std::string* src);
    int load(double* dest, std::string* src);
    int load(std::string* dest, std::string* src);

};

class MarshallerJBoss: public Marshaller{
    const static short DOUBLE_TYPE = 0x034f;
    const static short INT_TYPE = 0x034b;
    const static short STRING_TYPE_LEN_0 = 0x033d;
    const static short STRING_TYPE_LEN_1 = 0x033e;
    const static short STRING_TYPE_LEN_2 = 0x033f;
public:
    MarshallerJBoss();
    int dump(std::string* dest, int src);
    int dump(std::string* dest, double src);
    int dump(std::string* dest, std::string *src);

    int load(int* dest, std::string* src);
    int load(double* dest, std::string* src);
    int load(std::string* dest, std::string* src);

};