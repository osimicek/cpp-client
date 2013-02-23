#include <packetParser.h>

int DEBUG = 1;
int SERVERS_DEBUG = 1;

PacketParser::PacketParser(int s,Transporter &t):transporter(t)
{
 	sock = s;
 
}

int PacketParser::decode_varint(){
  int result = 0;
  int shift = 0;
  int n;
  u_short tmp_buf[1];
  while (1){
    n = recv(sock, tmp_buf, 1,0);  // cteni dat ze socketu
    result |= ((int)(tmp_buf[0] & 0x7f) << shift);
    //std::cout << shift << ' '<< result<< " "  << std::hex << tmp_buf[0]<<std::endl;
    //std::cout <<std::dec<< shift << " " <<result<< " " << std::hex << result<<std::endl;
    if (!(tmp_buf[0] & 0x80)){
      return result;
    }
    shift += 7;
    if (shift >= 32) {}//vyjimka
      //raise DecodeError("Too many bytes when decoding varint.")
  }
}

long long PacketParser::decode_varlong(){
  long long result = 0;
  int shift = 0;
  int n;
  u_short tmp_buf[1];
  while (1){
    n = recv(sock, tmp_buf, 1,0);  // cteni dat ze socketu
    result |= ((long long)(tmp_buf[0] & 0x7f) << shift);
    
    //std::cout << shift << ' '<< result<< " "  << std::hex << "0x"<<(int)tmp_buf[0]<<std::endl;
    //std::cout <<std::dec<< shift << " " <<result<< " " << std::hex << result<<std::endl;
    if (!(tmp_buf[0] & 0x80)){
      return result;
    }
    shift += 7;
    if (shift >= 64) {}//vyjimka
      //raise DecodeError("Too many bytes when decoding varint.")
  }
}

int PacketParser::read_data(int *state,long long *data,response *resp){
  char buffer;
  u_short buffer2[9];
  int n;
  int status,op_code;
  int err_len, err_msg_counter;
  long long msg_id;
  int topology_change = 0;
  int topology_id = 0;
  u_int num_key_own;
  u_short hash_ver;
  int hash_space, num_virtual, num_servers;


    //0-OK  1-Err

  buffer2[0] ='\0'; 
  if(DEBUG){
    std::cout <<"\n**********************************\n************ RESPONSE ************\n";
  }
  n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
  buffer2[n] = '\0';
  if(DEBUG){
    std::cout <<"* Magic: "<< std::hex << buffer2[0]<<std::endl;
  }
  msg_id = decode_varlong();
  if(DEBUG){
    std::cout <<"* Message ID: 0x"<<msg_id<< " - "<<std::dec << msg_id<<std::endl;
  }
  n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
  buffer2[n] = '\0';
  if(DEBUG){
    std::cout <<"* Op code: "<< std::hex << buffer2[0]<<std::endl;
  }
  op_code = buffer2[0];

  n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
  buffer2[n] = '\0';
  if(DEBUG){
    std::cout <<"* Status: "<< std::hex << buffer2[0]<<std::endl;
  }
  status = buffer2[0];
  *state = status;

  n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
  topology_change = buffer2[0];
  if(DEBUG){
    std::cout <<"* Topology change marker: "<< std::hex << buffer2[0]<<std::endl;
  }
  if(topology_change == 0x01){
      int server_sum = 0;

      topology_id = decode_varint();
      transporter.set_topology_id(topology_id);

      n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
      num_key_own = buffer2[0];
      num_key_own <<= 8;
      n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
      num_key_own |= buffer2[0];

      n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
      hash_ver= buffer2[0];

      hash_space = decode_varint();
      transporter.set_max_hash_size(hash_space);

      num_servers = decode_varint();
      server_sum = num_servers; 

      transporter.set_virtual_nodes_num(0);
      if(transporter.hotrod_version == 0x0b){ // added for hotrod 1.1
        num_virtual = decode_varint();
        transporter.set_virtual_nodes_num(num_virtual);
        //server_sum = num_virtual;
      }

      if(SERVERS_DEBUG){
        std::cout <<"** Topology ID: "<<std::dec<<topology_id<<" old: "<<transporter.get_topology_id()<<std::endl;
        std::cout <<"** Num key owners: "<<num_key_own<<std::endl;
        std::cout <<"** Hash function version: "<<hash_ver<<std::endl;
        std::cout <<"** Hash space size: "<<hash_space<<std::endl;
        std::cout <<"** Num servers in topology: "<<num_servers<<std::endl;
        if(transporter.hotrod_version == 0x0b){
          std::cout <<"** Num virtual nodes owners: "<<num_virtual<<std::endl;
        }
      }
      transporter.invalidate_servers();

      int host_len;
      u_int host_port;
      u_int hash;
      for(int i = 0; i<server_sum;i++){
        server s;
        s.socket = 0;
        s.used = 0;
        s.valid = 1;

        if(SERVERS_DEBUG){
            std::cout <<"***";
        } 
        host_len = decode_varint();
        for(int j = 0;j<host_len;j++){ //host name
          n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
          buffer2[n] = '\0';
          s.host += buffer2[0];
          if(SERVERS_DEBUG){
            std::cout <<(char)buffer2[0];
          }  

        }
        if(SERVERS_DEBUG){
            std::cout <<"/";
        } 
        n = recv(sock, buffer2, 1,0);    
 // cteni dat ze socketu
        host_port = buffer2[0];         //host port
        host_port <<= 8;
        n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
        host_port |= (u_short)buffer2[0];
        s.port = host_port;
        if(SERVERS_DEBUG){
          std::cout <<host_port;
        }  

        hash = 0;
        for(int k=0; k<4; k++){
          n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
          hash <<= 8;
          hash |= buffer2[0];
        }
        
       std::cout<<std::endl;

        s.hash = hash;

        // if(SERVERS_DEBUG){
        //   std::cout <<"\t"<<s.hash<<std::endl;
        // }  


        transporter.update_servers(&s);
        //s->port = 0;

      }
      transporter.del_invalid_servers();
      transporter.update_servers_bank();


  }

  if(status != 0x00) //chyba
  {
      if(status == 0x01){
        if(DEBUG){
          std::cerr <<"* Not put/remove/replaced: "<<std::endl;
        }
        return 1; //empty
      } else if(status == 0x02){
        if(DEBUG){
          std::cerr <<"* Key does not exist: "<<std::endl;
        }
        return 1; //empty
      }

      n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
      buffer2[n] = '\0';
      if(DEBUG){
        std::cerr <<"* Error Message Length: "<< std::hex << buffer2[0]<<std::endl;
      }
      err_len = (u_short)buffer2[0];
      if(DEBUG){
        std::cout << "* ";
      }
      
      for(err_msg_counter = 0;err_msg_counter < err_len; err_msg_counter++)
      {
      	
        n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
        buffer2[n] = '\0';
        if(DEBUG){
          std::cout << (char*)buffer2;
        }
        
        //std::cout << n<<" ";
        //std::cout << buffer2<<" "<< std::hex << (u_short)buffer2[0]<<std::endl;

      }
      if(DEBUG){
        std::cout << std::endl;
      }
      return 1; //empty
  }

  if(op_code == 0x04 )//get response
  {
    int value_len = decode_varint();
    if(DEBUG){
      std::cout <<"* Value Length: "<< value_len<<std::endl;
    }
    if(DEBUG){
      std::cout << "* ";
    }
    
    for(int i=0;i < value_len; i++)
      {
        
        n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
        buffer2[n] = '\0';
        if(DEBUG){
          std::cout << (char*)buffer2;
        }
        *(*resp).data += buffer2[0];
        //std::cout << n<<" ";
        //std::cout << buffer2<<" "<< std::hex << (u_short)buffer2[0]<<std::endl;

      }
      if(DEBUG){
        std::cout << std::endl;
      }
  }


  if(op_code == 0x12){//get with version
    if(DEBUG){
      std::cout <<"* Version: ";
    }
    for(int i=0;i<8;i++){
      n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
      buffer2[n] = '\0';
      if(DEBUG){
        std::cout <<std::hex << buffer2[0];
      }
      *data <<= 8;
      *data |= (u_short)buffer2[0];
    }
    if(DEBUG){
      std::cout <<std::endl;
    }

    int value_len = decode_varint();
    if(DEBUG){
      std::cout <<"* Value Length: "<< value_len<<std::endl;
      std::cout << "* ";
    }

    for(int i=0;i < value_len; i++)
      {
        
        n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
        buffer2[n] = '\0';
        if(DEBUG){
          std::cout << (char*)buffer2;
        }
        *(*resp).data += buffer2[0];
        //std::cout << n<<" ";
        //std::cout << buffer2<<" "<< std::hex << (u_short)buffer2[0]<<std::endl;

      }
      if(DEBUG){
        std::cout << std::endl;  
      }

  }

  if(op_code == 0x18){
      struct timeval end;

      gettimeofday(&end, NULL);

      long long the_time;
        
      the_time = end.tv_sec;
      the_time <<= 32;
      the_time += end.tv_usec;

      *data = the_time - msg_id;
      if(DEBUG){
        std::cout <<std::dec << "ping in ms: "<< (*data)/1000<<std::endl;
      }
    }
    

    if(op_code == 0x1a){ //getBulk
      int key_len = 0;
      int value_len = 0;
      std::string tmp_key, tmp_value;
      while(1){
        n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
        if(buffer2[0] == 0x00) break;
        key_len = decode_varint();
        if(DEBUG){
          std::cout << "* ";
        }
        tmp_key.clear();
        for(int i = 0;i < key_len; i++)
        {  
          n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
          buffer2[n] = '\0';
          if(DEBUG){
            std::cout << (char*)buffer2;
          }
          tmp_key += (char*)buffer2;

          //std::cout << n<<" ";
          //std::cout << buffer2<<" "<< std::hex << (u_short)buffer2[0]<<std::endl;
        }
        if(DEBUG){
          std::cout << " : ";
        }
        value_len = decode_varint();
        tmp_value.clear();
        for(int i = 0;i < value_len; i++)
        {  
          n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
          buffer2[n] = '\0';
          if(DEBUG){
            std::cout << (char*)buffer2;
          }
          tmp_value += (char*)buffer2;
          //std::cout << n<<" ";
          //std::cout << buffer2<<" "<< std::hex << (u_short)buffer2[0]<<std::endl;
        }
        (*(resp->bulk))[tmp_key] = tmp_value;
        if(DEBUG){
          std::cout << std::endl;
        }


      }
    }
    return 0; 
}

 