#include <remoteCache2.h>

RemoteCacheConfig::RemoteCacheConfig(){
    version = 11;
    lifespan = 0;
    idle = 0;
    flags = 0x00;
    cache_name = "";
    host = "127.0.0.1";
    port = 11222;
}


RemoteCache2::RemoteCache2(RemoteCacheConfig* remote_cache_config){
    init(remote_cache_config);
}

RemoteCache2::RemoteCache2(std::string host, int port){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.host = host;
    remote_cache_config.host = port;
    init(&remote_cache_config);
}

RemoteCache2::RemoteCache2(std::string host){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.host = host;
    init(&remote_cache_config);
}

RemoteCache2::RemoteCache2(){
    RemoteCacheConfig remote_cache_config;
    init(&remote_cache_config);
}

void RemoteCache2::init(RemoteCacheConfig* remote_cache_config){
    srand (time(NULL));
    transportFactory = new TransportFactory(remote_cache_config->host, remote_cache_config->port, remote_cache_config->version);

    lifespan = remote_cache_config->lifespan;
    idle = remote_cache_config->idle;
    flags = remote_cache_config->flags;
    cache_name = remote_cache_config->cache_name;
}

int RemoteCache2::ping(){
    // long long tmp;
    // return ping(&tmp);
}

int RemoteCache2::ping(long long *value){
    // PacketAssembler *PA = new PacketAssembler(transporter);
    // return PA->createPing(value);
}

int RemoteCache2::clear(){
    ClearOperation *clearOperation = new ClearOperation(*transportFactory, &cache_name, flags);
    return clearOperation->execute();
}
int RemoteCache2::put(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return put(&tmp_key,&tmp_value,lifespan,idle);
}


int RemoteCache2::put(const std::string *key,const std::string *value,int lifespan, int idle){
    if(lifespan < 0){lifespan = this->lifespan;}
    if(idle < 0){idle = this->idle;}
    PutOperation *putOperation = new PutOperation(value, *transportFactory, key, &cache_name, flags, lifespan, idle);
    return putOperation->execute();
}




static void *print_message_function( void * t_a)
{
    // thread_args *t_args;
    // t_args = (thread_args *)t_a;

    // //std::cout << *t_args->key <<std::flush<<std::endl;
    // return (void * )(t_args->RC->put(t_args->key,t_args->value,t_args->lifespan,t_args->idle));
}

int RemoteCache2::putAllAsync(std::map<std::string,std::string> *data,int lifespan, int idle){
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
    //     t_args[i].idle = idle;

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

int RemoteCache2::putAll(std::map<std::string,std::string> *data,int lifespan, int idle){
    std::map<std::string,std::string>::iterator pos;

    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
       // print_servers();
       put(&pos->first,&pos->second,lifespan,idle);
    }

    return 0;
}

int RemoteCache2::putIfAbsent(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return putIfAbsent(&tmp_key,&tmp_value,lifespan,idle);
}


int RemoteCache2::putIfAbsent(const std::string *key,std::string *value,int lifespan, int idle){
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createPutIfAbsent(key,value,lifespan,idle);
}

int RemoteCache2::replace(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return replace(&tmp_key,&tmp_value,lifespan,idle);
}


int RemoteCache2::replace(const std::string *key,std::string *value,int lifespan, int idle){
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createReplace(key,value,lifespan,idle);
}

int RemoteCache2::replaceWithVersion(const char *key,const char *value, long long version,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return replaceWithVersion(&tmp_key,&tmp_value, version,lifespan,idle);
}


int RemoteCache2::replaceWithVersion(const std::string *key,std::string *value, long long version,int lifespan, int idle){
    /**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been replaced
    */
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createReplaceWithVersion(key,value,version,lifespan,idle);
}
int RemoteCache2::get(const char* value, const char *key){
    std::string tmp_key(key);
    std::string val; //zmenit
    int ret;

    ret = get(&val,&tmp_key);
    value = (val.c_str());
    return ret;
}

int RemoteCache2::get(std::string *value, const std::string *key){

    GetOperation *getOperation = new GetOperation(value, *transportFactory, key, &cache_name, flags);
    return getOperation->execute();
    
}

int RemoteCache2::remove(const char *key){
    std::string tmp_key(key);
    return remove(&tmp_key);
}

int RemoteCache2::remove(const std::string *key){
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createRemove(key);
}

int RemoteCache2::removeWithVersion(const char *key, long long version){
    std::string tmp_key(key);
    return removeWithVersion(&tmp_key,version);
}

int RemoteCache2::removeWithVersion(const std::string *key, long long version){
    /**
    * Removes the given entry only if its version matches the supplied version.
    *
    * @param key key to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been removed
    */
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createRemoveWithVersion(key,version);
}

int RemoteCache2::containsKey(const char *key){
    std::string tmp_key(key);
    return containsKey(&tmp_key);
}

int RemoteCache2::containsKey(const std::string *key){
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createContainsKey(key);
}

int RemoteCache2::getWithVersion(const char *value, const char *key,long long *version){
    std::string tmp_key(key);
    std::string val;  //zmenit
    int ret;

    ret = getWithVersion(&val,&tmp_key,version);
    value = (val.c_str());
    return ret;
}

int RemoteCache2::getWithVersion(std::string *value,const std::string *key,long long *version){
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createGetWithVersion(value,key,version);
    
}

int RemoteCache2::getBulk(std::map<std::string,std::string> *bulk,int count){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @param count maximal number of returned entries
    * @return returns Map of string
    */
    // PacketAssembler *PA = new PacketAssembler(transporter);

    // return PA->createGetBulk(bulk,count);    
    
}
int RemoteCache2::getBulk(std::map<std::string,std::string> *bulk){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @return returns Map of string
    */

    return getBulk(bulk,0);
    
}

void RemoteCache2::stats(){
    //Not implemented
}

void RemoteCache2::print_servers(){
    // std::cout << transporter->servers.size()<<std::endl; 
    // for(int i = 0;i<transporter->servers.size();i++){
    //     std::cout << std::dec<<transporter->servers.front().host<<"/"<<transporter->servers.front().port<<"  hash:"<<transporter->servers.front().hash<<std::endl;
    //     transporter->servers.push(transporter->servers.front());
    //     transporter->servers.pop();
    // } 
    // std::cout << transporter->servers.size()<<std::endl; 
    // return;
}

void RemoteCache2::close(){
    // transporter->close_servers();

}


