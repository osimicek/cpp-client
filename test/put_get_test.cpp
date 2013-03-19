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


    cache->clear();
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