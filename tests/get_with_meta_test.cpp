#include "UnitTest++/UnitTest++.h"
#include "UnitTest++/AssertException.h"
#include "UnitTest++/ReportAssert.h"
#include "main.h"
extern RemoteCache *cache;

using namespace UnitTest;

namespace {

TEST(It_can_get_meta_basic)
{
    if(cache->getVersion() >= VERSION_12){
        int status;
        VarItem value;


        CLEAR();


        RemoteCacheConfig remote_cache_config2;
        remote_cache_config2.cache_name = "";
        remote_cache_config2.version = 12;
        remote_cache_config2.intelligence = CLIENT_INTELLIGENCE_BASIC;
        RemoteCache cache2(&remote_cache_config2);

        
        status = cache2.put(777, "value");
        CHECK(status == NO_ERROR_STATUS);

        Metadata meta;
        status = cache2.getWithMetadata(777, &value, &meta);
        CHECK(status == NO_ERROR_STATUS);
        std::cout <<status<<" "<<(int)meta.flag<<" "<<meta.lifespan<<" "<<meta.maxidle<<" "<<meta.created<<" "<<meta.lastused<<" "<<meta.version<<std::endl;
        CHECK(meta.flag == 0x03);
        CHECK(meta.lifespan == -1);
        CHECK(meta.maxidle == -1);
        CHECK(meta.created == -1);
        CHECK(meta.lastused == -1);
        CHECK(meta.version > 0);
        

        status = cache2.put(777, "value", 999, 999);
        CHECK(status == NO_ERROR_STATUS);

        meta.clear();
        status = cache2.getWithMetadata(777, &value, &meta);
        CHECK(status == NO_ERROR_STATUS);
        // std::cout <<status<<" "<<(int)meta.flag<<" "<<meta.lifespan<<" "<<meta.maxidle<<" "<<meta.created<<" "<<meta.lastused<<" "<<meta.version<<std::endl;
        CHECK(meta.flag == 0x00);
        CHECK(meta.lifespan > 0);

        CHECK(meta.maxidle == 999);
        CHECK(meta.created > 0);
        CHECK(meta.lastused > 0);
        CHECK(meta.version > 0);
    }else{
        int status;
        VarItem value;
        Metadata meta;
        status = cache->getWithMetadata(777, &value, &meta);
        CHECK(status == ERROR_NOT_IMPLEMENTED);
    }
    
}


TEST(It_can_get_meta_adv)
{
    if(cache->getVersion() >= VERSION_12){
        int status;
        VarItem value;


        CLEAR();


        RemoteCacheConfig remote_cache_config2;
        remote_cache_config2.cache_name = "";
        remote_cache_config2.lifespan = 33;
        remote_cache_config2.maxidle = 22;
        remote_cache_config2.version = 12;
        remote_cache_config2.intelligence = CLIENT_INTELLIGENCE_BASIC;
        RemoteCache cache2(&remote_cache_config2);

        Metadata meta;
        status = cache2.put(777, "value");
        CHECK(status == NO_ERROR_STATUS);

        status = cache2.getWithMetadata(777, &value, &meta);
        CHECK(status == NO_ERROR_STATUS);
        CHECK(meta.flag == 0x00);
        CHECK(meta.lifespan == 33);
        CHECK(meta.maxidle == 22);
        CHECK(meta.created > 0);
        CHECK(meta.lastused > 0);
        CHECK(meta.version > 0);

    }


}


}

