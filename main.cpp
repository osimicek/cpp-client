#include <string>
#include <remoteCache.h>
#include <RMMap.h>





int main(){

    int status;
    std::string resp;
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.cache_name = "";
    remote_cache_config.version = 11;
    remote_cache_config.intelligence = 3;
    RemoteCache x = RemoteCache(&remote_cache_config);

    x.clear();

    VarItem ret;
    long long ver;

    x.put(55,66);
    status = x.getWithVersion(55,&ret,&ver);
    std::cout<<std::hex <<status <<std::endl;

    status = x.removeWithVersion(55,ver);
    std::cout <<std::hex<<status <<std::endl;

	return 0;
}



    