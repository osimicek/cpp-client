#ifndef MARSHALLER_H_INCLUDED
#define MARSHALLER_H_INCLUDED

#include <string>
#include <iostream>

#include "constants.h"




class Marshaller{
public:
    Marshaller();
    virtual int dump(std::string *dest, const int src);
    virtual int dump(std::string *dest, const double src);
    virtual int dump(std::string *dest, const char *src);
    virtual int dump(std::string *dest, const std::string *src);

    virtual int load(int *dest, std::string *src);
    virtual int load(double *dest, std::string *src);
    virtual int load(char *dest, std::string *src);
    virtual int load(std::string *dest, std::string *src);

};

class MarshallerJBoss: public Marshaller{
    const static short DOUBLE_TYPE = 0x034f;
    const static short INT_TYPE = 0x034b;
    const static short STRING_TYPE_LEN_0 = 0x033d;
    const static short STRING_TYPE_LEN_1 = 0x033e;
    const static short STRING_TYPE_LEN_2 = 0x033f;
public:
    MarshallerJBoss();
    int dump(std::string *dest, const int src);
    int dump(std::string *dest, const double src);
    int dump(std::string *dest, const char *src);
    int dump(std::string *dest, const std::string *src);

    int load(int *dest, std::string *src);
    int load(double *dest, std::string *src);
    int load(char *dest, std::string *src);
    int load(std::string *dest, std::string *src);

};

#endif