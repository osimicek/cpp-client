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








/**
    * Implements configuration object for RemoteCache.
    *
    * @author ondrejsimicek@seznam.cz
*/
class RemoteCacheConfig{
    public:
        /** Version of Hot Rod protocol */
        int version;
        /** Default lifespan */
        int lifespan;
        /** Default maxidle */
        int maxidle;
        /** Default flags */
        int flags;
        /** Cache name to use */
        std::string cache_name;
        /** Host address */
        std::string host;
        /** Host port */
        int port;
        /** Inteligence to use */
        char intelligence;
        RemoteCacheConfig();

};

/**
    * Implements "RemoteCache" as interface of Infinispan cache
    *
    * @author ondrejsimicek@seznam.cz
*/
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
    
    /**
    * Puts entry to Infinispan cache and returns previous value.
    *
    * @param key        key to use
    * @param value      value to store
    * @param target_prev_value      pointer for saving previous value
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return status of operation
    */
    template <typename TYPE>
    int put(const VarItem key,const VarItem value, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int put(const VarItem key,const VarItem value, int lifespan, int maxidle);
    int put(const VarItem key,const VarItem value, int lifespan);
    int put(const VarItem key,const VarItem value);

    int putAll(std::map<VarItem,VarItem> *data,int lifespan=-1, int maxidle=-1);
    int putAllAsync(std::map<VarItem,VarItem> *data,int lifespan=-1, int maxidle=-1);

    /**
    * Puts entry to Infinispan cache only if cache doesn't contains specified key and returns previous value;
    *
    * @param key        key to use
    * @param value      value to store
    * @param target_prev_value      pointer for saving previous value
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return status of operation
*/
    template <typename TYPE>
    int putIfAbsent(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int putIfAbsent(const VarItem key, const VarItem value, int lifespan, int maxidle);
    int putIfAbsent(const VarItem key, const VarItem value, int lifespan);
    int putIfAbsent(const VarItem key, const VarItem value);

    /**
    * Replaces the given value and returns previous value.
    *
    * @param key key to use
    * @param value value to use
    * @param target_prev_value      pointer for saving previous value
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return 0 if the value has been replaced
    */
    template <typename TYPE>
    int replace(const VarItem key, const VarItem value, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int replace(VarItem key, VarItem value, int lifespan, int maxidle);
    int replace(VarItem key, VarItem value, int lifespan);
    int replace(VarItem key, VarItem value);

    /**
    * Replaces the given value only if its version matches the supplied version and returns previous value.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @param target_prev_value      pointer for saving previous value
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return 0 if the value has been replaced
    */
    template <typename TYPE>
    int replaceWithVersion(VarItem key, VarItem value, long long version, TYPE target_prev_value, int lifespan=-1, int maxidle=-1);
    int replaceWithVersion(VarItem key, VarItem value, long long version, int lifespan, int maxidle);
    int replaceWithVersion(VarItem key, VarItem value, long long version, int lifespan);
    int replaceWithVersion(VarItem key, VarItem value, long long version);

    /**
    * Removes the given value and returns previous value.
    *
    * @param key key to use
    * @param target_prev_value      pointer for saving previous value
    *
    * @return 0 if the value has been removed
    */
    template <typename TYPE>
    int remove(const VarItem key, TYPE target_prev_value);
    int remove(const VarItem key);

    /**
    * Removes the given value only if its version matches the supplied version and returns previous value.
    *
    * @param key key to use
    * @param version numeric version that should match the one in the server
    * @param target_prev_value      pointer for saving previous value
    *
    * @return 0 if the value has been removed
    */
    template <typename TYPE>
    int removeWithVersion(const VarItem key, long long version, TYPE target_prev_value);
    int removeWithVersion(const VarItem key, long long version);


    int containsKey(const VarItem key);

    /**
    * Retrieves value from server.
    *
    * @param key key to use
    * @param target pointer for saving value
    *
    * @return 0 if the get was succesfull
    */
    template <typename TYPE>
    int get(const VarItem key, TYPE target);

    /**
    * Retrieves value and its version from server.
    * 
    *
    * @param key key to use
    * @param target pointer for saving value
    * @param target_version pointer for saving version
    *
    * @return 0 if the get was succesfull
    */
    template <typename TYPE>
    int getWithVersion(const VarItem key, TYPE target,long long *target_version);

     /**
    * Retrieves value and its metadata from server.
    * \warning Supported from Hot Rod 1.2
    *
    * @param key key to use
    * @param target pointer for saving value
    * @param target_meta pointer for saving metadata
    *
    * @return 0 if the get was succesfull
    */
    template <typename TYPE>
    int getWithMetadata(const VarItem key, TYPE target, RemoteEntryMetadata *target_meta);

    int getBulk(std::map<VarItem,VarItem> *target);
    int getBulk(std::map<VarItem,VarItem> *target, int count);

    int keySet(std::vector<VarItem> *target, int scope=0);

    void close();

};

template <typename TYPE>
int RemoteCache::get(const VarItem key, TYPE target){
    int status;
    std::string m_value;
    GetOperation getOperation(&key.marshalled,
                             &m_value,
                             *transportFactory,
                             &cache_name,
                             default_flags);
    
    status = getOperation.execute();
    marshaller->load(&m_value, target);
    return status;   
};


template <typename TYPE>
int RemoteCache::getWithVersion(const VarItem key, TYPE target,long long *target_version){
    int status;
    std::string m_value;
    GetWithVersionOperation getWithVersionOperation(&key.marshalled,
                                                 &m_value,
                                                 target_version,
                                                 *transportFactory,
                                                 &cache_name,
                                                 default_flags);
    
    status = getWithVersionOperation.execute();
    marshaller->load(&m_value, target);
    return status;  
}

template <typename TYPE>
int RemoteCache::getWithMetadata(const VarItem key, TYPE target, RemoteEntryMetadata *target_meta){
    if(transportFactory->get_hotrod_version() < VERSION_12) return NOT_IN_THIS_VERSION_STATUS;
    int status;
    std::string m_value;
    GetWithRemoteEntryMetadataOperation getWithRemoteEntryMetadataOperation(&key.marshalled,
                                                                         &m_value,
                                                                         target_meta,
                                                                         *transportFactory,
                                                                         &cache_name,
                                                                         default_flags);
    
    status = getWithRemoteEntryMetadataOperation.execute();
    marshaller->load(&m_value, target);
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







#endif