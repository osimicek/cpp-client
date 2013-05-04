#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_basic_get_and_put)
{
    std::string key("key");
    std::string value;
    int status;


    CLEAR();
    status = cache->get(&key, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    value = "value";
    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    value = "value2";
    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value2");
}

TEST(It_can_get_and_put_with_int)
{

    int status;

    std::string value("value");
    CLEAR();
    status = cache->get(66, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    value = "value";
    status = cache->put(66, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(66, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");


    int value2    = 77;
    CLEAR();
    status = cache->get(66, &value2);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value2 == 0);

    value2 = 77;
    status = cache->put(66, value2);
    CHECK(status == NO_ERROR_STATUS);

    value2 = 0;
    status = cache->get(66, &value2);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value2 == 77);






    
}

TEST(It_can_get_and_put_with_double)
{

    int status;

    std::string value("value");
    CLEAR();
    status = cache->get(6.6, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    value = "value";
    status = cache->put(6.6, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(6.6, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");


    double value2    = 7.7;
    CLEAR();
    status = cache->get(6.6, &value2);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value2 == 0);

    value2 = 7.7;
    status = cache->put(6.6, value2);
    CHECK(status == NO_ERROR_STATUS);

    value2 = 0;
    status = cache->get(6.6, &value2);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value2 == 7.7);

}

TEST(It_can_get_and_put_with_varItem)
{

    int status;

    VarItem key, value;
    CLEAR();
    status = cache->get(6.6, &value);

    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");
    CHECK(value == std::string(""));
    CHECK(value == 0);
    CHECK(value == 0.0);

    key = 6.6;
    value = "value";
    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value = 99;
    status = cache->get(6.6, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");


    value = 99.99;
    key = 6.75;
    CLEAR();
    status = cache->get(&key, &value);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == 0);

    value = 7.7;
    status = cache->put(6.75, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&key, &value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 7.7);

}

TEST(It_can_put_with_obtain_prev_value)
{
    int status;

    CLEAR();
    VarItem prev_value, curr_value;
    status = cache->put("key", "value1");
    CHECK(status == NO_ERROR_STATUS);
    status = cache->get("key", &curr_value);
    CHECK(curr_value == "value1");

    status = cache->put("key", 5, &prev_value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(prev_value == "value1");
    status = cache->get("key", &curr_value);
    CHECK(curr_value == 5);

    status = cache->put("key", 5.555, &prev_value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(prev_value == 5);
    status = cache->get("key", &curr_value);
    CHECK(curr_value == 5.555);

    status = cache->put("key", "value1", &prev_value);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(prev_value == 5.555);
    status = cache->get("key", &curr_value);
    CHECK(curr_value == "value1");




}






}


