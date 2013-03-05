#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED
#include "transportFactory.h"
#include "transport.h"
#include "constants.h"
#include <map>
#include <iostream>

class Metadata{
    public:
        char flag; 
        int lifespan, maxidle;
        long long version, created, lastused;
        Metadata();
};

class AbstractOperation{
    public:
        int status;
        TransportFactory &transportFactory;
        Transport *transport;
        const std::string *cache_name;
        int flags;
        void  return_possible_prev_value(std::string *prev_value);
        explicit AbstractOperation(TransportFactory &tF);
        int execute();
        virtual int execute_operation();


};

class GetOperation : public AbstractOperation{
    public:
        const std::string *key;
        std::string *value;
        GetOperation(std::string *value, const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class GetWithVersionOperation : public AbstractOperation{
    public:
        const std::string *key;
        std::string *value;
        long long *version;
        GetWithVersionOperation(std::string *value, const std::string *key, long long *version, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class GetWithMetadataOperation : public AbstractOperation{
    public:
        const std::string *key;
        std::string *value;
        Metadata *meta;
        GetWithMetadataOperation(std::string *value, Metadata *meta, const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class ContainsKeyOperation : public AbstractOperation{
    public:
        const std::string *key;
        ContainsKeyOperation(const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class RemoveOperation : public AbstractOperation{
    public:
        const std::string *key;
        std::string *prev_value;
        RemoveOperation(std::string *prev_value, const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class RemoveIfUnmodifiedOperation : public AbstractOperation{
    public:
        const std::string *key;
        std::string *prev_value;
        long long version;
        RemoveIfUnmodifiedOperation(std::string *prev_value, const std::string *key, long long version, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class PutBasedOperation : public AbstractOperation{
    public:
        const std::string *key;
        const std::string *value;
        std::string *prev_value;
        int lifespan, idle;
        using AbstractOperation::return_possible_prev_value;
        PutBasedOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation(int op_code);
};

class PutOperation : public PutBasedOperation{
    public:
        PutOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class PutIfAbsentOperation : public PutBasedOperation{
    public:
        PutIfAbsentOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class ReplaceOperation : public PutBasedOperation{
    public:
        ReplaceOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class ReplaceIfUnmodifiedOperation : public AbstractOperation{
    public:
        const std::string *key;
        const std::string *value;
        std::string *prev_value;
        long long version;
        int lifespan, idle;
        using AbstractOperation::return_possible_prev_value;
        ReplaceIfUnmodifiedOperation(const std::string *value, const std::string *key, std::string *prev_value, long long version, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class GetBulkOperation : public AbstractOperation{
    public:
        int count;
        std::map<std::string,std::string> *bulk;
        GetBulkOperation(std::map<std::string,std::string> *bulk, int count, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class BulkKeysGetOperation : public AbstractOperation{
    public:
        int scope;
        std::vector<std::string> *keys;
        BulkKeysGetOperation(std::vector<std::string> *keys, int scope, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};


class ClearOperation : public AbstractOperation{
    public:
        ClearOperation(TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();

};

class PingOperation : public AbstractOperation{
    public:
        PingOperation(TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();

};

class StatsOperation : public AbstractOperation{
    public:
        std::map<std::string,std::string> *stats;
        StatsOperation(std::map<std::string,std::string> *stats, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

#endif