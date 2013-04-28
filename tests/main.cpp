#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"
#include "main.h"

RemoteCache *cache;

int main(int, char const *[])
{
// remote_cache_config.intelligence = 1;
    
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.cache_name = "";

    
    std::cout<<"TESTING client with Hot Rod v 1.0" <<std::endl;
    remote_cache_config.version = 10;
    cache = new RemoteCache(&remote_cache_config);
    UnitTest::RunAllTests();
    delete cache;
    std::cout<<"TESTING client with Hot Rod v 1.1" <<std::endl;
    remote_cache_config.version = 11;
    cache = new RemoteCache(&remote_cache_config);
    UnitTest::RunAllTests();
    delete cache;
    std::cout<<"TESTING client with Hot Rod v 1.2" <<std::endl;
    remote_cache_config.version = 12;
    cache = new RemoteCache(&remote_cache_config);
    UnitTest::RunAllTests();
    delete cache;

    return 0;
}
