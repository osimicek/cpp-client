#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
#include "RMMap.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_RMMap_basic_inserts)
{
    // CLEAR();
    RMMap rm_map;
    std::string key = "66";
    std::string value = "value5";

    rm_map["55"]  = "value1";
    rm_map["5.5"] = "value2";
    rm_map[55]    = "value3";
    rm_map[5.5]   = "value4";
    rm_map[key]   = value;
 
    CHECK(rm_map["55"]  == "value1");
    CHECK(rm_map["5.5"] == "value2");
    CHECK(rm_map[55]    == "value3");
    CHECK(rm_map[5.5]   == "value4");
    CHECK(rm_map[key]   == value);

    rm_map["55"]  = 11;
    rm_map["5.5"] = 22;
    rm_map[55]    = 33;
    rm_map[5.5]   = 44;
 
    CHECK(rm_map["55"]  == 11);
    CHECK(rm_map["5.5"] == 22);
    CHECK(rm_map[55]    == 33);
    CHECK(rm_map[5.5]   == 44);

     rm_map["55"]  = 11.11;
    rm_map["5.5"] = 22.22;
    rm_map[55]    = 33.33;
    rm_map[5.5]   = 44.44;
 
    CHECK(rm_map["55"]  == 11.11);
    CHECK(rm_map["5.5"] == 22.22);
    CHECK(rm_map[55]    == 33.33);
    CHECK(rm_map[5.5]   == 44.44);

}



TEST(It_can_RMMap_iterations)
{
    std::map<VarItem,VarItem>  bulk;
    cache->getBulk(&bulk);

    std::map<VarItem,VarItem>::iterator pos;
    std::cout << "Bulk:" <<std::endl;
    for (pos = bulk.begin(); pos != bulk.end(); ++pos) {
        std::cout << "key: \"" << pos->first<< "\" "<< "value: " << pos->second << std::endl;
    }


    RMMap rm_map;
    std::map<VarItem,VarItem>::iterator pos2;
    std::cout << "RMMap:" <<std::endl;
    for (pos2 = rm_map.begin(); pos2 != rm_map.end(); ++pos2) {
        std::cout << "key: \"" << pos2->first  << "\" "<< "value: " << pos2->second << std::endl;
    }

}

}