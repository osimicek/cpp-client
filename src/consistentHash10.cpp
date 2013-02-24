#define DONT_INCLUDE_CONSISTENTHASH11  //circular dependency
#include "consistentHash10.h"
#undef DONT_INCLUDE_CONSISTENTHASH11  //circular dependency

ConsistentHash10::ConsistentHash10(TransportFactory &tf):transportFactory(tf){}


Transport *ConsistentHash10::get_transport(){
    pthread_mutex_lock (&transportFactory.mutex);
    Transport *transport = NULL;
    if(transportFactory.transports.size() > 0){
        transport = transportFactory.transports.front();
        transportFactory.transports.push(transport);
        transportFactory.transports.pop();
    }
 
    if(transport != NULL)   transport->used = 1;
    pthread_mutex_unlock (&transportFactory.mutex);
    

    return transport;
} 

Transport *ConsistentHash10::get_transport(const std::string *key){
    
    Transport *transport = NULL;
    if(transportFactory.hash_vector.size() == 0){
        transport = get_transport();
    }else{
        pthread_mutex_lock (&transportFactory.mutex);
        if(transportFactory.hash_transport_bank.size() > 0){
            int hash = MurmurHash2(  key->c_str(), key->length(), 9001) & INT_MAX;
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