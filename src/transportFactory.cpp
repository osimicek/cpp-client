#include "transportFactory.h"
#include "constants.h"

TransportFactory::TransportFactory(std::string host, int port, int version)
{
    hotrod_version = version;
    topology_id = 0;
    virtual_nodes_num = 0;
    max_hash_size = 0;
    message_id = 1;
    hash_ver = 1;
    num_key_owners = 1;
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_t_id, NULL);


    if(hotrod_version == VERSION_10){
        consistentHash = new ConsistentHash10(*this);
    }else if(hotrod_version == VERSION_11){
        consistentHash = new ConsistentHash11(*this);
    }else{
        consistentHash = new ConsistentHash12(*this);
    }

    first_transport = create_transport(&host, port, 0);
}  


void TransportFactory::set_topology_id(int id){
    pthread_mutex_lock (&mutex_t_id);
    topology_id = id;
    pthread_mutex_unlock (&mutex_t_id);
}
int TransportFactory::get_topology_id(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = topology_id;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}  

int TransportFactory::get_and_inc_message_id( ){
    pthread_mutex_lock (&mutex_t_id);
    int ret = message_id;
    message_id++;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}


// void TransportFactory::set_hotrod_version(int version){
//     pthread_mutex_lock (&mutex_t_id);
//     hotrod_version = version;
//     pthread_mutex_unlock (&mutex_t_id);
// }

int TransportFactory::get_hotrod_version(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = hotrod_version;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}  

void TransportFactory::set_virtual_nodes_num(int num){
    pthread_mutex_lock (&mutex_t_id);
      virtual_nodes_num = num;
    pthread_mutex_unlock (&mutex_t_id);
}
int TransportFactory::get_virtual_nodes_num (){
    pthread_mutex_lock (&mutex_t_id);
    int ret = virtual_nodes_num;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}

void TransportFactory::set_max_hash_size(int size){
    pthread_mutex_lock (&mutex_t_id);
      max_hash_size = size;
    pthread_mutex_unlock (&mutex_t_id);
}
int TransportFactory::get_max_hash_size(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = max_hash_size;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}

void TransportFactory::set_num_key_owners(int num){
    pthread_mutex_lock (&mutex_t_id);
    num_key_owners = num;
    pthread_mutex_unlock (&mutex_t_id);
}
int TransportFactory::get_num_key_owners(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = num_key_owners;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}

void TransportFactory::set_hash_ver(int ver){
    pthread_mutex_lock (&mutex_t_id);
    hash_ver = ver;
    pthread_mutex_unlock (&mutex_t_id);
}
int TransportFactory::get_hash_ver(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = hash_ver;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}

int TransportFactory::get_hash(const char* key, int length){
    if(hash_ver == 1){
        return MurmurHash2( key, length, 9001) & INT_MAX;
    }else{
      return MurmurHash3_x64_32( key, length, 9001) & INT_MAX;
    }
}

Transport *TransportFactory::get_transport(){
    if(hotrod_version == VERSION_10){
        return ((ConsistentHash10 *) consistentHash)->get_transport(); 
    }else if(hotrod_version == VERSION_11){
        return ((ConsistentHash11 *) consistentHash)->get_transport();
    }else{
        return ((ConsistentHash12 *) consistentHash)->get_transport();
    }
} 

Transport *TransportFactory::get_transport(const std::string *key){
    if(hotrod_version == VERSION_10){
        return ((ConsistentHash10 *) consistentHash)->get_transport(key);
    }else if(hotrod_version == VERSION_11){
        return ((ConsistentHash11 *) consistentHash)->get_transport(key);
    }else{
        return ((ConsistentHash12 *) consistentHash)->get_transport(key);
    }
} 

Transport * TransportFactory::get_transport(std::string *host, int port, int hash){
  pthread_mutex_lock (&mutex);
  Transport *transport, *ret_transport;
  ret_transport = NULL;

  int changed = 0;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->transports.size();i++){
        //std::cout << std::dec<<this->servers.front().host<<"/"<<this->servers.front().port<<std::flush<<std::endl;
        transport = this->transports.front();
        this->transports.push(transport);
        this->transports.pop();
        if(transport->port == port and transport->host == *host and transport->hash == hash){
            ret_transport = transport;
        }
       
  } 
  pthread_mutex_unlock (&mutex);
  return ret_transport;
}

void TransportFactory::release_transport(Transport * transport){
  pthread_mutex_lock (&mutex);
  transport->used = 0;
  pthread_mutex_unlock (&mutex);
}

Transport * TransportFactory::create_transport(std::string *host, int port, int hash){
  Transport *transport = new Transport(*host, port, *this);
  transport->hash = hash;
  pthread_mutex_lock (&mutex);
  this->transports.push(transport);
  pthread_mutex_unlock (&mutex);
  return transport;
}

void TransportFactory::invalidate_transports(){
  pthread_mutex_lock (&mutex);
  Transport *transport;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        transport->valid = 0;
        
        this->transports.push(transport);
        this->transports.pop();
        //this->servers.pop();
        
  } 
  pthread_mutex_unlock (&mutex);
}

void TransportFactory::del_invalid_transports(){
  pthread_mutex_lock (&mutex);
  Transport *transport;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->transports.size();i++){
        transport = this->transports.front();
        if(transport->valid == 1){ 
          this->transports.push(transport);
        }
        this->transports.pop();
        //this->servers.pop();
        
  } 
  pthread_mutex_unlock (&mutex);

}

void TransportFactory::print_hash_bank(){
    for(int i=0; i < this->hash_transport_bank.size(); i++){

      std::cout << i << "  "<< this->hash_transport_bank[i].first<<"  "<<this->hash_transport_bank[i].second->port<<std::endl;



  }
}




