#include <remoteCache.h>

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
    if(DEFAULT_MARSHALLER != NULL){
        marshaller = DEFAULT_MARSHALLER;
    }else{
        marshaller = new MarshallerJBoss();
    }
    lifespan = remote_cache_config->lifespan;
    maxidle = remote_cache_config->maxidle;
    flags = remote_cache_config->flags;
    cache_name = remote_cache_config->cache_name;

    ping();
}

int RemoteCache::ping(){
    PingOperation *pingOperation = new PingOperation(*transportFactory, &cache_name, flags);
    return pingOperation->execute();
}



int RemoteCache::clear(){
    ClearOperation *clearOperation = new ClearOperation(*transportFactory, &cache_name, flags);
    return clearOperation->execute();
}



static void *print_message_function( void * t_a)
{
    // thread_args *t_args;
    // t_args = (thread_args *)t_a;

    // //std::cout << *t_args->key <<std::flush<<std::endl;
    // return (void * )(t_args->RC->put(t_args->key,t_args->value,t_args->lifespan,t_args->maxidle));
}

int RemoteCache::putAllAsync(std::map<VarItem,VarItem> *data,int lifespan, int maxidle){
    // int max_threads = 10;
    // std::map<VarItem,VarItem>::iterator pos;
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

int RemoteCache::putAll(std::map<VarItem,VarItem> *data,int lifespan, int maxidle){
    std::map<VarItem,VarItem>::iterator pos;

    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
       // print_servers();
       put(&pos->first,&pos->second,lifespan,maxidle);
    }

    return 0;
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
    GetBulkOperation *getBulkOperation = new GetBulkOperation(bulk, count, *transportFactory, &cache_name, flags);
    return getBulkOperation->execute();  
    
}

int RemoteCache::keySet(std::vector<VarItem> *keys,int scope){
    /**
    * Key set operations, returns all keys in the remote server.
    *
    * @param scope 1 - Global scope , 2 - Local scope
    * @return returns Vector of string
    */
    if(transportFactory->get_hotrod_version() < VERSION_12) return ERROR_NOT_IMPLEMENTED;
   
    BulkKeysGetOperation *bulkKeysGetOperation = new BulkKeysGetOperation(keys, scope, *transportFactory, &cache_name, flags);
    return bulkKeysGetOperation->execute();  
    
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


