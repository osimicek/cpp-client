#define DONT_INCLUDE_CODEC12  //circular dependency
#include "codec11.h"
#undef DONT_INCLUDE_CODEC12  //circular dependency


Codec11::Codec11(Transport &t):Codec10(t){

}

void Codec11::write_header(short op_code, const std::string *cache_name, int flags){
    Codec10::write_header(op_code, cache_name, flags, VERSION_11);
}

int Codec11::read_new_topology_if_present(){
    short topology_change =transport.read_byte();
    if(DEBUG) std::cout <<"* Topology change marker: "<< std::hex <<topology_change<<std::endl;

    if(topology_change == 0x01){
        int topology_id;
        u_int num_key_own;
        u_short hash_ver;
        int hash_space, num_virtual, num_servers;

        topology_id = transport.read_varint();
        transport.transportFactory.set_topology_id(topology_id);

        num_key_own = transport.read_2bytes();
        transport.transportFactory.set_num_key_owners(num_key_own);

        hash_ver = transport.read_byte();

        hash_space = transport.read_varint();
        transport.transportFactory.set_max_hash_size(hash_space);

        num_servers = transport.read_varint();

        transport.transportFactory.set_virtual_nodes_num(0); // rozdil ve verzich
        num_virtual = transport.read_varint();
        transport.transportFactory.set_virtual_nodes_num(num_virtual);


      if(DEBUG){
        std::cout <<"** Topology ID: "<<std::dec<<topology_id<<" old: "<<transport.transportFactory.get_topology_id()<<std::endl;
        std::cout <<"** Num key owners: "<<num_key_own<<std::endl;
        std::cout <<"** Hash function version: "<<hash_ver<<std::endl;
        std::cout <<"** Hash space size: "<<hash_space<<std::endl;
        std::cout <<"** Num servers in topology: "<<num_servers<<std::endl;
        std::cout <<"** Num virtual nodes owners: "<<num_virtual<<std::endl;
      }


      int port;
      u_int hash;
      std::string host;


      transport.transportFactory.invalidate_transports();
      for(int i = 0; i<num_servers;i++){
  

        if(DEBUG) std::cout <<"***";
        transport.read_array(&host);
        if(DEBUG) std::cout <<host<<"/";
        
        port = transport.read_2bytes();

        if(DEBUG) std::cout <<std::dec << port; 

        hash = transport.read_4bytes();
        if(DEBUG) std::cout <<std::dec << " + " << hash<< std::endl; 
 
        Transport *t = transport.transportFactory.get_transport(&host, port, hash);
        if(t == NULL){
            t = transport.transportFactory.create_transport(&host, port, hash);
        }
        t->valid = 1;

        // std::cout << transport.transportFactory.transports.size() << std::endl;
        //s->port = 0;

      }
      transport.transportFactory.del_invalid_transports();
      transport.transportFactory.print_hash_bank();
      update_transport_bank();
       transport.transportFactory.print_hash_bank();

  }


}

void Codec11::update_transport_bank(){
  pthread_mutex_lock (&transport.transportFactory.mutex);
  Transport *tmp_transport;
  
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  transport.transportFactory.hash_transport_bank.clear();
  transport.transportFactory.hash_vector.clear();
  for(int i = 0;i<transport.transportFactory.transports.size();i++){
        tmp_transport = transport.transportFactory.transports.front();

        int nodeBaseHashCode = tmp_transport->hash;
        int virtualNodeBaseHashCode = 0;
        int virtualNodeHashCode = 0;
        int virtual_nodes_num = transport.transportFactory.get_virtual_nodes_num();
        for(int id=0; id < virtual_nodes_num or id == 0; id++){
            virtualNodeBaseHashCode = 31 * id + nodeBaseHashCode;
            virtualNodeHashCode = (MurmurHash3_x64_32( (const char *) &virtualNodeBaseHashCode, 4,9001) & INT_MAX);
            virtualNodeHashCode = transport.transportFactory.get_hash( (const char *) &virtualNodeBaseHashCode, 4);
            transport.transportFactory.hash_transport_bank.push_back((std::make_pair(virtualNodeHashCode,tmp_transport)));
            transport.transportFactory.hash_vector.push_back(virtualNodeHashCode);
 
        }


        transport.transportFactory.transports.push(tmp_transport);
        transport.transportFactory.transports.pop();        
  }
  // std::cout << "SIZE" << transport.transportFactory.hash_transport_bank.size() << std::endl; 

  


  std::sort(transport.transportFactory.hash_transport_bank.begin(), transport.transportFactory.hash_transport_bank.end());
  std::sort(transport.transportFactory.hash_vector.begin(), transport.transportFactory.hash_vector.end());

    
  pthread_mutex_unlock (&transport.transportFactory.mutex);

}