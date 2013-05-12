#include <remoteCache.h>


RemoteCacheConfig::RemoteCacheConfig(){
    version = 12;
    lifespan = 0;
    maxidle = 0;
    flags = 0x00;
    cache_name = "";
    host = "127.0.0.1";
    port = 11222;
    intelligence = CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE;
}

/**
    * Constructor of RemoteCache.
    *
    * @param remote_cache_config configurating object
    *
    * @throw value of constant representing exception
*/
RemoteCache::RemoteCache(RemoteCacheConfig* remote_cache_config){
    init(remote_cache_config);
}
/**
    * Constructor of RemoteCache. Using default Hot Rot protocol v 1.2
    *
    * @param host address of host
    * @param port port of host
    *
    * @throw value of constant representing exception
*/
RemoteCache::RemoteCache(std::string host, int port){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.host = host;
    remote_cache_config.host = port;
    init(&remote_cache_config);
}
/**
    * Constructor of RemoteCache. Using default port 11222, Hot Rot protocol v 1.2
    *
    * @param host address of host
    *
    * @throw value of constant representing exception
*/
RemoteCache::RemoteCache(std::string host){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.host = host;
    init(&remote_cache_config);
}
/**
    * Constructor of RemoteCache. Using default addres 127.0.0.1:11222, Hot Rot protocol v 1.2
    *
    * @throw value of constant representing exception
*/
RemoteCache::RemoteCache(){
    RemoteCacheConfig remote_cache_config;
    init(&remote_cache_config);
}
//Destructor of RemoteCache.
RemoteCache::~RemoteCache(){
    close();
    delete transportFactory;
    if(created_marshaller){
        delete marshaller;
    }
}

/**
    * Retrieves the version of Hot Rod protocol
    *
    * @return a version
*/
int RemoteCache::getVersion(){
    return transportFactory->get_hotrod_version();
}
/**
    * Retrieves number of copy of each entry. In parallel use of client you should't 
    * create more threads then number of key owners.
    *
    * @return number of key owners
*/
int RemoteCache::getKeyOwnersNum(){
    return transportFactory->get_key_owners_num();
}

//initialize RemoteCache and do a ping operation
void RemoteCache::init(RemoteCacheConfig* remote_cache_config){
    int status;
    srand (time(NULL));
    created_marshaller = 0;
    transportFactory = new TransportFactory(remote_cache_config->host, remote_cache_config->port, remote_cache_config->version, remote_cache_config->intelligence);
    if(DEFAULT_MARSHALLER != NULL){
        marshaller = DEFAULT_MARSHALLER;
    }else{
        created_marshaller = 1;
        marshaller = new MarshallerJBoss();
    }
    default_lifespan = remote_cache_config->lifespan;
    default_maxidle = remote_cache_config->maxidle;
    default_flags = remote_cache_config->flags;
    cache_name = remote_cache_config->cache_name;
    
    status = ping();
    if(status != NO_ERROR_STATUS){
        throw status;
    }
}

 /**
    * Do a ping operation. To check if server si available. Servers are selected round-robin like.
    *
    * @return 0 if the ping was succesfull
*/
int RemoteCache::ping(){
    PingOperation pingOperation(*transportFactory,
                                 &cache_name,
                                 default_flags);
    return pingOperation.execute();
}


 /**
    * Removes all of the entries from this cache. The cache will be empty after this call returns.
    *
    * @return 0 if the clear was succesfull
*/
int RemoteCache::clear(){
    ClearOperation clearOperation(*transportFactory,
                                 &cache_name,
                                 default_flags);
    return clearOperation.execute();
}

/**
    * Replaces the given value.
    *
    * @param key key to use
    * @param value value to use
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return 0 if the value has been replaced
*/
int RemoteCache::replace(const VarItem key, const VarItem value, int lifespan, int maxidle){
    std::string prev_value; //has no effect
    return replace(key, value, &prev_value, lifespan, maxidle, default_flags);
}
/**
    * @see #replace(const VarItem key, const VarItem value, int lifespan, int maxidle)
*/
int RemoteCache::replace(const VarItem key, const VarItem value, int lifespan){
    std::string prev_value; //has no effect
    return replace(key, value, &prev_value, lifespan, -1, default_flags);
}
/**
    * @see #replace(const VarItem key, const VarItem value, int lifespan, int maxidle)
*/
int RemoteCache::replace(const VarItem key, const VarItem value){
    std::string prev_value; //has no effect
    return replace(key, value, &prev_value, -1, -1, default_flags);
}

/**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return 0 if the value has been replaced
*/
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version,int lifespan, int maxidle){
    std::string prev_value; //has no effect
    return replaceWithVersion(key, value, version, &prev_value, lifespan, maxidle, default_flags);
}
/**
    * @see #replaceWithVersion(VarItem key, VarItem value, long long version,int lifespan)
*/
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version,int lifespan){
    std::string prev_value; //has no effect
    return replaceWithVersion(key, value, version, &prev_value, lifespan, -1, default_flags);
}
/**
    * @see #replaceWithVersion(VarItem key, VarItem value, long long version,int lifespan)
*/
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version){
    std::string prev_value; //has no effect
    return replaceWithVersion(key, value, version, &prev_value, -1, -1, default_flags);
}

/**
    * Puts entry to Infinispan cache.
    *
    * @param key        key to use
    * @param value      value to store
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return status of operation
*/
int RemoteCache::put(const VarItem key, const VarItem value, int lifespan, int maxidle){
    std::string prev_value; //has no effect
    return put(key, value, &prev_value, lifespan, maxidle, default_flags);
}
/**
    * @see #put(const VarItem key, const VarItem value, int lifespan, int maxidle)
*/
int RemoteCache::put(const VarItem key, const VarItem value, int lifespan){
    
    std::string prev_value; //has no effect
    return put(key, value, &prev_value, lifespan, -1, default_flags);
}
/**
    * @see #put(const VarItem key, const VarItem value, int lifespan, int maxidle)
*/
int RemoteCache::put(const VarItem key, const VarItem value){
   
    std::string prev_value; //has no effect
    return put(key, value, &prev_value, -1, -1, default_flags);
}
/**
    * Puts multiple entries to Infinispan cache.
    *
    * @param data       map of entries key x value
    * @param lifespan   lifespan of the entrie.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time every key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return status of operation
*/
int RemoteCache::putAll(std::map<VarItem,VarItem> *data, int lifespan, int maxidle){
    std::map<VarItem,VarItem>::iterator pos;
    int state, tmp_state = NO_ERROR_STATUS;
    if(lifespan < 0){lifespan = this->default_lifespan;}
    if(maxidle < 0){maxidle = this->default_maxidle;}
    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
       tmp_state = put(&pos->first,&pos->second,lifespan,maxidle);
       if(tmp_state > NO_ERROR_STATUS){
            state = tmp_state;
       }
    }

    return state;
}

static void *put_provider( void * t_a)
{
    thread_args *t_args;
    t_args = (thread_args *)t_a;

    // std::cout << *t_args->key <<std::flush<<std::endl;
    return (void * )(t_args->RC->put(t_args->key,t_args->value,t_args->lifespan,t_args->maxidle));
}
/**
    * Asynchronous version of #putAll(std::map<VarItem,VarItem> *data, int lifespan, int maxidle)
*/
int RemoteCache::putAllAsync(std::map<VarItem,VarItem> *data, int lifespan, int maxidle){
    #ifdef NOT_THREAD_SAFE
        return putAll(data, lifespan, maxidle);
    #endif
    // mel by vracet i chyby, asi pocet neulozenych polozek
    int max_threads = transportFactory->get_key_owners_num();
    if(lifespan < 0){lifespan = this->default_lifespan;}
    if(maxidle < 0){maxidle = this->default_maxidle;}
    std::map<VarItem,VarItem>::iterator pos;
    int *rets = new int[data->size()];
    pthread_t *threads = new pthread_t[data->size()];
    thread_args *t_args = new thread_args[data->size()];
    int i =0;
    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {

        t_args[i].RC = this;
        t_args[i].key = &pos->first;
        t_args[i].value = &pos->second;
        t_args[i].lifespan = lifespan;
        t_args[i].maxidle = maxidle;

        rets[i] = pthread_create( &threads[i], NULL, put_provider, (void*) &t_args[i]);

        i++;

        if(i>= max_threads){ // limit number of threads
            pthread_join( threads[i-max_threads], NULL);

        }

    }
    int start = data->size() - max_threads;
    if(start < 0){start = 0;}

    for(int j = start;j<data->size();j++){
        pthread_join( threads[j], NULL);
        // std::cout<<"Thread "<<j<<" returns: "<<rets[j]<<std::endl;

    }
    delete [] rets;
    delete [] t_args; 
    delete [] threads;

      
    // std::cout << "done"<<std::endl;
    return 0;
}

/**
    * Puts entry to Infinispan cache only if cache doesn't contains specified key.
    *
    * @param key        key to use
    * @param value      value to store
    * @param lifespan   lifespan of the entry.  Negative values are interpreted as default lifespan.
    * @param maxIdle    the maximum amount of time this key is allowed to be idle for before it is considered as
    *                        expired. Negative values are interpreted as default maxidle.
    * @return status of operation
*/
int RemoteCache::putIfAbsent(const VarItem key, const VarItem value, int lifespan, int maxidle){
    std::string prev_value; //has no effect
    return putIfAbsent(key, value, &prev_value, lifespan, maxidle, default_flags);
}
/**
    *@see #putIfAbsent(const VarItem key, const VarItem value, int lifespan, int maxidle)
*/
int RemoteCache::putIfAbsent(const VarItem key, const VarItem value, int lifespan){
    std::string prev_value; //has no effect
    return putIfAbsent(key, value, &prev_value, lifespan, -1, default_flags);
}
/**
    *@see #putIfAbsent(const VarItem key, const VarItem value, int lifespan, int maxidle)
*/
int RemoteCache::putIfAbsent(const VarItem key, const VarItem value){
    std::string prev_value; //has no effect
    return putIfAbsent(key, value, &prev_value, -1, -1, default_flags);
}


/**
    * Removes the given value.
    *
    * @param key key to use
    *
    * @return 0 if the value has been removed
*/
int RemoteCache::remove(const VarItem key){
    std::string prev_value; //has no effect
    return remove(key, &prev_value, default_flags);
}
/**
    * Removes the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param version numeric version that should match the one in the server
    *
    * @return 0 if the value has been removed
*/
int RemoteCache::removeWithVersion(const VarItem key, long long version){
    std::string prev_value; //has no effect
    return removeWithVersion(key, version, &prev_value, default_flags);
}


/**
    * Retrieves all the entries from cache.
    * \warning doesn't support reduce map
    *
    * @param target pointer for saving map
    * @param count number of entries to retrieve, 0 - unlimited
    *
    * @return 0 if operation was succesfull
*/
int RemoteCache::getBulk(std::map<VarItem,VarItem> *target, int count){
    GetBulkOperation getBulkOperation(target,
                                     count,
                                     *transportFactory,
                                     &cache_name,
                                     default_flags);
    return getBulkOperation.execute();    
}
/**
    *@see #getBulk(std::map<VarItem,VarItem> *target, int count)
*/
int RemoteCache::getBulk(std::map<VarItem,VarItem> *target){
    return getBulk(target, 0); 
}


 /**
    * Key set operations, returns all keys in the remote server.
    *
    * @param target pointer for saving vector
    * @param scope 0,1 - Global scope , 2 - Local scope
    *
    * @return 0 if operation was succesfull
*/
int RemoteCache::keySet(std::vector<VarItem> *target,int scope){
    if(transportFactory->get_hotrod_version() < VERSION_12) return NOT_IN_THIS_VERSION_STATUS;
   
    BulkKeysGetOperation bulkKeysGetOperation(target,
                                             scope,
                                             *transportFactory,
                                             &cache_name,
                                             default_flags);
    return bulkKeysGetOperation.execute();  
    
}


/**
    * Retrieves statistics from server. Stats are saved to map stat_name x stat_value.
    *
    * @param stats pointer to return map of stats
    *
    * @return 0 if the stats was succesfull
*/
int RemoteCache::stats(std::map<std::string,std::string> *stats){
    StatsOperation statsOperation(stats,
                                 *transportFactory,
                                 &cache_name,
                                 default_flags);
    return statsOperation.execute();  
}

/**
    * Checks if cache cointains key.
    *
    * @param key key to use
    *
    * @return 0 if contains key else return state
*/
int RemoteCache::containsKey(const VarItem key){
    ContainsKeyOperation containsKeyOperation(&key.marshalled,
                                             *transportFactory,
                                             &cache_name,
                                             default_flags);
    return containsKeyOperation.execute();
}


/**
    * Close all connections to the servers.
*/
void RemoteCache::close(){
    transportFactory->close_servers();
}


