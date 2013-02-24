#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED
#include "transportFactory.h"
#include "transport.h"
#include "constants.h"
#include <iostream>

class AbstractOperation{
    public:
        int status;
        TransportFactory &transportFactory;
        Transport *transport;
        const std::string *cache_name;
        int flags;
        explicit AbstractOperation(TransportFactory &tF);
        int execute();
        virtual int execute_operation();

};

class GetOperation : public AbstractOperation{
    public:
        const std::string *key;
        std::string *value;
        GetOperation(std::string *value, TransportFactory &tF, const std::string *key, const std::string *cache_name, int flags);
        virtual int execute_operation();

};

class PutOperation : public AbstractOperation{
    public:
        const std::string *key;
        const std::string *value;
        int lifespan, idle;
        PutOperation(const std::string *value, TransportFactory &tF, const std::string *key, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();

};

class ClearOperation : public AbstractOperation{
    public:
        ClearOperation(TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();

};

#endif