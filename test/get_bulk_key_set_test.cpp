#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_get_bulk)
{
    std::map<VarItem,VarItem> data;
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


    std::map<VarItem,VarItem>  bulk;
    cache->getBulk(&bulk);
    CHECK(bulk.size() <= data.size());

    std::map<VarItem,VarItem>::iterator pos;
    int count = 0;
    for (pos = bulk.begin(); pos != bulk.end(); ++pos) {
        count++;
        // std::cout << "key: \"" << pos->first<< "\" "<< "value: " << pos->second << std::endl;
    }
    CHECK(count == bulk.size());

    bulk.clear();
    cache->getBulk(&bulk,1);
    CHECK(bulk.size() == 1);
}

TEST(It_can_get_key_set)
{
    std::map<VarItem,VarItem> data;
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


    std::vector<VarItem>  k_set;
    cache->keySet(&k_set);
    CHECK(k_set.size() == data.size());

    k_set.clear();
    cache->keySet(&k_set,2);
    CHECK(k_set.size() <= data.size());


}






}

