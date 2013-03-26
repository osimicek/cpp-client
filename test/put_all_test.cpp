#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_put_all)
{
    std::map<VarItem,VarItem> data;
    VarItem value;
    int status;

    CLEAR();
    data["key1"] = "value";
    data["key2"] = 555;
    data["key3"] = 55.5;
    data[11] = "value";
    data[22] = 555;
    data[33] = 55.5;
    data[1.1] = "value";
    data[2.2] = 555;
    data[3.3] = 55.5;

    cache->putAll(&data);

    status = cache->get(&value, "key1");
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(&value, "key2");
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(&value, "key3");
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

    status = cache->get(&value, 11);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(&value, 22);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(&value, 33);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

    status = cache->get(&value, 1.1);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(&value, 2.2);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(&value, 3.3);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

   
}


}
