#include <string>
#include <remoteCache.h>
// #include <marshaller.h>
#include <RMMap.h>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include "varItem.h"
#include <murmur/MurmurHash3.h>
#include <murmur/MurmurHash2.h>




int main(){

    int status;
    std::string resp;
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.cache_name = "";
    remote_cache_config.version = 11;
    remote_cache_config.intelligence = 2;
    RemoteCache x = RemoteCache(&remote_cache_config);

    x.clear();
    std::cout<< "OK\n"<<std::flush;
    VarItem ret, prev_value;
    long long ver;
    std::cout<< "OK\n"<<std::flush;
    x.put(55,66);
    status = x.getWithVersion(55,&ret,&ver);
    std::cout <<status <<std::endl;

    status = x.removeWithVersion(55,ver);
    std::cout <<std::hex<<status <<std::endl;
    std::cout<< "OK\n"<<std::flush;

	return 0;
}



    