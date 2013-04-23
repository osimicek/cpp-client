#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_replace_basic)
{
    std::string key("key");
    std::string value;
    int status;

    CLEAR();
    value = "value";
    status = cache->replace(&key, &value);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->replace(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(value == "value2");
    CHECK(status == NO_ERROR_STATUS);
}

TEST(It_can_replace_int)
{
    VarItem value;
    int status;

    CLEAR();
    value = "value";
    status = cache->replace(123, 456);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(123, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    status = cache->put(123, 456);
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->replace(123, 789);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(123, &value);
    CHECK(value == 789);
    CHECK(status == NO_ERROR_STATUS);
}

TEST(It_can_replace_double)
{
    VarItem value;
    int status;

    CLEAR();
    value = "value";
    status = cache->replace(12.3, 45.6);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(12.3, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    status = cache->put(12.3, 45.6);
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->replace(12.3, 78.9);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(12.3, &value);
    CHECK(value == 78.9);
    CHECK(status == NO_ERROR_STATUS);
}

TEST(It_can_replace_with_version)
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


    status = cache->replaceWithVersion(12.3, 999, version +1);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 45.6);

     status = cache->replaceWithVersion(12.3, 999, version);
    CHECK(status == NO_ERROR_STATUS);

    status = cache->getWithVersion(12.3, &value, &version);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 999);


}

}
