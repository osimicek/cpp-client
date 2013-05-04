#ifndef DONT_INCLUDE_CODEC
#define DONT_INCLUDE_CODEC

#include <limits.h>
#include "constants.h"
#include <sys/time.h>
#include <iostream>


#ifndef NOT_THREAD_SAFE
    #define LOCK_T_TF() pthread_mutex_lock (&transport.transportFactory.mutex);
    #define UNLOCK_T_TF() pthread_mutex_unlock (&transport.transportFactory.mutex);
#else
    #define LOCK_T_TF()
    #define UNLOCK_T_TF()
#endif

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