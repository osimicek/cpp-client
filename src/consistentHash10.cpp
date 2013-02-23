#define DONT_INCLUDE_CONSISTENTHASH11  //circular dependency
#include "consistentHash10.h"
#undef DONT_INCLUDE_CONSISTENTHASH11  //circular dependency

ConsistentHash10::ConsistentHash10(TransportFactory &tf):transportFactory(tf){}

Transport *ConsistentHash10::get_transport(const std::string *key){
    pthread_mutex_lock (&transportFactory.mutex);
    Transport *transport = NULL;
    if(transportFactory.hash_vector.size() == 0){
        transport = transportFactory.transports.front();
        transportFactory.transports.push(transport);
        transportFactory.transports.pop();
    }else{
        int hash = MurmurHash3_x64_32(  key->c_str(), key->length(), 9001) & INT_MAX;
        hash = hash % transportFactory.get_max_hash_size();
        int index = find_index_of_transport(hash);
        int index_to_return = (index + (rand() % transportFactory.get_num_key_owners())) % transportFactory.hash_transport_bank.size();
        transport = transportFactory.hash_transport_bank[index_to_return].second;
        std::cout << std::dec<<"vybran pro "  <<hash<<"  "<< transport->port << "  "<<index_to_return<< std::endl;

    }
    pthread_mutex_unlock (&transportFactory.mutex);
    

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
        std::cout <<"counter "<<++counter<< std::endl;
      /* calculate the midpoint for roughly equal partition */
        int imid = (imin + imax) / 2;
        distance = transportFactory.hash_vector[imid] - key_hash;
        if(distance >= 0 && distance < best_distance){
            result_i = imid;
            best_distance = distance;
        }
        if (transportFactory.hash_vector[imid] < key_hash)
          // change min index to search upper subarray
          imin = imid + 1;
        else if (transportFactory.hash_vector[imid] > key_hash)
          // change max index to search lower subarray
          imax = imid - 1;
        else
          // key found at index imid
          return imid;
    }
    if(imax >= imin) result_i = 0;
    return result_i;
}