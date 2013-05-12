#ifndef MARSHALLER_H_INCLUDED
#define MARSHALLER_H_INCLUDED

#include <string>
#include <iostream>
#include <sstream>

#include "constants.h"

class Marshaller;
extern Marshaller* DEFAULT_MARSHALLER;

class VarItem;


/**
    * Implements interface of marshaller
    *
    * @author ondrejsimicek@seznam.cz
*/
class Marshaller{
public:
    /** Represents int type */
    const static short INT_TYPE    = 0x01;
    /** Represents double type */
    const static short DOUBLE_TYPE = 0x02;
    /** Represents string type */
    const static short STRING_TYPE = 0x03;

    Marshaller();
    virtual int dump(const int src,         std::string *dest);
    virtual int dump(const double src,      std::string *dest);
    virtual int dump(const char *src,       std::string *dest);
    virtual int dump(const std::string *src, std::string *dest);
    virtual int dump(const VarItem *src,    std::string *dest);

    virtual int load(const std::string *src, int *dest);
    virtual int load(const std::string *src, double *dest);
    virtual int load(const std::string *src, char *dest);
    virtual int load(const std::string *src, std::string *dest);
    virtual int load(const std::string *src, VarItem *dest);

    virtual int get_type(const std::string* src);
    virtual std::ostream & print(std::ostream& _stream, const std::string* src);

};

/**
    * Implements JBoss like marshaller
    *
    * @author ondrejsimicek@seznam.cz
*/
class MarshallerJBoss: public Marshaller{
public:
    /** Represents int type */
    const static short INT_TYPE = 0x034b;
    /** Represents double type */
    const static short DOUBLE_TYPE = 0x034f;
    /** Represents string type */
    const static short STRING_TYPE = 0x033d;
    const static short STRING_TYPE_LEN_0 = 0x033d;
    const static short STRING_TYPE_LEN_1 = 0x033e;
    const static short STRING_TYPE_LEN_2 = 0x033f;

    MarshallerJBoss();
    int dump(const int src,         std::string *dest);
    int dump(const double src,      std::string *dest);
    int dump(const char *src,       std::string *dest);
    int dump(const std::string *src, std::string *dest);
    int dump(const VarItem *src,    std::string *dest);

    int load(const std::string *src, int *dest);
    int load(const std::string *src, double *dest);
    int load(const std::string *src, char *dest);
    int load(const std::string *src, std::string *dest);
    int load(const std::string *src, VarItem *dest);

    int get_type(const std::string* src);
    std::ostream & print(std::ostream& _stream, const std::string* src);

};

#include "varItem.h"

#endif