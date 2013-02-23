#define DONT_INCLUDE_CODEC11  //circular dependency
#include "codec10.h"
#undef DONT_INCLUDE_CODEC11  //circular dependency
// Codec10::Codec10(){}
Codec10::Codec10(Transport &t):transport(t){}

void Codec10::write_header(char op_code){
     std::cout << "WRITE 10"<< std::endl;
    write_header(op_code, 10);
}   

void Codec10::write_header(char op_code, char version){
    std::cout << "WRITE"<< version<< std::endl;
    struct timeval begin;
    gettimeofday(&begin, NULL);
    long long the_time;  
    the_time = begin.tv_sec;
    the_time <<= 32;
    the_time += begin.tv_usec;

    transport.packet.clear(); //clear packet 
    transport.write_char(REQUEST_MAGIC); //HotRod request
    transport.write_varlong(1);//encode_varlong(the_time); //msg ID

    transport.write_char(version); //version 10 or 11
    transport.write_char(op_code); //op_code
    transport.write_char(0x00); //cache name length
    transport.write_char(0x00); //flags
    transport.write_char(CLIENT_INTELLIGENCE_HASH_DISTRIBUTION_AWARE); //inteligence
    transport.write_varint(transport.transportFactory.get_topology_id()); //topology
    //todo change once TX support is added
    transport.write_char(0x00); //transaction type
}


int Codec10::read_header(){
    char magic;
    long received_message_id;
    char received_op_code;
    char status;



    magic = transport.read_byte();
    //std::cout <<std::hex<<"INVALID_SERVER_MAGIC"<< magic<< std::endl;
    if (magic != RESPONSE_MAGIC) {
        if(DEBUG){
            std::cerr <<std::hex<< "INVALID_SERVER_MAGIC, got "<< (u_short)magic<< std::endl;
        }
        return INVALID_SERVER_MAGIC;
    }
    received_message_id = transport.read_varlong();
    received_op_code = transport.read_byte();

    status = transport.read_byte();

    read_new_topology_if_present();

    if (received_op_code == ERROR_RESPONSE) {
        return check_for_errors_in_response_status(status);
    }
    return status;
}

int Codec10::check_for_errors_in_response_status(char status){
    if(status == NOT_PUT_REMOVED_REPLACED_STATUS){
        if(DEBUG){
            std::cerr <<"* Not put/remove/replaced: "<<std::endl;
        }
        return status;
    } else if(status == KEY_DOES_NOT_EXIST_STATUS){
        if(DEBUG){
          std::cerr <<"* Key does not exist: "<<std::endl;
        }
        return status; 
    }

    std::string error_msg;

    transport.read_array(&error_msg);
    if(DEBUG){
        std::cerr <<"* Error Message: "<< error_msg<<std::endl;
    }
    return status; 
}

int Codec10::read_new_topology_if_present(){
    short topology_change =transport.read_byte();
    if(DEBUG){
        std::cout <<"* Topology change marker: "<< std::hex <<topology_change<<std::endl;
    }
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
        if(transport.transportFactory.get_hotrod_version() == 0x0b){ // added for hotrod 1.1
            num_virtual = transport.read_varint();
            transport.transportFactory.set_virtual_nodes_num(num_virtual);
            //num_servers = num_virtual;
        }

      if(DEBUG){
        std::cout <<"** Topology ID: "<<std::dec<<topology_id<<" old: "<<transport.transportFactory.get_topology_id()<<std::endl;
        std::cout <<"** Num key owners: "<<num_key_own<<std::endl;
        std::cout <<"** Hash function version: "<<hash_ver<<std::endl;
        std::cout <<"** Hash space size: "<<hash_space<<std::endl;
        std::cout <<"** Num servers in topology: "<<num_servers<<std::endl;
        if(transport.transportFactory.get_hotrod_version() == 0x0b){ //rozdil ve verzi
          std::cout <<"** Num virtual nodes owners: "<<num_virtual<<std::endl;
        }
      }


      int port;
      u_int hash;
      std::string host;


      transport.transportFactory.invalidate_transports();
      for(int i = 0; i<num_servers;i++){
  

        if(DEBUG){
            std::cout <<"***";
        } 
        transport.read_array(&host);
        if(DEBUG){
            std::cout <<host<<"/";
        } 
        port = transport.read_2bytes();

        if(DEBUG){
          std::cout <<std::dec << port;
        }  

        hash = transport.read_4bytes();
        if(DEBUG){
          std::cout <<std::dec << " + " << hash<< std::endl;
        }  
        // std::cout<<std::endl;
        Transport *t = transport.transportFactory.get_transport(&host, port);
        if(t == NULL){
            // std::cout << "create " << port << std::endl;
            t = transport.transportFactory.create_transport(&host, port);
        }
        t->valid = 1;
        t->hash = hash;
        std::cout << transport.transportFactory.transports.size() << std::endl;
        //s->port = 0;

      }
      transport.transportFactory.del_invalid_transports();
      transport.transportFactory.print_hash_bank();
      update_transport_bank();
      // transport.transportFactory.print_hash_bank();


  }


}


// bool compareByLength(const std::pair<int,Transport*> a, const std::pair<int,Transport*> b)
//   {
//       return a.first < b.first;
//   }

void Codec10::update_transport_bank(){
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
            
            transport.transportFactory.hash_transport_bank.push_back((std::make_pair(virtualNodeHashCode,tmp_transport)));
            transport.transportFactory.hash_vector.push_back(virtualNodeHashCode);
           

            // std::cout <<"\n\t"<<virtualNodeHashCode;
           
        }
        // std::cout<< "xx"<<virtualNodeHashCode<<std::endl;

        transport.transportFactory.transports.push(tmp_transport);
        transport.transportFactory.transports.pop();        
  }
  std::cout << "SIZE" << transport.transportFactory.hash_transport_bank.size() << std::endl; 

  


  std::sort(transport.transportFactory.hash_transport_bank.begin(), transport.transportFactory.hash_transport_bank.end());
  std::sort(transport.transportFactory.hash_vector.begin(), transport.transportFactory.hash_vector.end());

    
  pthread_mutex_unlock (&transport.transportFactory.mutex);

}


