#include "transportFactory.h"
#include "constants.h"

TransportFactory::TransportFactory(std::string host, int port, int version, char intelligence)
{
    hotrod_version = version;
    topology_id = 0;
    virtual_nodes_num = 0;
    max_hash_size = 0;
    message_id = 1;
    hash_ver = 1;
    num_key_owners = 1;
    this->intelligence = intelligence;
    transports_in_use = 0;
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_tf_id, NULL);


    if(hotrod_version == VERSION_10){
        consistentHash = new ConsistentHash10(*this);
    }else if(hotrod_version == VERSION_11){
        consistentHash = new ConsistentHash11(*this);
    }else{
        consistentHash = new ConsistentHash12(*this);
    }

    create_transport(&host, port, 0);
}  
TransportFactory::~TransportFactory(){
    Transport *transport;
    delete consistentHash;
    for(u_int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        delete transport;
        this->transports.pop();       
  } 

}

char TransportFactory::get_intelligence(){
    return intelligence;
}  

void TransportFactory::set_topology_id(int id){
    LOCK_ID();
    topology_id = id;
    UNLOCK_ID()
}
int TransportFactory::get_topology_id(){
    return topology_id;
}  

int TransportFactory::get_and_inc_message_id( ){
    LOCK_ID();
    int ret = message_id;
    message_id++;
    UNLOCK_ID()
    return ret;
}


// void TransportFactory::set_hotrod_version(int version){
//     LOCK_ID();
//     hotrod_version = version;
//     UNLOCK_ID()
// }

int TransportFactory::get_hotrod_version(){
    return hotrod_version;
}  

void TransportFactory::set_virtual_nodes_num(int num){
    LOCK_ID();
    virtual_nodes_num = num;
    UNLOCK_ID()
}
int TransportFactory::get_virtual_nodes_num (){
    return virtual_nodes_num;
}

void TransportFactory::set_max_hash_size(int size){
    LOCK_ID();
    max_hash_size = size;
    UNLOCK_ID()
}
int TransportFactory::get_max_hash_size(){
    return max_hash_size;
}

void TransportFactory::set_key_owners_num(int num){
    LOCK_ID();
    num_key_owners = num;
    UNLOCK_ID()
}
int TransportFactory::get_key_owners_num(){
    return num_key_owners;
}

void TransportFactory::set_hash_ver(int ver){
    LOCK_ID();
    hash_ver = ver;
    UNLOCK_ID()
}
int TransportFactory::get_hash_ver(){
    return hash_ver;
}

int TransportFactory::get_hash(const char* key, int length){
    if(hash_ver == 1){
        return MurmurHash2( key, length, 9001) & INT_MAX;
    }else{
      return MurmurHash3_x64_32( key, length, 9001) & INT_MAX;
    }
}

Transport *TransportFactory::get_transport(){
    Transport *trans = consistentHash->get_transport(); 
    LOCK()
    transports_in_use++;
    UNLOCK();
    return trans;
} 

Transport *TransportFactory::get_transport(const std::string *key){
    Transport *trans = consistentHash->get_transport(key); 
    LOCK()
    transports_in_use++;
    UNLOCK();
    return trans;
} 

Transport * TransportFactory::get_transport(std::string *host, int port, int hash){
  LOCK()
  Transport *transport, *ret_transport;
  ret_transport = NULL;

  // int changed = 0; 
  for(u_int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        this->transports.push(transport);
        this->transports.pop();
        if(transport->port == port and transport->host == *host and transport->hash == hash){
            ret_transport = transport;
        }
       
  } 
  UNLOCK();
  return ret_transport;
}

void TransportFactory::release_transport(Transport * transport){
  LOCK()
  transports_in_use--;
  transport->used = 0;
  UNLOCK();
}

Transport * TransportFactory::create_transport(std::string *host, int port, int hash){
  Transport *transport = new Transport(*host, port, *this);
  transport->hash = hash;
  LOCK()
  this->transports.push(transport);
  UNLOCK();
  return transport;
}

void TransportFactory::invalidate_transports(){
  LOCK()
  Transport *transport;
  for(u_int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        transport->valid = 0;
        
        this->transports.push(transport);
        this->transports.pop();
        //this->servers.pop();
        
  } 
  UNLOCK();
}

void TransportFactory::del_invalid_transports(){
  LOCK()
  Transport *transport;
  for(u_int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        if(transport->valid == 1){ 
          this->transports.push(transport);
        }
        this->transports.pop();
        //this->servers.pop();
        
  } 
  UNLOCK();

}

void TransportFactory::print_hash_bank(){
    for(u_int i=0; i < this->hash_transport_bank.size(); i++){

      std::cout << i << "  "<< this->hash_transport_bank[i].first<<"  "<<this->hash_transport_bank[i].second->port<<std::endl;
  }
}


void TransportFactory::close_servers(){
  LOCK()
  Transport *transport;
  // int changed = 0; 
  for(u_int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        this->transports.push(transport);
        this->transports.pop();
        transport->close_connection();
  } 
  UNLOCK();
}



