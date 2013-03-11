#include "consistentHash.h"
ConsistentHash::ConsistentHash(TransportFactory &tf):transportFactory(tf){}



Transport *ConsistentHash::get_transport(){
    pthread_mutex_lock (&transportFactory.mutex);
    Transport *transport = NULL;
    
    if(transportFactory.transports.size() > 0){
        for(int i=0; i<transportFactory.transports.size(); i++){
            transport = transportFactory.transports.front();
            transportFactory.transports.push(transport);
            transportFactory.transports.pop();
            if(transport->used != 1) break;
            else transport = NULL;
        }
    }
 
    if(transport != NULL)   transport->used = 1;
    pthread_mutex_unlock (&transportFactory.mutex);
    

    return transport;
} 

Transport *ConsistentHash::get_transport(const std::string *key){
    return NULL;
}




ConsistentHash10::ConsistentHash10(TransportFactory &tf):ConsistentHash(tf){}

Transport *ConsistentHash10::get_transport(const std::string *key){
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
                if(DEBUG) std::cout << std::dec<<"vybran pro "  <<hash<<"  "<< transport->port << "  "<<index_to_return<< std::endl;
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

int ConsistentHash10::find_index_of_transport(int key_hash){
    int imid;
    int imin = 0;
    int imax = transportFactory.hash_vector.size() - 1;
    int result_i = 0;
    int best_distance = INT_MAX;
    int distance = 0;
    int counter = 0;
    while (imax >= imin)
    {
        int imid = (imin + imax) / 2;
        distance = transportFactory.hash_vector[imid] - key_hash;
        if(distance >= 0 && distance < best_distance){
            result_i = imid;
            best_distance = distance;
        }
        if(transportFactory.hash_vector[imid] < key_hash){
            imin = imid + 1;
        }
        else if(transportFactory.hash_vector[imid] > key_hash){
            imax = imid - 1;
        }
        else{
          return imid;
        }
    }
    if(imax >= imin) result_i = 0;
    return result_i;
}