#include "consistentHash.h"

ConsistentHash11::ConsistentHash11(TransportFactory &tf):ConsistentHash10(tf){}

// Transport *ConsistentHash11::get_transport(){
//     ConsistentHash10::get_transport();
// } 


Transport *ConsistentHash11::get_transport(const std::string *key){
    Transport *transport = NULL;
    if(transportFactory.hash_vector.size() == 0){
        transport = get_transport();
    }else{
        int num_key_owners = transportFactory.get_num_key_owners();
        int hash = transportFactory.get_hash( key->c_str(), key->length());
        hash = hash % transportFactory.get_max_hash_size();
        int random_choice = rand();
        int found = 0;
        for(int i=0; i<num_key_owners; i++){
            pthread_mutex_lock (&transportFactory.mutex);
            if(transportFactory.hash_transport_bank.size() > 0){
                int index = find_index_of_transport(hash);
                int index_to_return = (index + ((random_choice + i) % num_key_owners)) % transportFactory.hash_transport_bank.size();
                transport = transportFactory.hash_transport_bank[index_to_return].second;
                if(DEBUG) std::cout << std::dec<<"11 vybran pro "  <<hash<<"  "<< transport->port << "  "<<index_to_return<< std::endl; 
            }
            if(transport != NULL and transport->used != 1){
                transport->used = 1;
                found = 1;
            }
            pthread_mutex_unlock (&transportFactory.mutex);
            if(found) break;
        }
    }
    
    

    return transport;
} 

