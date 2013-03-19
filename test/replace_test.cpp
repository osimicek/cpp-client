#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_replace)
{
    std::string key("key");
    std::string value;
    int status;


    cache->clear();
    value = "value";
    status = cache->replace(&key, &value);
    CHECK(status == NOT_PUT_REMOVED_REPLACED_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value = "value2";
    status = cache->replace(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(value == "value2");
    CHECK(status == NO_ERROR_STATUS);
}

}
