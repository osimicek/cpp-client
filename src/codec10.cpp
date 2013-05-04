#include "codec.h"

Codec10::Codec10(Transport &t):Codec(t){}

void Codec10::write_header(char op_code, const std::string *cache_name, int flags){
    write_header(op_code, cache_name, flags, VERSION_10);
}   




int Codec10::read_header(){
    char magic;
    long received_message_id;
    char received_op_code;
    int status;



    magic = transport.read_byte();
    if (magic != RESPONSE_MAGIC) {
        if(DEBUG){
            std::cerr <<std::hex<< "INVALID_SERVER_MAGIC, got "<<   (int)magic<< std::endl;
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

int Codec10::check_for_errors_in_response_status(int status){
    if(status == NOT_PUT_REMOVED_REPLACED_STATUS){
        return status;
    } else if(status == KEY_DOES_NOT_EXIST_STATUS){
        return status; 
    }

    std::string error_msg;

    transport.read_array(&error_msg);
    if(DEBUG) std::cerr <<"* Error Message: "<< error_msg<<std::endl;
    return status; 
}

void Codec10::read_new_topology_if_present(){
    short topology_change =transport.read_byte();
    if(DEBUG){
        std::cout <<"* Topology change marker: "<< std::hex <<topology_change<<std::endl;
    }
    if(topology_change == 0x01){
        int topology_id;
        u_int key_own_num;
        u_short hash_ver;
        int hash_space, num_servers;

        topology_id = transport.read_varint();
        transport.transportFactory.set_topology_id(topology_id);

        key_own_num = transport.read_2bytes();
        transport.transportFactory.set_key_owners_num(key_own_num);

        hash_ver = transport.read_byte();
        transport.transportFactory.set_hash_ver(hash_ver);

        hash_space = transport.read_varint();
        transport.transportFactory.set_max_hash_size(hash_space);

        num_servers = transport.read_varint();

        transport.transportFactory.set_virtual_nodes_num(0); // rozdil ve verzich


      if(DEBUG){
        std::cout <<"** Topology ID: "<<std::dec<<topology_id<<" old: "<<transport.transportFactory.get_topology_id()<<std::endl;
        std::cout <<"** Num key owners: "<<key_own_num<<std::endl;
        std::cout <<"** Hash function version: "<<hash_ver<<std::endl;
        std::cout <<"** Hash space size: "<<hash_space<<std::endl;
        std::cout <<"** Num servers in topology: "<<num_servers<<std::endl;

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
       
        Transport *t = transport.transportFactory.get_transport(&host, port, hash);
        if(t == NULL){
            t = transport.transportFactory.create_transport(&host, port, hash);
        }
        t->valid = 1;

      }
      transport.transportFactory.del_invalid_transports();
      update_transport_bank();
      // transport.transportFactory.print_hash_bank();


  }

}

void Codec10::update_transport_bank(){
  LOCK_T_TF();
  Transport *tmp_transport;
  
  transport.transportFactory.hash_transport_bank.clear();
  transport.transportFactory.hash_vector.clear();
  for(u_int i = 0;i<transport.transportFactory.transports.size();i++){
        tmp_transport = transport.transportFactory.transports.front();

        int nodeHashCode = tmp_transport->hash;

        transport.transportFactory.hash_transport_bank.push_back((std::make_pair(nodeHashCode,tmp_transport)));
        transport.transportFactory.hash_vector.push_back(nodeHashCode);
 


        transport.transportFactory.transports.push(tmp_transport);
        transport.transportFactory.transports.pop();        
  }


  std::sort(transport.transportFactory.hash_transport_bank.begin(), transport.transportFactory.hash_transport_bank.end());
  std::sort(transport.transportFactory.hash_vector.begin(), transport.transportFactory.hash_vector.end());

    
  UNLOCK_T_TF();

}


