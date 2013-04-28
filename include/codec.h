#ifndef DONT_INCLUDE_CODEC
#define DONT_INCLUDE_CODEC

#include <limits.h>
#include "constants.h"
#include <sys/time.h>
#include <iostream>

class Transport;

class Codec{

public:
    Transport &transport;
    Codec(Transport &t);
    virtual int read_header();
    virtual void write_header(char op_code, const std::string *cache_name, int flags);
    void write_header(char op_code, const std::string *cache_name, int flags, char version);
};

#include "codec10.h"
#include "codec11.h"
#include "codec12.h"

#endif