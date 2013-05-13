#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"
#include "main.h"

RemoteCache *cache;

int main(int, char const *[])
{
// remote_cache_config.intelligence = 1;
    
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.cache_name = "";

    for(int i=1; i<=3; i++){
        std::cout<<"TESTING client with Hot Rod v 1.0 inteligence 0x0"<< i <<std::endl;
        remote_cache_config.version = 10;
        remote_cache_config.intelligence = i;
        cache = new RemoteCache(&remote_cache_config);
        UnitTest::RunAllTests();
        delete cache;
        std::cout<<"TESTING client with Hot Rod v 1.1 inteligence 0x0"<< i <<std::endl;
        remote_cache_config.version = 11;
        remote_cache_config.intelligence = i;
        cache = new RemoteCache(&remote_cache_config);
        UnitTest::RunAllTests();
        delete cache;
        std::cout<<"TESTING client with Hot Rod v 1.2 inteligence 0x0"<< i <<std::endl;
        remote_cache_config.version = 12;
        remote_cache_config.intelligence = i;
        cache = new RemoteCache(&remote_cache_config);
        UnitTest::RunAllTests();
        delete cache;
    }

    return 0;
}
