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
    int default_lifespan, default_maxidle, default_flags;
    int created_marshaller;
    std::string cache_name;
    void init(RemoteCacheConfig* remote_cache_config);

    template <typename TYPE>
    int put(const VarItem key,const VarItem value, TYPE target_prev_value, int lifespan, int maxidle, int flags);
    template <typename TYPE>
    int putIfAbsent(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle, int flags);
    template <typename TYPE>
    int replace(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle, int flags);
    template <typename TYPE>
    int replaceWithVersion(VarItem key, VarItem value, long long version, TYPE target_prev_value, int lifespan, int maxidle, int flags);
    template <typename TYPE>
    int remove(const VarItem key, TYPE target_prev_value, int flags);
    template <typename TYPE>
    int removeWithVersion(const VarItem key, long long version, TYPE target_prev_value, int flags);
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
    
    template <typename TYPE>
    int put(const VarItem key,const VarItem value, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int put(const VarItem key,const VarItem value, int lifespan, int maxidle);
    int put(const VarItem key,const VarItem value, int lifespan);
    int put(const VarItem key,const VarItem value);

    int putAll(std::map<VarItem,VarItem> *data,int lifespan=-1, int maxidle=-1);
    int putAllAsync(std::map<VarItem,VarItem> *data,int lifespan=-1, int maxidle=-1);

    template <typename TYPE>
    int putIfAbsent(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int putIfAbsent(const VarItem key, const VarItem value, int lifespan, int maxidle);
    int putIfAbsent(const VarItem key, const VarItem value, int lifespan);
    int putIfAbsent(const VarItem key, const VarItem value);

    template <typename TYPE>
    int replace(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int replace(VarItem key, VarItem value, int lifespan, int maxidle);
    int replace(VarItem key, VarItem value, int lifespan);
    int replace(VarItem key, VarItem value);

    template <typename TYPE>
    int replaceWithVersion(VarItem key, VarItem value, long long version, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int replaceWithVersion(VarItem key, VarItem value, long long version, int lifespan, int maxidle);
    int replaceWithVersion(VarItem key, VarItem value, long long version, int lifespan);
    int replaceWithVersion(VarItem key, VarItem value, long long version);

    template <typename TYPE>
    int remove(const VarItem key, TYPE target_prev_value);
    int remove(const VarItem key);

    template <typename TYPE>
    int removeWithVersion(const VarItem key, long long version, TYPE target_prev_value);
    int removeWithVersion(const VarItem key, long long version);


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

    GetOperation getOperation(&m_key,
                             &m_value,
                             *transportFactory,
                             &cache_name,
                             default_flags);
    
    status = getOperation.execute();
    marshaller->load(&m_value, value);
    return status;   
};


template <typename TYPE,typename TYPE2>
int RemoteCache::getWithVersion(const TYPE key, TYPE2 value,long long *version){
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(key, &m_key);
    GetWithVersionOperation getWithVersionOperation(&m_key,
                                                 &m_value,
                                                 version,
                                                 *transportFactory,
                                                 &cache_name,
                                                 default_flags);
    
    status = getWithVersionOperation.execute();
    marshaller->load(&m_value, value);
    return status;  
}

template <typename TYPE,typename TYPE2>
int RemoteCache::getWithMetadata(const TYPE key, TYPE2 value, RemoteEntryMetadata *meta){
    if(transportFactory->get_hotrod_version() < VERSION_12) return NOT_IN_THIS_VERSION_STATUS;
    int status = 0;
    std::string m_value, m_key;
    marshaller->dump(key, &m_key);
    GetWithRemoteEntryMetadataOperation getWithRemoteEntryMetadataOperation(&m_key,
                                                                         &m_value,
                                                                         meta,
                                                                         *transportFactory,
                                                                         &cache_name,
                                                                         default_flags);
    
    status = getWithRemoteEntryMetadataOperation.execute();
    marshaller->load(&m_value, value);
    return status;    
}


template <typename TYPE>
int RemoteCache::put(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle, int flags){
    std::string prev_value;
    int status;

    if(lifespan < 0){lifespan = this->default_lifespan;}
    if(maxidle < 0){maxidle = this->default_maxidle;}
    PutOperation putOperation(&key.marshalled,
                            &value.marshalled,
                            &prev_value,
                            *transportFactory,
                            &cache_name,
                            flags,
                            lifespan,
                            maxidle);
    status = putOperation.execute();
    marshaller->load(&prev_value, target_prev_value);
    return status;
};

template <typename TYPE>
int RemoteCache::put(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle){
    return put(key, value, target_prev_value, lifespan, maxidle, default_flags|FORCE_RETURN_PREVIOUS_VALUE);
}

template <typename TYPE>
int RemoteCache::putIfAbsent(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle, int flags){
    std::string prev_value;
    int status;
    if(lifespan < 0){lifespan = this->default_lifespan;}
    if(maxidle < 0){maxidle = this->default_maxidle;}
    PutIfAbsentOperation putIfAbsentOperation(&key.marshalled,
                                            &value.marshalled,
                                            &prev_value,
                                            *transportFactory,
                                            &cache_name,
                                            flags,
                                            lifespan,
                                            maxidle);
    status =  putIfAbsentOperation.execute();
    marshaller->load(&prev_value, target_prev_value);
    return status;
}

template <typename TYPE>
int RemoteCache::putIfAbsent(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle){
    return putIfAbsent(key, value, target_prev_value, lifespan, maxidle, default_flags|FORCE_RETURN_PREVIOUS_VALUE);
}

template <typename TYPE>
int RemoteCache::replace(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle, int flags){
    std::string prev_value;
    int status;

    if(lifespan < 0){lifespan = this->default_lifespan;}
    if(maxidle < 0){maxidle = this->default_maxidle;}
    ReplaceOperation replaceOperation(&key.marshalled,
                                        &value.marshalled,
                                        &prev_value,
                                        *transportFactory,
                                        &cache_name,
                                        flags,
                                        lifespan,
                                        maxidle);
    status = replaceOperation.execute();
    marshaller->load(&prev_value, target_prev_value);
    return status;
}

template <typename TYPE>
int RemoteCache::replace(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan, int maxidle){
    return replace(key, value, target_prev_value, lifespan, maxidle, default_flags|FORCE_RETURN_PREVIOUS_VALUE);
}


template <typename TYPE>
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version, TYPE target_prev_value, int lifespan, int maxidle, int flags){
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
    int status;
    if(lifespan < 0){lifespan = this->default_lifespan;}
    if(maxidle < 0){maxidle = this->default_maxidle;}
    ReplaceIfUnmodifiedOperation replaceIfUnmodifiedOperation(&key.marshalled, 
                                                            &value.marshalled, 
                                                            &prev_value, 
                                                            version, 
                                                            *transportFactory, 
                                                            &cache_name, 
                                                            flags, 
                                                            lifespan, 
                                                            maxidle);
    status = replaceIfUnmodifiedOperation.execute();
    marshaller->load(&prev_value, target_prev_value);
    return status;
}

template <typename TYPE>
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version, TYPE target_prev_value, int lifespan, int maxidle){
    return replaceWithVersion(key, value, version, target_prev_value, lifespan, maxidle, default_flags|FORCE_RETURN_PREVIOUS_VALUE);
}

template <typename TYPE>
int RemoteCache::remove(const VarItem key, TYPE target_prev_value, int flags){
    std::string prev_value;
    int status;
    RemoveOperation removeOperation(&key.marshalled, 
                                    &prev_value, 
                                    *transportFactory, 
                                    &cache_name, 
                                    flags);
    status = removeOperation.execute();
    marshaller->load(&prev_value, target_prev_value);
    return status;
}
template <typename TYPE>
int RemoteCache::remove(const VarItem key, TYPE target_prev_value){
    return remove(key, target_prev_value, default_flags|FORCE_RETURN_PREVIOUS_VALUE);
}

template <typename TYPE>
int RemoteCache::removeWithVersion(const VarItem key, long long version, TYPE target_prev_value, int flags){
    /**
    * Removes the given entry only if its version matches the supplied version.
    *
    * @param key key to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been removed
    */
    std::string prev_value;
    int status;

    RemoveIfUnmodifiedOperation removeIfUnmodifiedOperation(&key.marshalled, 
                                                         &prev_value,
                                                         version,
                                                         *transportFactory,
                                                         &cache_name,
                                                         flags);
    status = removeIfUnmodifiedOperation.execute();
    marshaller->load(&prev_value, target_prev_value);
    return status;
}

template <typename TYPE>
int RemoteCache::removeWithVersion(const VarItem key, long long version, TYPE target_prev_value){
    return removeWithVersion(key, version, target_prev_value, default_flags|FORCE_RETURN_PREVIOUS_VALUE);
}

template <typename TYPE>
int RemoteCache::containsKey(const TYPE key){
    std::string m_key;
    marshaller->dump(key, &m_key);
    ContainsKeyOperation containsKeyOperation(&m_key,
                                             *transportFactory,
                                             &cache_name,
                                             default_flags);
    return containsKeyOperation.execute();
}





#endif