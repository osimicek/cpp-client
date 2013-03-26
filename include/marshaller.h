#ifndef MARSHALLER_H_INCLUDED
#define MARSHALLER_H_INCLUDED

#include <string>
#include <iostream>
#include <sstream>

#include "constants.h"

class Marshaller;
extern Marshaller* DEFAULT_MARSHALLER;

class VarItem;

class Marshaller{
public:
    const static short DOUBLE_TYPE = 0x01;
    const static short INT_TYPE = 0x02;
    const static short STRING_TYPE = 0x03;

    Marshaller();
    virtual int dump(std::string *dest, const int src);
    virtual int dump(std::string *dest, const double src);
    virtual int dump(std::string *dest, const char *src);
    virtual int dump(std::string *dest, const std::string *src);
    virtual int dump(std::string *dest, const VarItem *src);

    virtual int load(int *dest, const std::string *src);
    virtual int load(double *dest, const std::string *src);
    virtual int load(char *dest, const std::string *src);
    virtual int load(std::string *dest, const std::string *src);
    virtual int load(VarItem *dest, const std::string *src);

    virtual int get_type(const std::string* src);
    virtual std::ostream & print(std::ostream& _stream, const std::string* src);

};

class MarshallerJBoss: public Marshaller{
public:
    const static short DOUBLE_TYPE = 0x034f;
    const static short INT_TYPE = 0x034b;
    const static short STRING_TYPE = 0x033d;
    const static short STRING_TYPE_LEN_0 = 0x033d;
    const static short STRING_TYPE_LEN_1 = 0x033e;
    const static short STRING_TYPE_LEN_2 = 0x033f;

    MarshallerJBoss();
    int dump(std::string *dest, const int src);
    int dump(std::string *dest, const double src);
    int dump(std::string *dest, const char *src);
    int dump(std::string *dest, const std::string *src);
    int dump(std::string *dest, const VarItem *src);

    int load(int *dest, const std::string *src);
    int load(double *dest, const std::string *src);
    int load(char *dest, const std::string *src);
    int load(std::string *dest, const std::string *src);
    int load(VarItem *dest, const std::string *src);

    int get_type(const std::string* src);
    std::ostream & print(std::ostream& _stream, const std::string* src);

};

#include "varItem.h"

#endif