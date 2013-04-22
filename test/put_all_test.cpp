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

    status = cache->get("key1", &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get("key2", &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get("key3", &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

    status = cache->get(11, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(22, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(33, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

    status = cache->get(1.1, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(2.2, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(3.3, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

   
}

TEST(It_can_put_all_async)
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

    cache->putAllAsync(&data);

    status = cache->get("key1", &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get("key2", &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get("key3", &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

    status = cache->get(11, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(22, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(33, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

    status = cache->get(1.1, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    status = cache->get(2.2, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 555);

    status = cache->get(3.3, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 55.5);

   
}


}
