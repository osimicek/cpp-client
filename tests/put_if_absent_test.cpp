#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_put_if_absent_basic)
{
    std::string key("key");
    std::string value;
    int status;


    CLEAR();
    CHECK(status == NO_ERROR_STATUS);
    value = "value";
    status = cache->putIfAbsent(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(value == "value");
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->putIfAbsent(&key, &value);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(value == "value");
    CHECK(status == NO_ERROR_STATUS);

}

TEST(It_can_put_if_absent_int)
{
    VarItem value;
    int status;


    CLEAR();
    CHECK(status == NO_ERROR_STATUS);
    value = "value";
    status = cache->putIfAbsent(15, 99);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(15, &value);
    CHECK(value == 99);
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->putIfAbsent(15, 66);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(15, &value);
    CHECK(value == 99);
    CHECK(status == NO_ERROR_STATUS);

}

TEST(It_can_put_if_absent_double)
{
    VarItem value;
    int status;


    CLEAR();
    CHECK(status == NO_ERROR_STATUS);
    value = "value";
    status = cache->putIfAbsent(1.5, 9.9);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(1.5, &value);
    CHECK(value == 9.9);
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->putIfAbsent(1.5, 6.6);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(1.5, &value);
    CHECK(value == 9.9);
    CHECK(status == NO_ERROR_STATUS);

}

}
