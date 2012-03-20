#include "transporter.h"
int DEBUG = 0;

Transporter::Transporter(std::string parhost,std::string parport){
      //std::cout <<" OK\n" << std::flush;
	    host = parhost;
      port = parport;
      cache_name = "";

      create_connection();
        //std::cout << "Transporter zije "<<"\n"<<host<<":"<<port<<std::endl;
}

void Transporter::create_connection(){
  struct sockaddr_in sin;
  sin.sin_family = PF_INET;
  sin.sin_port = htons(11222);
  struct hostent *hptr;

  if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)  // vytvoreni socketu
  {
    std::cerr << "error socket"<<std::endl;
    return ;//EXIT_FAILURE;
  }

  if((hptr = gethostbyname(host.c_str())) == NULL)  // preklad URL adresy
  {
    std::cerr << "error host"<<std::endl;
    return ;//EXIT_FAILURE;
  }
  
  memcpy(&sin.sin_addr, hptr->h_addr_list[0], hptr->h_length);

  if(connect(sock, (struct sockaddr*) &sin, sizeof(sin)) < 0)  // navazani spojeni se serverem
  {
    std::cerr << "error connect"<<std::endl;
    return ;//EXIT_FAILURE;
  }

    //std::cout << "cconnected"<<std::endl;
}

int Transporter::decode_varint(){
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

long long Transporter::decode_varlong(){
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

response Transporter::send(const char *packet,int len, int *state){
        //std::cout << "send"<<std::endl;
        response ret;

        write_data(packet,len);
        ret = read_data(state);
        if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
        }
        PacketParser *PP = new PacketParser(this);

        return ret;
}

response Transporter::send(const char *packet,int len, int *state,long long *data){
        //std::cout << "send"<<std::endl;
        response ret;

        write_data(packet,len);
        ret = read_data(state,data);
        if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
          //std::cout <<"\n"<<*data<<std::endl;
        }
        PacketParser *PP = new PacketParser(this);

        return ret;
}



response Transporter::read_data(int *state,long long *data){
  
  result = "";
  resp.data = &result;
  char buffer;
  u_short buffer2[9];
  int n;
  int status,op_code;
  int err_len, err_msg_counter;
  long long msg_id;


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
  buffer2[n] = '\0';
  if(DEBUG){
    std::cout <<"* Topology change marker: "<< std::hex << buffer2[0]<<std::endl;
  }

  if(status != 0x00) //chyba
  {
      if(status == 0x01){
        std::cerr <<"* Not put/remove/replaced: "<<std::endl;
              return resp; //empty
      } else if(status == 0x02){
        std::cerr <<"* Key does not exist: "<<std::endl;
              return resp; //empty
      }

      n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
      buffer2[n] = '\0';
      std::cerr <<"* Error Message Length: "<< std::hex << buffer2[0]<<std::endl;
      err_len = (u_short)buffer2[0];

      std::cout << "* ";
      for(err_msg_counter = 0;err_msg_counter < err_len; err_msg_counter++)
      {
      	
        n = recv(sock, buffer2, 1,0);  // cteni dat ze socketu
        buffer2[n] = '\0';
        std::cout << (char*)buffer2;
        //std::cout << n<<" ";
        //std::cout << buffer2<<" "<< std::hex << (u_short)buffer2[0]<<std::endl;

      }
      if(DEBUG){
        std::cout << std::endl;
      }
      return resp; //empty
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
        *(resp.data) += buffer2[0];
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
        *(resp.data) += buffer2[0];
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
        bulk[tmp_key] = tmp_value;
        if(DEBUG){
          std::cout << std::endl;
        }


      }
      resp.bulk = &bulk;
    }
    return resp; //may containst restult of get operation

}
void Transporter::write_data(const char *data,int len){
    

	  if(write(sock, data, len) < 0)  // odeslani pozadavku na server 24
	  {
	    //printError(ESOCKETWRITE);
	    std::cerr << "error zapis"<<std::endl;
	    return;// EXIT_FAILURE;
	  }
        //std::cout << "write"<<std::endl;
}

void Transporter::close_connection(){
	  if(close(sock) < 0)  // uzavreni socketu
  {
    //printError(ESOCKETCLOSE);
    std::cerr << "error during closing socket"<<std::endl;
    return;// EXIT_FAILURE;
  }
    std::cerr << "connection closed"<<std::endl;
}

