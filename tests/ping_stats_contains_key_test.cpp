#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_ping)
{
    int status; 
    status = cache->ping();
    CHECK(status == NO_ERROR_STATUS);
}

TEST(It_can_read_stats)
{
    int status;
    std::map<std::string,std::string> stats;
   
    status = cache->stats(&stats);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(stats.size() > 10 );
}

TEST(It_can_check_key)
{
    int status;
   
    status = cache->containsKey(15);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    status = cache->put(15,65);
    CHECK(status == NO_ERROR_STATUS);
    status = cache->containsKey(15);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->containsKey(1.5);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    status = cache->put(1.5,6.5);
    CHECK(status == NO_ERROR_STATUS);
    status = cache->containsKey(1.5);
    CHECK(status == NO_ERROR_STATUS);

    std::string key = "key-xx";

    status = cache->containsKey(&key);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    status = cache->put(&key,6.5);
    CHECK(status == NO_ERROR_STATUS);
    status = cache->containsKey(&key);
    CHECK(status == NO_ERROR_STATUS);
}



}
