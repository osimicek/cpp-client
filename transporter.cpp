#include "transporter.h"


Transporter::Transporter(std::string parhost,int parport){
      //std::cout <<" OK\n" << std::flush;
	    host = parhost;
      port = parport;
      cache_name = "";
      hotrod_version = 0x0b;
      topology_id = 0x00;

      server s;
      s.socket = 0;
      s.used = 0;
      s.valid = 1;
      s.host = parhost;
      s.port = parport;
      update_servers(&s);

      //sock = create_connection(host,port);

      //std::cout << sock<<"\n"<<host<<":"<<port<<std::endl;
}


int Transporter::create_connection(std::string h,int p){
  struct sockaddr_in sin;
  sin.sin_family = PF_INET;
  sin.sin_port = htons(p);
  struct hostent *hptr;
  int ret_socket;

  if((ret_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)  // vytvoreni socketu
  {
    std::cerr << "error socket"<<std::endl;
    return 0;//EXIT_FAILURE;
  }

  if((hptr = gethostbyname(h.c_str())) == NULL)  // preklad URL adresy
  {
    std::cerr << "error host"<<std::endl;
    return 0;//EXIT_FAILURE;
  }
  
  memcpy(&sin.sin_addr, hptr->h_addr_list[0], hptr->h_length);

  if(connect(ret_socket, (struct sockaddr*) &sin, sizeof(sin)) < 0)  // navazani spojeni se serverem
  {
    std::cerr << "error connect"<<std::endl;
    return 0;//EXIT_FAILURE;
  }
  return ret_socket;

    //std::cout << "cconnected"<<std::endl;
}



int Transporter::send(const char *packet,int len, int *state, response *resp){
        int s,ret;
        s = choose_socket();
        write_data(packet,len,s);
        
        /*if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
        }*/
        
        //std::cout <<"socket status "<<s<<std::endl;
        PacketParser *PP = new PacketParser(s,*this);
        long long unused;
        ret = PP->read_data(state,&unused,resp);
        release_server(s);
        return ret;

}

int Transporter::send(const char *packet,int len, int *state,long long *data,response *resp){
        int s,ret;
        s = choose_socket();
        write_data(packet,len,s);
        
        /*if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
          //std::cout <<"\n"<<*data<<std::endl;
        }*/
        
        PacketParser *PP = new PacketParser(s,*this);
        ret = PP->read_data(state,data,resp);
        release_server(s);
        return ret;

}




void Transporter::write_data(const char *data,int len,int s){
    
  while(1){
	  if(write(s, data, len) < 0)  // odeslani pozadavku na server 24
	  {
	    //printError(ESOCKETWRITE);
	    std::cerr << "error zapis"<<std::endl;
      if(s != 0){
        s = reconnect_server(s);
      }
      if(s == 0){
        s = choose_socket();
      }
	  }else{break;}
  }
        //std::cout << "write"<<std::endl;
}

int Transporter::choose_socket(){
  server *tmp_server;
  int s;
  
  tmp_server = &(this->servers.front());
  tmp_server->used = 1;

  if(tmp_server->socket == 0){
     s = create_connection(tmp_server->host,tmp_server->port);
     //std::cout <<"ii " <<tmp_server->host<<tmp_server->port<<std::flush<<std::endl;
     tmp_server->socket = s;
  }else{
    s = tmp_server->socket;
  }

  this->servers.push(*tmp_server);
  this->servers.pop();
  return s;
}

void Transporter::release_server(int s){
  server *tmp_server;
  int ret_socket = 0;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());
        if(tmp_server->socket == s){
          tmp_server->used = 0;
        }
        this->servers.push(*tmp_server);
        this->servers.pop();
        //this->servers.pop();
        
  }  

}

int Transporter::reconnect_server(int s){
  server *tmp_server;
  int ret_socket = 0;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());
        if(tmp_server->socket == s){
          ret_socket = create_connection(tmp_server->host,tmp_server->port);
          tmp_server->socket = ret_socket;
          tmp_server->used = 1;
        }
        this->servers.push(*tmp_server);
        this->servers.pop();
        //this->servers.pop();
        
  }   
  return ret_socket;
}

void Transporter::close_connection(int s){
	  if(s and close(s) < 0)  // uzavreni socketu
  {
    //printError(ESOCKETCLOSE);
    std::cerr << "error during closing socket "<< s<<std::endl;
    return;// EXIT_FAILURE;
  }
    std::cerr << "connection closed"<<std::endl;
}



void Transporter::close_servers(){
  server *tmp_server;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());
        close_connection(tmp_server->socket);
        tmp_server->socket = 0;
        tmp_server->used =0;
        
        this->servers.push(*tmp_server);
        this->servers.pop();
        //this->servers.pop();
        
  } 
}

void Transporter::update_servers(server *s){
  server *tmp_server;
  int changed = 0;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        //std::cout << std::dec<<this->servers.front().host<<"/"<<this->servers.front().port<<std::flush<<std::endl;
        tmp_server = &(this->servers.front());
        //std::cout << std::dec<<tmp_server->host<<"/"<<tmp_server->port<< "  "<<s->host<<"/"<<s->port<< " - "<<(int)(tmp_server->host==s->host)<< (int)(tmp_server->port==s->port)<<std::flush<<std::endl;
        //std::cout << std::dec<<this->servers.front().host<<"/"<<this->servers.front().port<<std::flush<<std::endl;
        if(tmp_server->port == s->port and tmp_server->host == s->host){
            tmp_server->hash = s->hash;
            tmp_server->valid = s->valid;
            changed = 1;
        }

        this->servers.push(*tmp_server);
        this->servers.pop();
        //this->servers.pop();
       
  } 
  if(changed == 0){
    this->servers.push(*s);
  }  
}

void Transporter::invalidate_servers(){
  server *tmp_server;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());
        tmp_server->valid = 0;
        
        this->servers.push(*tmp_server);
        this->servers.pop();
        //this->servers.pop();
        
  } 
}

void Transporter::del_invalid_servers(){
  server *tmp_server;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());
        if(tmp_server->valid == 1){ 
          this->servers.push(*tmp_server);
        }
        this->servers.pop();
        //this->servers.pop();
        
  } 

}

/*int Transporter::connected(int ssocket)
{
     char buf;
     int err = recv(ssocket, &buf, 1, MSG_PEEK);
     if(err == SOCKET_ERROR)
     {
          if(WSAGetLastError() != EWOULDBLOCK)
          {return 0;}
     }
     return 1;
}*/