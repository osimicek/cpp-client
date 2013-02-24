#include "consistentHash11.h"

ConsistentHash11::ConsistentHash11(TransportFactory &tf):ConsistentHash10(tf){}

// Transport *ConsistentHash11::get_transport(){
//     ConsistentHash10::get_transport();
// } 


Transport *ConsistentHash11::get_transport(const std::string *key){
    Transport *transport = NULL;
    if(transportFactory.hash_vector.size() == 0){
        transport = get_transport();
    }else{
        pthread_mutex_lock (&transportFactory.mutex);
        if(transportFactory.hash_transport_bank.size() > 0){
            int hash = MurmurHash3_x64_32(  key->c_str(), key->length(), 9001) & INT_MAX;
            hash = hash % transportFactory.get_max_hash_size();
            int index = find_index_of_transport(hash);
            int index_to_return = (index + (rand() % transportFactory.get_num_key_owners())) % transportFactory.hash_transport_bank.size();
            transport = transportFactory.hash_transport_bank[index_to_return].second;
            if(DEBUG) std::cout << std::dec<<"vybran pro "  <<hash<<"  "<< transport->port << "  "<<index_to_return<< std::endl; 
        }
        if(transport != NULL)   transport->used = 1;
        pthread_mutex_unlock (&transportFactory.mutex);
    }
    
    

    return transport;
} 

