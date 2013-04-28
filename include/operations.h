#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED
#include "transportFactory.h"
#include "transport.h"
#include "constants.h"
#include "varItem.h"
#include <map>
#include <iostream>






class RemoteEntryMetadata{
    public:
        char flag; 
        int lifespan, maxidle;
        long long version, created, lastused;
        RemoteEntryMetadata();
        void clear();
};

class AbstractOperation{
    public:
        int status;
        TransportFactory &transportFactory;
        Transport *transport;
        const std::string *cache_name;
        const std::string *key;
        int flags;
        void  return_possible_prev_value(std::string *prev_value);
        explicit AbstractOperation(TransportFactory &tF);
        int execute();
        virtual int execute_operation();


};

class GetOperation : public AbstractOperation{
    public:
        std::string *value;
        GetOperation(const std::string *key, std::string *value, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class GetWithVersionOperation : public AbstractOperation{
    public:
        std::string *value;
        long long *version;
        GetWithVersionOperation(const std::string *key, std::string *value, long long *version, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class GetWithRemoteEntryMetadataOperation : public AbstractOperation{
    public:
        std::string *value;
        RemoteEntryMetadata *meta;
        GetWithRemoteEntryMetadataOperation(const std::string *key, std::string *value, RemoteEntryMetadata *meta, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class ContainsKeyOperation : public AbstractOperation{
    public:
        ContainsKeyOperation(const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class RemoveOperation : public AbstractOperation{
    public:
        std::string *prev_value;
        RemoveOperation(const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class RemoveIfUnmodifiedOperation : public AbstractOperation{
    public:
        std::string *prev_value;
        long long version;
        RemoveIfUnmodifiedOperation(const std::string *key, std::string *prev_value, long long version, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class PutBasedOperation : public AbstractOperation{
    public:
        const std::string *value;
        std::string *prev_value;
        int lifespan, idle;
        using AbstractOperation::return_possible_prev_value;
        PutBasedOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation(int op_code);
};

class PutOperation : public PutBasedOperation{
    public:
        PutOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class PutIfAbsentOperation : public PutBasedOperation{
    public:
        PutIfAbsentOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class ReplaceOperation : public PutBasedOperation{
    public:
        ReplaceOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class ReplaceIfUnmodifiedOperation : public AbstractOperation{
    public:
        const std::string *value;
        std::string *prev_value;
        long long version;
        int lifespan, idle;
        using AbstractOperation::return_possible_prev_value;
        ReplaceIfUnmodifiedOperation(const std::string *key, const std::string *value, std::string *prev_value, long long version, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};

class GetBulkOperation : public AbstractOperation{
    public:
        int count;
        std::map<VarItem, VarItem> *bulk;
        GetBulkOperation(std::map<VarItem, VarItem> *bulk, int count, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

class BulkKeysGetOperation : public AbstractOperation{
    public:
        int scope;
        std::vector<VarItem> *keys;
        BulkKeysGetOperation(std::vector<VarItem> *keys, int scope, TransportFactory &tF, const std::string *cache_name, int flags);
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