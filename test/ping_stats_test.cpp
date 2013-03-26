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

TEST(It_can_stats)
{
    int status;
    std::map<std::string,std::string> stats;
   
    status = cache->stats(&stats);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(stats.size() > 10 );
}



}
