#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_put_if_absent)
{
    std::string key("key");
    std::string value;
    int status;


    status = cache->clear();
    CHECK(status == NO_ERROR_STATUS);
    value = "value";
    status = cache->putIfAbsent(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(value == "value");
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->putIfAbsent(&key, &value);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(value == "value");
    CHECK(status == NO_ERROR_STATUS);

}

}
