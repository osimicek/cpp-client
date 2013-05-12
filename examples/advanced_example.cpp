#include "remoteCache.h"

void basic(){
    RemoteCache cache;
    std::string value;
    int status;

    cache.put("key","value");
    status = cache.get("key",&value);
    std::cout << "status: "<<status << " value: " << value <<std::endl;
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

//shows how to config cache
void config(){
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.version = VERSION_12;
    remote_cache_config.lifespan = 60000;
    remote_cache_config.maxidle = 10000;
    remote_cache_config.flags = 0x00;
    remote_cache_config.cache_name = "AA";
    remote_cache_config.host = "127.0.0.1";
    remote_cache_config.port = 11222;
    remote_cache_config.intelligence = CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE;

    RemoteCache cache(&remote_cache_config);
    cache.put("key", "value");

    std::cout <<"Number of key owners: " << cache.getKeyOwnersNum() << std::endl;

    std::cout <<"Hot Rod version: " << cache.getVersion() << std::endl;
}

void metadata(){
    RemoteCache cache;
    RemoteEntryMetadata metadata;
    VarItem value;
    

    cache.put("key", "value", 60000, 10000);
    cache.getWithMetadata("key", &value, &metadata);
    std::cout<<
    "flag " << metadata.flag <<
    "\nlifespan " << metadata.lifespan <<
    "\nmaxidle " << metadata.maxidle <<
    "\ncreated " << metadata.created <<
    "\nlastused " << metadata.lastused <<
    "\nversion " << metadata.version<<
    std::endl;
}
void work_with_int(){
    RemoteCache cache;
    int value;

    cache.put(55, 66);
    cache.get(55, &value);
    std::cout <<"value: " << value << std::endl;
}

void work_with_double(){
    RemoteCache cache;
    double value;

    cache.put(5.5, 6.6);
    cache.get(5.5, &value);
    std::cout <<"value: " << value << std::endl;
}

void work_with_string(){
    RemoteCache cache;
    std::string value;

    cache.put("key", "string value");
    cache.get("key", &value);
    std::cout <<"value: " << value << std::endl;
}

void work_with_varitem(){
    RemoteCache cache;
    VarItem key, value;
    char * str;

    key = 55;
    std::cout <<"key: " << key << std::endl;
    key = 99.99;
    std::cout <<"key: " << key << std::endl;
    key = "Longer string array Longer string array Longer string array ";
    std::cout <<"key: " << key << std::endl;
    cache.put(key, "string value");
    cache.get(key, &value);
    std::cout <<"value: " << value << std::endl;

    str = value;
    std::cout <<"string: " << str << std::endl;
}



int main(int, char const *[])
{
    std::cout <<"\nBasic example /////////////////////"<< std::endl;
    basic();
    std::cout <<"\nBulk example /////////////////////"<< std::endl;
    bulk();
    std::cout <<"\nConfig example /////////////////////"<< std::endl;
    config();
    std::cout <<"\nMetadata example /////////////////////"<< std::endl;
    metadata();
    std::cout <<"\nwork_with_int example /////////////////////"<< std::endl;
    work_with_int();
    std::cout <<"\nwork_with_double example /////////////////////"<< std::endl;
    work_with_double();
    std::cout <<"\nwork_with_string    example /////////////////////"<< std::endl;
    work_with_string();
    std::cout <<"\nWork with VarItem example /////////////////////"<< std::endl;
    work_with_varitem();

    return 0;
}