#include "remoteCache.h"

void basic(){
    RemoteCache cache;
    std::string value;
    int status;

    cache.put("key","value");
    status = cache.get("key",&value);
    std::cout << status << " " << value <<std::endl;
}

void bulk(){
    RemoteCache cache;
    std::map<VarItem,VarItem>  bulk;

    cache.clear();
    cache.put(10, 100);
    cache.put(10.1, 100.1);
    cache.getBulk(&bulk);
    for (auto entry = bulk.begin(); entry != bulk.end(); ++entry) {
        std::cout << "key: " << entry->first<< " "<< "value: " << entry->second << std::endl;
    }
}

void config(){
    RemoteCache cache;
    RemoteCacheConfig remote_cache_config;
    remote_cache_config = VERSION_12;
    remote_cache_config = 60000;
    remote_cache_config = 10000;
    remote_cache_config = 0x00;
    remote_cache_config = "AA";
    remote_cache_config = "127.0.0.1";
    remote_cache_config = "11222";
    remote_cache_config = CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE;
}



int main(int, char const *[])
{
    basic();
    bulk();
    config();

    return 0;
}