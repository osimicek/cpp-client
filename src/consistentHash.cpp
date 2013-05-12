#include "consistentHash.h"
ConsistentHash::ConsistentHash(TransportFactory &tf):transportFactory(tf){}

/**
    * Returns Transport object for comunication with server.
    *
    * @return NULL or Transport object
*/
Transport *ConsistentHash::get_transport(){
    return NULL;
} 

/**
    * Returns Transport object for comunication with server where
    * the key is located
    *
    * @param key key to use
    * @return NULL or Transport object
*/
Transport *ConsistentHash::get_transport(const std::string *key){
    return NULL;
}
