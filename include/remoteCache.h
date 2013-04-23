#ifndef REMOTE2_H_INCLUDED
#define REMOTE2_H_INCLUDED

#include <string>
#include <iostream>
#include <transportFactory.h>
#include <marshaller.h>
#include <operations.h>
#include <map>
#include <pthread.h>
#include "varItem.h"
class RemoteCache;

static void *put_provider(void *);
typedef struct {
    RemoteCache * RC;
    const VarItem *key;
    const VarItem *value;
    int lifespan;
    int maxidle;
} thread_args;









class RemoteCacheConfig{
    public:
        int version;
        int lifespan;
        int maxidle;
        int flags;
        std::string cache_name;
        std::string host;
        int port;
        char intelligence;
        RemoteCacheConfig();

};


class RemoteCache{
  private:
    TransportFactory *transportFactory;
    Marshaller *marshaller;
    int lifespan;
    int maxidle;
    int flags;
    std::string cache_name;
    void init(RemoteCacheConfig* remote_cache_config);
  public:
    RemoteCache(RemoteCacheConfig* remote_cache_config);
    RemoteCache(std::string host, int port);
    RemoteCache(std::string host);
    RemoteCache(void);
    int getKeyOwnersNum();
    int getVersion();
    
    int stats(std::map<std::string,std::string> *stats);
    int ping();
    int clear();
    template <typename TYPE,typename TYPE2>
    int put(const TYPE key,const TYPE2 value,int lifespan=-1, int maxidle=-1);

    int putAll(std::map<VarItem,VarItem> *data,int lifespan=-1, int maxidle=-1);
    int putAllAsync(std::map<VarItem,VarItem> *data,int lifespan=-1, int maxidle=-1);
    template <typename TYPE,typename TYPE2>
    int putIfAbsent(const TYPE key,const TYPE2 value,int lifespan=-1, int maxidle=-1);

    template <typename TYPE,typename TYPE2>
    int replace(const TYPE key, const TYPE2 value,int lifespan=-1, int maxidle=-1);

    template <typename TYPE,typename TYPE2>
    int replaceWithVersion(const TYPE key,const TYPE2 value,long long version,int lifespan=-1, int maxidle=-1);

    template <typename TYPE>
    int remove(const TYPE key);
    template <typename TYPE>
    int removeWithVersion(const TYPE key, long long version);
    template <typename TYPE>
    int containsKey(const TYPE key);


    

    template <typename TYPE,typename TYPE2>
    int get(const TYPE key, TYPE2 value);


    template <typename TYPE,typename TYPE2>
    int getWithVersion(const TYPE key, TYPE2 value,long long *version);

    template <typename TYPE,typename TYPE2>
    int getWithMetadata(const TYPE key, TYPE2 value, Metadata *meta);

    int getBulk(std::map<VarItem,VarItem> *bulk);
    int getBulk(std::map<VarItem,VarItem> *bulk, int count);

    int keySet(std::vector<VarItem> *keys, int scope=0);

    void print_servers();
    void close();

};

template <typename TYPE,typename TYPE2>
int RemoteCache::get(const TYPE key, TYPE2 value){
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);

    GetOperation *getOperation = new GetOperation(&m_key, &m_value, *transportFactory, &cache_name, flags);
    
    status = getOperation->execute();
    marshaller->load(value, &m_value);
    return status;   
};

template <typename TYPE,typename TYPE2>
int RemoteCache::getWithVersion(const TYPE key, TYPE2 value,long long *version){
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);
    GetWithVersionOperation *getWithVersionOperation = new GetWithVersionOperation(&m_key, &m_value, version, *transportFactory, &cache_name, flags);
    
    status = getWithVersionOperation->execute();
    marshaller->load(value, &m_value);
    return status;  
}

template <typename TYPE,typename TYPE2>
int RemoteCache::getWithMetadata(const TYPE key, TYPE2 value, Metadata *meta){
    if(transportFactory->get_hotrod_version() < VERSION_12) return ERROR_NOT_IMPLEMENTED;
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);
    GetWithMetadataOperation *getWithMetadataOperation = new GetWithMetadataOperation(&m_key, &m_value, meta, *transportFactory, &cache_name, flags);
    
    status = getWithMetadataOperation->execute();
    marshaller->load(value, &m_value);
    return status;    
}


template <typename TYPE,typename TYPE2>
int RemoteCache::put(const TYPE key,const TYPE2 value,int lifespan, int maxidle){
    std::string prev_value;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);
    marshaller->dump(&m_value, value);

    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    PutOperation *putOperation = new PutOperation(&m_key, &m_value, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return putOperation->execute();
};

template <typename TYPE,typename TYPE2>
int RemoteCache::putIfAbsent(const TYPE key,const TYPE2 value,int lifespan, int maxidle){
    std::string prev_value;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);
    marshaller->dump(&m_value, value);
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    PutIfAbsentOperation *putIfAbsentOperation = new PutIfAbsentOperation(&m_key, &m_value, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return putIfAbsentOperation->execute();
}

template <typename TYPE,typename TYPE2>
int RemoteCache::replace(const TYPE key,const TYPE2 value,int lifespan, int maxidle){
    std::string prev_value;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);
    marshaller->dump(&m_value, value);
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    ReplaceOperation *replaceOperation = new ReplaceOperation(&m_key, &m_value, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return replaceOperation->execute();
}

template <typename TYPE,typename TYPE2>
int RemoteCache::replaceWithVersion(const TYPE key, const TYPE2 value, long long version,int lifespan, int maxidle){
    /**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been replaced
    */
    std::string prev_value;
    std::string m_value, m_key;
    marshaller->dump(&m_key, key);
    marshaller->dump(&m_value, value);
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    ReplaceIfUnmodifiedOperation *replaceIfUnmodifiedOperation = new ReplaceIfUnmodifiedOperation(&m_key, &m_value, &prev_value, version, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return replaceIfUnmodifiedOperation->execute();
}

template <typename TYPE>
int RemoteCache::remove(const TYPE key){
    std::string prev_value;
    std::string m_key;
    marshaller->dump(&m_key, key);
    RemoveOperation *removeOperation = new RemoveOperation(&m_key, &prev_value, *transportFactory, &cache_name, flags);
    return removeOperation->execute();
}

template <typename TYPE>
int RemoteCache::removeWithVersion(const TYPE key, long long version){
    /**
    * Removes the given entry only if its version matches the supplied version.
    *
    * @param key key to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been removed
    */
    std::string prev_value;
    std::string m_key;
    marshaller->dump(&m_key, key);
    RemoveIfUnmodifiedOperation *removeIfUnmodifiedOperation = new RemoveIfUnmodifiedOperation(&m_key, &prev_value, version, *transportFactory, &cache_name, flags);
    return removeIfUnmodifiedOperation->execute();
    
}

template <typename TYPE>
int RemoteCache::containsKey(const TYPE key){
    std::string m_key;
    marshaller->dump(&m_key, key);
    ContainsKeyOperation *containsKeyOperation = new ContainsKeyOperation(&m_key, *transportFactory, &cache_name, flags);
    return containsKeyOperation->execute();
}





#endif