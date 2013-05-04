#include <remoteCache.h>

RemoteCacheConfig::RemoteCacheConfig(){
    version = 11;
    lifespan = 0;
    maxidle = 0;
    flags = 0x00;
    cache_name = "";
    host = "127.0.0.1";
    port = 11222;
    intelligence = CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE;
}

RemoteCache::RemoteCache(RemoteCacheConfig* remote_cache_config){
    init(remote_cache_config);
}

RemoteCache::RemoteCache(std::string host, int port){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.host = host;
    remote_cache_config.host = port;
    init(&remote_cache_config);
}

RemoteCache::RemoteCache(std::string host){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.host = host;
    init(&remote_cache_config);
}

RemoteCache::RemoteCache(){
    RemoteCacheConfig remote_cache_config;
    init(&remote_cache_config);
}
RemoteCache::~RemoteCache(){
    close();
    delete transportFactory;
    if(created_marshaller){
        delete marshaller;
    }
}

int RemoteCache::getVersion(){
    return transportFactory->get_hotrod_version();
}
int RemoteCache::getKeyOwnersNum(){
    return transportFactory->get_key_owners_num();
}

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

int RemoteCache::ping(){
    PingOperation pingOperation(*transportFactory,
                                 &cache_name,
                                 default_flags);
    return pingOperation.execute();
}



int RemoteCache::clear(){
    ClearOperation clearOperation(*transportFactory,
                                 &cache_name,
                                 default_flags);
    return clearOperation.execute();
}


int RemoteCache::replace(const VarItem key, const VarItem value, int lifespan, int maxidle){
    std::string prev_value; //has no effect
    return replace(key, value, &prev_value, lifespan, maxidle, default_flags);
}
int RemoteCache::replace(const VarItem key, const VarItem value, int lifespan){
    std::string prev_value; //has no effect
    return replace(key, value, &prev_value, lifespan, -1, default_flags);
}
int RemoteCache::replace(const VarItem key, const VarItem value){
    std::string prev_value; //has no effect
    return replace(key, value, &prev_value, -1, -1, default_flags);
}


int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version,int lifespan, int maxidle){
    /**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been replaced
    */
    std::string prev_value; //has no effect
    return replaceWithVersion(key, value, version, &prev_value, lifespan, maxidle, default_flags);
}
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version,int lifespan){
    /**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been replaced
    */
    std::string prev_value; //has no effect
    return replaceWithVersion(key, value, version, &prev_value, lifespan, -1, default_flags);
}
int RemoteCache::replaceWithVersion(VarItem key, VarItem value, long long version){
    /**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been replaced
    */
    std::string prev_value; //has no effect
    return replaceWithVersion(key, value, version, &prev_value, -1, -1, default_flags);
}


int RemoteCache::put(const VarItem key, const VarItem value, int lifespan, int maxidle){
    std::string prev_value; //has no effect

    return put(key, value, &prev_value, lifespan, maxidle, default_flags);
}

static void *put_provider( void * t_a)
{
    thread_args *t_args;
    t_args = (thread_args *)t_a;

    // std::cout << *t_args->key <<std::flush<<std::endl;
    return (void * )(t_args->RC->put(t_args->key,t_args->value,t_args->lifespan,t_args->maxidle));
}

int RemoteCache::putAllAsync(std::map<VarItem,VarItem> *data,int lifespan, int maxidle){
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

int RemoteCache::putIfAbsent(const VarItem key, const VarItem value, int lifespan, int maxidle){
    std::string prev_value; //has no effect
    return putIfAbsent(key, value, &prev_value, lifespan, maxidle, default_flags);
}

int RemoteCache::putAll(std::map<VarItem,VarItem> *data,int lifespan, int maxidle){
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

int RemoteCache::remove(const VarItem key){
    std::string prev_value; //has no effect
    return remove(key, &prev_value, default_flags);
}

int RemoteCache::removeWithVersion(const VarItem key, long long version){
    std::string prev_value; //has no effect
    return removeWithVersion(key, version, &prev_value, default_flags);
}

int RemoteCache::getBulk(std::map<VarItem,VarItem> *bulk){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @return returns Map of string
    */

    return getBulk(bulk,0);
    
}

int RemoteCache::getBulk(std::map<VarItem,VarItem> *bulk,int count){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @param count maximal number of returned entries
    * @return returns Map of string
    */
    GetBulkOperation getBulkOperation(bulk,
                                     count,
                                     *transportFactory,
                                     &cache_name,
                                     default_flags);
    return getBulkOperation.execute();  
    
}

int RemoteCache::keySet(std::vector<VarItem> *keys,int scope){
    /**
    * Key set operations, returns all keys in the remote server.
    *
    * @param scope 1 - Global scope , 2 - Local scope
    * @return returns Vector of string
    */
    if(transportFactory->get_hotrod_version() < VERSION_12) return NOT_IN_THIS_VERSION_STATUS;
   
    BulkKeysGetOperation bulkKeysGetOperation(keys,
                                             scope,
                                             *transportFactory,
                                             &cache_name,
                                             default_flags);
    return bulkKeysGetOperation.execute();  
    
}



int RemoteCache::stats(std::map<std::string,std::string> *stats){
    StatsOperation statsOperation(stats,
                                 *transportFactory,
                                 &cache_name,
                                 default_flags);
    return statsOperation.execute();  
}

void RemoteCache::print_servers(){
    // std::cout << transporter->servers.size()<<std::endl; 
    // for(int i = 0;i<transporter->servers.size();i++){
    //     std::cout << std::dec<<transporter->servers.front().host<<"/"<<transporter->servers.front().port<<"  hash:"<<transporter->servers.front().hash<<std::endl;
    //     transporter->servers.push(transporter->servers.front());
    //     transporter->servers.pop();
    // } 
    // std::cout << transporter->servers.size()<<std::endl; 
    // return;
}

void RemoteCache::close(){
    transportFactory->close_servers();
}


