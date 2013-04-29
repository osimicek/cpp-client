#include "consistentHash.h"
ConsistentHash::ConsistentHash(TransportFactory &tf):transportFactory(tf){}



Transport *ConsistentHash::get_transport(){
    pthread_mutex_lock (&transportFactory.mutex);
    Transport *transport = NULL;
    
    if(transportFactory.transports.size() > 0){
        for(u_int i=0; i<transportFactory.transports.size(); i++){
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
