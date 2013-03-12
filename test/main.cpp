#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/TestReporterStdout.h"
#include "main.h"

RemoteCache *cache;

int main(int, char const *[])
{
    RemoteCacheConfig remote_cache_config;
    remote_cache_config.cache_name = "";
    remote_cache_config.version = 12;
    cache = &RemoteCache(&remote_cache_config);
    return UnitTest::RunAllTests();
}
