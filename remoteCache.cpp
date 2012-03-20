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
int RemoteCache::put(const char *key,const char *value){
    return put(std::string(key),std::string(value));
}


int RemoteCache::put(std::string key,std::string value){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createPut(key,value);
}

int RemoteCache::putIfAbsent(const char *key,const char *value){
    return putIfAbsent(std::string(key),std::string(value));
}


int RemoteCache::putIfAbsent(std::string key,std::string value){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createPut(key,value);
}

int RemoteCache::replace(const char *key,const char *value){
    return replace(std::string(key),std::string(value));
}


int RemoteCache::replace(std::string key,std::string value){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createPut(key,value);
}

int RemoteCache::replaceWithVersion(const char *key,const char *value, long long version){
    return replaceWithVersion(std::string(key),std::string(value), version);
}


int RemoteCache::replaceWithVersion(std::string key,std::string value, long long version){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createReplaceWithVersion(key,value,version);
}
const char* RemoteCache::get(const char *key){
    return get(std::string(key)).c_str();
}

std::string RemoteCache::get(std::string key){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createGet(key);
}

int RemoteCache::remove(const char *key){
    return remove(std::string(key));
}

int RemoteCache::remove(std::string key){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createRemove(key);
}

int RemoteCache::removeWithVersion(const char *key, long long version){
    return removeWithVersion(std::string(key),version);
}

int RemoteCache::removeWithVersion(std::string key, long long version){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createRemoveWithVersion(key,version);
}

int RemoteCache::containsKey(const char *key){
    return containsKey(std::string(key));
}

int RemoteCache::containsKey(std::string key){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createContainsKey(key);
}

const char* RemoteCache::getWithVersion(const char *key,long long *version){
    return getWithVersion(std::string(key),version).c_str();
}

std::string RemoteCache::getWithVersion(std::string key,long long *version){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createGetWithVersion(key,version);
}

std::map<std::string,std::string>  RemoteCache::getBulk(){
    return getBulk(0);
}
std::map<std::string,std::string> RemoteCache::getBulk(int count){
    PacketAssembler *PA = new PacketAssembler(transporter);

    return PA->createGetBulk(count);    
}

void RemoteCache::print_info(){
    //std::cout << transporter->host<<":"<< transporter->port<<std::endl;

}
