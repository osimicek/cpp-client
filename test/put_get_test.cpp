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
    status = cache->get(&value, &key);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    value = "value";
    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");

    value = "value2";
    status = cache->put(&key, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value2");
}

TEST(It_can_get_and_put_with_int)
{

    int status;

    std::string value("value");
    CLEAR();
    status = cache->get(&value, 66);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    value = "value";
    status = cache->put(66, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, 66);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");


    int value2    = 77;
    CLEAR();
    status = cache->get(&value2, 66);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value2 == 0);

    value2 = 77;
    status = cache->put(66, value2);
    CHECK(status == NO_ERROR_STATUS);

    value2 = 0;
    status = cache->get(&value2, 66);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value2 == 77);






    
}

TEST(It_can_get_and_put_with_double)
{

    int status;

    std::string value("value");
    CLEAR();
    status = cache->get(&value, 6.6);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == "");

    value = "value";
    status = cache->put(6.6, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, 6.6);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");


    double value2    = 7.7;
    CLEAR();
    status = cache->get(&value2, 6.6);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value2 == 0);

    value2 = 7.7;
    status = cache->put(6.6, value2);
    CHECK(status == NO_ERROR_STATUS);

    value2 = 0;
    status = cache->get(&value2, 6.6);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value2 == 7.7);

}

TEST(It_can_get_and_put_with_varItem)
{

    int status;

    VarItem key, value;
    CLEAR();
    status = cache->get(&value, 6.6);
    std::cout << value <<std::endl;
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
    status = cache->get(&value, 6.6);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == "value");


    value = 99.99;
    key = 6.75;
    CLEAR();
    status = cache->get(&value, &key);
    CHECK(status == KEY_DOES_NOT_EXIST_STATUS);
    CHECK(value == 0);

    value = 7.7;
    status = cache->put(6.75, &value);
    CHECK(status == NO_ERROR_STATUS);

    value.clear();
    status = cache->get(&value, &key);
    CHECK(status == NO_ERROR_STATUS);
    CHECK(value == 7.7);

}






}


// int test1(){
//     RemoteCache x = RemoteCache();
//     x.clear();
//     std::string resp;

//     int ret = 0;
    
//     std::string s1 = std::string("Hell");
//     std::string s2 = std::string("aa");
//     x.putIfAbsent(&s1,&s2);
//     //std::cerr << x.get(std::string("Hell")) <<"|"<<std::endl;
//     x.get(&resp, &s1);

//     if( resp != s2){
//         std::cerr << "Error #1 in test1" <<resp<<std::endl;
//         ret = 1;
//     }
//     resp.clear();

//     std::string s3 = std::string("bb");
//     x.put(&s1,&s3);
//     x.get(&resp, &s1);
//     if(resp != s3){
//         std::cerr << "Error #2 in test1" <<std::endl;
//         ret = 1;
//     }
//      resp.clear();

//     std::string s4 = std::string("cc");
//     x.replace(&s1,&s4);
//     x.get(&resp, &s1);
//     if(resp!= s4){
//         std::cerr << "Error #3 in test1" <<std::endl;
//         ret = 1;
//     }
//     resp.clear();
//     x.clear();
//     x.close();
//     return ret;

// }