#include <remoteCache2.h>

RemoteCacheConfig::RemoteCacheConfig(){
    version = 11;
    lifespan = 0;
    maxidle = 0;
    flags = 0x00;
    cache_name = "";
    host = "127.0.0.1";
    port = 11222;
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

void RemoteCache::init(RemoteCacheConfig* remote_cache_config){
    srand (time(NULL));
    transportFactory = new TransportFactory(remote_cache_config->host, remote_cache_config->port, remote_cache_config->version);

    lifespan = remote_cache_config->lifespan;
    maxidle = remote_cache_config->maxidle;
    flags = remote_cache_config->flags;
    cache_name = remote_cache_config->cache_name;
}

int RemoteCache::ping(){
    PingOperation *pingOperation = new PingOperation(*transportFactory, &cache_name, flags);
    return pingOperation->execute();
}



int RemoteCache::clear(){
    ClearOperation *clearOperation = new ClearOperation(*transportFactory, &cache_name, flags);
    return clearOperation->execute();
}
int RemoteCache::put(const char *key,const char *value,int lifespan, int maxidle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return put(&tmp_key,&tmp_value,lifespan,maxidle);
}


int RemoteCache::put(const std::string *key,const std::string *value,int lifespan, int maxidle){
    std::string prev_value;
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    PutOperation *putOperation = new PutOperation(value, key, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return putOperation->execute();
}




static void *print_message_function( void * t_a)
{
    // thread_args *t_args;
    // t_args = (thread_args *)t_a;

    // //std::cout << *t_args->key <<std::flush<<std::endl;
    // return (void * )(t_args->RC->put(t_args->key,t_args->value,t_args->lifespan,t_args->maxidle));
}

int RemoteCache::putAllAsync(std::map<std::string,std::string> *data,int lifespan, int maxidle){
    // int max_threads = 10;
    // std::map<std::string,std::string>::iterator pos;
    // int *rets = new int[data->size()];
    // pthread_t *threads = new pthread_t[data->size()];
    // thread_args *t_args = new thread_args[data->size()];
    // int i =0;
    // for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
    //     t_args[i].RC = this;
    //     t_args[i].key = &pos->first;
    //     t_args[i].value = &pos->second;
    //     t_args[i].lifespan = lifespan;
    //     t_args[i].maxidle = maxidle;

    //     rets[i] = pthread_create( &threads[i], NULL, print_message_function, (void*) &t_args[i]);

    //     i++;

    //     if(i>= max_threads){
    //         pthread_join( threads[i-max_threads], NULL);

    //     }

    // }
    // int start = data->size() - max_threads;
    // if(start < 0){start = 0;}

    // for(int j = start;j<data->size();j++){
    //     pthread_join( threads[j], NULL);
    //     printf("Thread %d returns: %d\n",j,rets[j]);

    // }
    // std::cout << "done"<<std::endl;
    return 0;
}

int RemoteCache::putAll(std::map<std::string,std::string> *data,int lifespan, int maxidle){
    std::map<std::string,std::string>::iterator pos;

    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
       // print_servers();
       put(&pos->first,&pos->second,lifespan,maxidle);
    }

    return 0;
}

int RemoteCache::putIfAbsent(const char *key,const char *value,int lifespan, int maxidle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return putIfAbsent(&tmp_key,&tmp_value,lifespan,maxidle);
}


int RemoteCache::putIfAbsent(const std::string *key,std::string *value,int lifespan, int maxidle){
    std::string prev_value;
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    PutIfAbsentOperation *putIfAbsentOperation = new PutIfAbsentOperation(value, key, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return putIfAbsentOperation->execute();
}

int RemoteCache::replace(const char *key,const char *value,int lifespan, int maxidle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return replace(&tmp_key,&tmp_value,lifespan,maxidle);
}


int RemoteCache::replace(const std::string *key,std::string *value,int lifespan, int maxidle){
    std::string prev_value;
    if(lifespan < 0){lifespan = this->lifespan;}
    if(maxidle < 0){maxidle = this->maxidle;}
    ReplaceOperation *replaceOperation = new ReplaceOperation(value, key, &prev_value, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return replaceOperation->execute();
}

int RemoteCache::replaceWithVersion(const char *key,const char *value, long long version,int lifespan, int maxidle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return replaceWithVersion(&tmp_key,&tmp_value, version,lifespan,maxidle);
}


int RemoteCache::replaceWithVersion(const std::string *key, const std::string *value, long long version,int lifespan, int maxidle){
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
    ReplaceIfUnmodifiedOperation *replaceIfUnmodifiedOperation = new ReplaceIfUnmodifiedOperation(value, key, &prev_value, version, *transportFactory, &cache_name, flags, lifespan, maxidle);
    return replaceIfUnmodifiedOperation->execute();
}
int RemoteCache::get(char* value, const char *key){
    std::string tmp_key(key);
    std::string val; //zmenit
    int ret;

    ret = get(&val,&tmp_key);
    strcpy(value, val.c_str()); 
    return ret;
}

int RemoteCache::get(std::string *value, const std::string *key){

    GetOperation *getOperation = new GetOperation(value, key, *transportFactory, &cache_name, flags);
    return getOperation->execute();
    
}

int RemoteCache::remove(const char *key){
    std::string tmp_key(key);
    return remove(&tmp_key);
}

int RemoteCache::remove(const std::string *key){
    std::string prev_value;
    RemoveOperation *removeOperation = new RemoveOperation(&prev_value, key, *transportFactory, &cache_name, flags);
    return removeOperation->execute();
}

int RemoteCache::removeWithVersion(const char *key, long long version){
    std::string tmp_key(key);
    return removeWithVersion(&tmp_key,version);
}

int RemoteCache::removeWithVersion(const std::string *key, long long version){
    /**
    * Removes the given entry only if its version matches the supplied version.
    *
    * @param key key to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been removed
    */
    std::string prev_value;
    RemoveIfUnmodifiedOperation *removeIfUnmodifiedOperation = new RemoveIfUnmodifiedOperation(&prev_value, key, version, *transportFactory, &cache_name, flags);
    return removeIfUnmodifiedOperation->execute();
    
}

int RemoteCache::containsKey(const char *key){
    std::string tmp_key(key);
    return containsKey(&tmp_key);
}

int RemoteCache::containsKey(const std::string *key){
    ContainsKeyOperation *containsKeyOperation = new ContainsKeyOperation(key, *transportFactory, &cache_name, flags);
    return containsKeyOperation->execute();
}

int RemoteCache::getWithVersion(char *value, const char *key,long long *version){
    std::string tmp_key(key);
    std::string val;  //zmenit
    int ret;

    ret = getWithVersion(&val,&tmp_key,version);
    strcpy(value, val.c_str()); 
    return ret;
}

int RemoteCache::getWithVersion(std::string *value,const std::string *key,long long *version){
    GetWithVersionOperation *getWithVersionOperation = new GetWithVersionOperation(value, key, version, *transportFactory, &cache_name, flags);
    return getWithVersionOperation->execute();
}

int RemoteCache::getWithMetadata(std::string *value, Metadata *meta, const std::string *key){
    GetWithMetadataOperation *getWithMetadataOperation = new GetWithMetadataOperation(value, meta, key, *transportFactory, &cache_name, flags);
    return getWithMetadataOperation->execute();   
}

int RemoteCache::getBulk(std::map<std::string,std::string> *bulk,int count){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @param count maximal number of returned entries
    * @return returns Map of string
    */
    GetBulkOperation *getBulkOperation = new GetBulkOperation(bulk, count, *transportFactory, &cache_name, flags);
    return getBulkOperation->execute();  
    
}

int RemoteCache::keySet(std::vector<std::string> *keys,int scope){
    /**
    * Key set operations, returns all keys in the remote server.
    *
    * @param scope 1 - Global scope , 2 - Local scope
    * @return returns Vector of string
    */
    BulkKeysGetOperation *bulkKeysGetOperation = new BulkKeysGetOperation(keys, scope, *transportFactory, &cache_name, flags);
    return bulkKeysGetOperation->execute();  
    
}

int RemoteCache::getBulk(std::map<std::string,std::string> *bulk){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @return returns Map of string
    */

    return getBulk(bulk,0);
    
}

int RemoteCache::stats(std::map<std::string,std::string> *stats){
    StatsOperation *statsOperation = new StatsOperation(stats, *transportFactory, &cache_name, flags);
    return statsOperation->execute();  
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
    // transporter->close_servers();

}


