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
    int lifespan, maxidle, flags;
    int created_marshaller;
    std::string cache_name;
    void init(RemoteCacheConfig* remote_cache_config);
  public:
    RemoteCache(RemoteCacheConfig* remote_cache_config);
    RemoteCache(std::string host, int port);
    RemoteCache(std::string host);
    RemoteCache(void);
    ~RemoteCache(void);
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

    int replace(VarItem key, VarItem value,int lifespan=-1, int maxidle=-1);

    int replaceWithVersion(VarItem key,VarItem value,long long version,int lifespan=-1, int maxidle=-1);

    template <typename TYPE>
    int replaceWithVersion(VarItem key,VarItem value, TYPE target_prev_value, long long version,int lifespan=-1, int maxidle=-1);

    int remove(const VarItem key);
    template <typename TYPE>
    int removeWithVersion(const TYPE key, long long version);
    template <typename TYPE>
    int containsKey(const TYPE key);


    

    template <typename TYPE,typename TYPE2>
    int get(const TYPE key, TYPE2 value);


    template <typename TYPE,typename TYPE2>
    int getWithVersion(const TYPE key, TYPE2 value,long long *version);

    template <typename TYPE,typename TYPE2>
    int getWithMetadata(const TYPE key, TYPE2 value, RemoteEntryMetadata *meta);

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
    marshaller->dump(key, &m_key);

    GetOperation *getOperation = new GetOperation(&m_key, &m_value, *transportFactory, &cache_name, flags);
    
    status = getOperation->execute();
    marshaller->load(&m_value, value);
    return status;   
};


template <typename TYPE,typename TYPE2>
int RemoteCache::getWithVersion(const TYPE key, TYPE2 value,long long *version){
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(key, &m_key);
    GetWithVersionOperation *getWithVersionOperation = new GetWithVersionOperation(&m_key, &m_value, version, *transportFactory, &cache_name, flags);
    
    status = getWithVersionOperation->execute();
    marshaller->load(&m_value, value);
    return status;  
}

template <typename TYPE,typename TYPE2>
int RemoteCache::getWithMetadata(const TYPE key, TYPE2 value, RemoteEntryMetadata *meta){
    if(transportFactory->get_hotrod_version() < VERSION_12) return NOT_SUPPORTED_VERSION_STATUS;
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(key, &m_key);
    GetWithRemoteEntryMetadataOperation *getWithRemoteEntryMetadataOperation = new GetWithRemoteEntryMetadataOperation(&m_key, &m_value, meta, *transportFactory, &cache_name, flags);
    
    status = getWithRemoteEntryMetadataOperation->execute();
    marshaller->load(&m_value, value);
    return status;    
}


template <typename TYPE,typename TYPE2>
int RemoteCache::put(const TYPE key,const TYPE2 value,int lifespan, int maxidle){
    std::string prev_value;
    std::string m_value, m_key;
    marshaller->dump(key, &m_key);
    marshaller->dump(value, &m_value);

    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    PutOperation *putOperation = new PutOperation(&m_key, &m_value, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return putOperation->execute();
};

template <typename TYPE,typename TYPE2>
int RemoteCache::putIfAbsent(const TYPE key,const TYPE2 value,int lifespan, int maxidle){
    std::string prev_value;
    std::string m_value, m_key;
    marshaller->dump(key, &m_key);
    marshaller->dump(value, &m_value);
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    PutIfAbsentOperation *putIfAbsentOperation = new PutIfAbsentOperation(&m_key, &m_value, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return putIfAbsentOperation->execute();
}



template <typename TYPE>
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, TYPE target_prev_value, long long version,int lifespan, int maxidle){
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
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    ReplaceIfUnmodifiedOperation *replaceIfUnmodifiedOperation = new ReplaceIfUnmodifiedOperation(&key.marshalled, 
                                                                                                &value.marshalled, 
                                                                                                &prev_value, version, 
                                                                                                *transportFactory, 
                                                                                                &cache_name, 
                                                                                                flags|FORCE_RETURN_PREVIOUS_VALUE, 
                                                                                                lifespan, 
                                                                                                maxidle);
    marshaller->load(&prev_value, target_prev_value);
    return replaceIfUnmodifiedOperation->execute();
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
    marshaller->dump(key, &m_key);
    RemoveIfUnmodifiedOperation *removeIfUnmodifiedOperation = new RemoveIfUnmodifiedOperation(&m_key, &prev_value, version, *transportFactory, &cache_name, flags);
    return removeIfUnmodifiedOperation->execute();
    
}

template <typename TYPE>
int RemoteCache::containsKey(const TYPE key){
    std::string m_key;
    marshaller->dump(key, &m_key);
    ContainsKeyOperation *containsKeyOperation = new ContainsKeyOperation(&m_key, *transportFactory, &cache_name, flags);
    return containsKeyOperation->execute();
}





#endif