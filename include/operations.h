#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED
#include "transportFactory.h"
#include "transport.h"
#include "constants.h"
#include "varItem.h"
#include <map>
#include <iostream>





/**
 * Metadata returned by RemoteCache#getWithMetadata
 *
 * @author ondrejsimicek@seznam.cz
 */
class RemoteEntryMetadata{
    public:
        short flag; 
        int lifespan, maxidle;
        long long version, created, lastused;
        RemoteEntryMetadata();
        void clear();
};
/**
 * Basic class for all hot rod operations. Retries on
 * failture operation.
 *
 * @author ondrejsimice@seznam.cz
 */
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
/**
 * Implements Hot Rot get operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class GetOperation : public AbstractOperation{
    public:
        std::string *value;
        GetOperation(const std::string *key, std::string *value, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot getWithVersion operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class GetWithVersionOperation : public AbstractOperation{
    public:
        std::string *value;
        long long *version;
        GetWithVersionOperation(const std::string *key, std::string *value, long long *version, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot getWithMetadata operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class GetWithRemoteEntryMetadataOperation : public AbstractOperation{
    public:
        std::string *value;
        RemoteEntryMetadata *meta;
        GetWithRemoteEntryMetadataOperation(const std::string *key, std::string *value, RemoteEntryMetadata *meta, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot containsKey operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class ContainsKeyOperation : public AbstractOperation{
    public:
        ContainsKeyOperation(const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot remove operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class RemoveOperation : public AbstractOperation{
    public:
        std::string *prev_value;
        RemoveOperation(const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot removeIfUnmodified operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class RemoveIfUnmodifiedOperation : public AbstractOperation{
    public:
        std::string *prev_value;
        long long version;
        RemoveIfUnmodifiedOperation(const std::string *key, std::string *prev_value, long long version, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Base class for all Hot Rot put operations.
 *
 * @author ondrejsimice@seznam.cz
 */
class PutBasedOperation : public AbstractOperation{
    public:
        const std::string *value;
        std::string *prev_value;
        int lifespan, idle;
        using AbstractOperation::return_possible_prev_value;
        PutBasedOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation(int op_code);
};
/**
 * Implements Hot Rot put operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class PutOperation : public PutBasedOperation{
    public:
        PutOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot putIfAbsent operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class PutIfAbsentOperation : public PutBasedOperation{
    public:
        PutIfAbsentOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot replace operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class ReplaceOperation : public PutBasedOperation{
    public:
        ReplaceOperation(const std::string *key, const std::string *value, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot replaceIfUnmodified operation.
 *
 * @author ondrejsimice@seznam.cz
 */
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
/**
 * Implements Hot Rot getBulk operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class GetBulkOperation : public AbstractOperation{
    public:
        int count;
        std::map<VarItem, VarItem> *bulk;
        GetBulkOperation(std::map<VarItem, VarItem> *bulk, int count, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};
/**
 * Implements Hot Rot bulkKeysGet operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class BulkKeysGetOperation : public AbstractOperation{
    public:
        int scope;
        std::vector<VarItem> *keys;
        BulkKeysGetOperation(std::vector<VarItem> *keys, int scope, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

/**
 * Implements Hot Rot clear operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class ClearOperation : public AbstractOperation{
    public:
        ClearOperation(TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();

};
/**
 * Implements Hot Rot ping operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class PingOperation : public AbstractOperation{
    public:
        PingOperation(TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();

};
/**
 * Implements Hot Rot stats operation.
 *
 * @author ondrejsimice@seznam.cz
 */
class StatsOperation : public AbstractOperation{
    public:
        std::map<std::string,std::string> *stats;
        StatsOperation(std::map<std::string,std::string> *stats, TransportFactory &tF, const std::string *cache_name, int flags);
        virtual int execute_operation();
};

#endif