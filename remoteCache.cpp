#include "remoteCache.h"

RemoteCache::RemoteCache(std::string parhost,std::string parport){

    transporter = new Transporter(parhost,parport);
}

RemoteCache::RemoteCache(std::string parhost){

    transporter = new Transporter(parhost,"11222");
}

RemoteCache::RemoteCache(){
    transporter = new Transporter("127.0.0.1","11222");
}

int RemoteCache::ping(){
    long long tmp;
    return ping(&tmp);
}

int RemoteCache::ping(long long *value){
    PacketAssembler *PA = new PacketAssembler(transporter);
    return PA->createPing(value);
}

int RemoteCache::clear(){
    PacketAssembler *PA = new PacketAssembler(transporter);
    return PA->createClear();
}
int RemoteCache::put(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return put(&tmp_key,&tmp_value,lifespan,idle);
}


int RemoteCache::put(const std::string *key,const std::string *value,int lifespan, int idle){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createPut(key,value,lifespan,idle);
}
int RemoteCache::putAll(std::map<std::string,std::string> *data,int lifespan, int idle){
    //PacketAssembler *PA = new PacketAssembler(transporter);
    std::map<std::string,std::string>::iterator pos;
    for (pos = (*data).begin(); pos != (*data).end(); ++pos) {
        put(&pos->first,&pos->second,lifespan,idle);
        //std::cout << "key: \"" << pos->first << "\" "<< "value: " << pos->second << std::endl;
    }
    return 0;
}

int RemoteCache::putIfAbsent(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return putIfAbsent(&tmp_key,&tmp_value,lifespan,idle);
}


int RemoteCache::putIfAbsent(const std::string *key,std::string *value,int lifespan, int idle){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createPutIfAbsent(key,value,lifespan,idle);
}

int RemoteCache::replace(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return replace(&tmp_key,&tmp_value,lifespan,idle);
}


int RemoteCache::replace(const std::string *key,std::string *value,int lifespan, int idle){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createReplace(key,value,lifespan,idle);
}

int RemoteCache::replaceWithVersion(const char *key,const char *value, long long version,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return replaceWithVersion(&tmp_key,&tmp_value, version,lifespan,idle);
}


int RemoteCache::replaceWithVersion(const std::string *key,std::string *value, long long version,int lifespan, int idle){
    /**
    * Replaces the given value only if its version matches the supplied version.
    *
    * @param key key to use
    * @param value value to use
    * @param version numeric version that should match the one in the server
    *                for the operation to succeed
    * @return 0 if the value has been replaced
    */
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createReplaceWithVersion(key,value,version,lifespan,idle);
}
int RemoteCache::get(const char* value, const char *key){
    std::string tmp_key(key);
    std::string val; //zmenit
    int ret;

    ret = get(&val,&tmp_key);
    value = (val.c_str());
    return ret;
}

int RemoteCache::get(std::string *value, const std::string *key){
    PacketAssembler *PA = new PacketAssembler(transporter);
    //std::cout << *key << *value <<std::flush<<std::endl;
    return PA->createGet(value, key);
    //std::cout << "gva "<< *key << " "<<*value<< " | "<<value <<std::endl;
    //return 1;
}

int RemoteCache::remove(const char *key){
    std::string tmp_key(key);
    return remove(&tmp_key);
}

int RemoteCache::remove(const std::string *key){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createRemove(key);
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
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createRemoveWithVersion(key,version);
}

int RemoteCache::containsKey(const char *key){
    std::string tmp_key(key);
    return containsKey(&tmp_key);
}

int RemoteCache::containsKey(const std::string *key){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createContainsKey(key);
}

int RemoteCache::getWithVersion(const char *value, const char *key,long long *version){
    std::string tmp_key(key);
    std::string val;  //zmenit
    int ret;

    ret = getWithVersion(&val,&tmp_key,version);
    value = (val.c_str());
    return ret;
}

int RemoteCache::getWithVersion(std::string *value,const std::string *key,long long *version){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createGetWithVersion(value,key,version);
    
}

int RemoteCache::getBulk(std::map<std::string,std::string> *bulk,int count){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @param count maximal number of returned entries
    * @return returns Map of string
    */
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createGetBulk(bulk,count);    
    
}
int RemoteCache::getBulk(std::map<std::string,std::string> *bulk){
    /**
    * Bulk get operations, returns all the entries within the remote cache.
    *
    * @return returns Map of string
    */

    return getBulk(bulk,0);
    
}

void RemoteCache::stats(){
    //std::cout << transporter->host<<":"<< transporter->port<<std::endl;

}
