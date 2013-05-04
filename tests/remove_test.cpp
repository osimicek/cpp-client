#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_remove_basic)
{
    std::string key("key");
    std::string value;
    int status;
    CLEAR();

    value = "value";
    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(value == "value");
    CHECK(status == NO_ERROR_STATUS);

    status = cache->remove(&key);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(value == "");
    CHECK(status = KEY_DOES_NOT_EXIST_STATUS);

}

TEST(It_remove_int)
{
    VarItem value;
    int status;
    CLEAR();

    status = cache->put(123, 336);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->get(123, &value);
    CHECK(value == 336);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->remove(123);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(123, &value);
    CHECK(value == "");
    CHECK(status = KEY_DOES_NOT_EXIST_STATUS);

}

TEST(It_remove_double)
{
    VarItem value;
    int status; 
    CLEAR();

    status = cache->put(12.3, 33.6);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->get(12.3, &value);
    CHECK(value == 33.6);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->remove(12.3);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(12.3, &value);
    CHECK(value == "");
    CHECK(status = KEY_DOES_NOT_EXIST_STATUS);

}

TEST(It_can_remove_with_version)
{
    VarItem value;
    int status;
    long long version;

    CLEAR();

    status = cache->put(12.3, 45.6);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 45.6);


    status = cache->removeWithVersion(12.3, version +1);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 45.6);

     status = cache->removeWithVersion(12.3, version);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == 0);


}

TEST(It_can_remove_with_obtain_prev_value)
{
    VarItem prev_value;
    int status;

    CLEAR();

    status = cache->remove(55, &prev_value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(prev_value == "");


    status = cache->put(55, "value");
    CHECK(status == NO_ERROR_STATUS);

    status = cache->remove(55, &prev_value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(prev_value == "value");

}

TEST(It_can_remove_with_version_with_obtain_prev_value)
{
    VarItem value, prev_value;
    int status;
    long long version;

    CLEAR();

    status = cache->put(12.3, 45.6);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 45.6);


    status = cache->removeWithVersion(12.3, version +1, &prev_value);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);
    CHECK(prev_value == 45.6);

    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 45.6);

     status = cache->removeWithVersion(12.3, version, &prev_value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(prev_value == 45.6);

    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == 0);


}


}
