#include "consistentHash.h"

ConsistentHash11::ConsistentHash11(TransportFactory &tf):ConsistentHash10(tf){}

/**
    * Returns Transport object for comunication with server where
    * the key is located
    *
    * @param key key to use
    * @return NULL or Transport object
    */
Transport *ConsistentHash11::get_transport(const std::string *key){
    Transport *transport = NULL;
    if(transportFactory.get_intelligence() != CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE or transportFactory.hash_vector.size() == 0){
        transport = get_transport();
    }else{
        int key_owners_num = transportFactory.get_key_owners_num();
        int hash = transportFactory.get_hash( key->c_str(), key->length());
        hash = hash % transportFactory.get_max_hash_size();
        int random_choice = rand();
        int found = 0;
        for(int i=0; i<key_owners_num; i++){
            LOCK_TF();
            if(transportFactory.hash_transport_bank.size() > 0){
                int index = find_index_of_transport(hash);
                int index_to_return = (index + ((random_choice + i) % key_owners_num)) % transportFactory.hash_transport_bank.size();
                transport = transportFactory.hash_transport_bank[index_to_return].second;
                if(DEBUG) std::cout << std::dec<<"11 vybran pro "  <<hash<<"  "<< transport->port << "  "<<index_to_return<< std::endl; 
            }
            if(transport != NULL and transport->used != 1){
                transport->used = 1;
                found = 1;
            }
            UNLOCK_TF();
            if(found) break;
        }
    }
    
    

    return transport;
} 

