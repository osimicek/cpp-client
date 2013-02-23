#include <transporter.h>

int T_DEBUG = 1;

Transporter::Transporter(std::string parhost,int parport){
      //std::cout <<" OK\n" << std::flush;
	    host = parhost;
      port = parport;
      cache_name = "";
      hotrod_version = 0x0b;
      topology_id = 0x00;

      pthread_mutex_init(&mutex, NULL);
      pthread_mutex_init(&mutex_t_id, NULL);

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

void Transporter::set_topology_id(int id){
    pthread_mutex_lock (&mutex_t_id);
      topology_id = id;
    pthread_mutex_unlock (&mutex_t_id);
}
int Transporter::get_topology_id(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = topology_id;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}

void Transporter::set_virtual_nodes_num(int id){
    pthread_mutex_lock (&mutex_t_id);
      virtual_nodes_num = id;
    pthread_mutex_unlock (&mutex_t_id);
}
int Transporter::get_virtual_nodes_num(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = virtual_nodes_num;;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
}

void Transporter::set_max_hash_size(int id){
    pthread_mutex_lock (&mutex_t_id);
      max_hash_size = id;
    pthread_mutex_unlock (&mutex_t_id);
}
int Transporter::get_max_hash_size(){
    pthread_mutex_lock (&mutex_t_id);
    int ret = max_hash_size;;
    pthread_mutex_unlock (&mutex_t_id);
    return ret;
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
        if(T_DEBUG){
         std::cout <<"Mam socket "<<s<<std::endl;
        }
        write_data(packet,len,s);
        
        /*if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
        }*/
        
        //std::cout <<"socket status "<<s<<std::endl;
        PacketParser *PP = new PacketParser(s,*this);
        long long unused;
        ret = PP->read_data(state,&unused,resp);
        if(T_DEBUG){
          std::cout <<"Vracim socket "<<s<<std::endl;
        }
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
        release_server(s);
        s = choose_socket();
      }
	  }else{break;}
  }
        //std::cout << "write"<<std::endl;
}

int Transporter::choose_socket(){
  pthread_mutex_lock (&mutex);
  server *tmp_server;
  int s;

  if(this->servers.size() == 0) return 0;
  int target = 946978224;
  int dist = max_hash_size;
  int tmp_dist = 0;
  int index = 0;
  int half_size = max_hash_size / 2;
  int current_hash = 0;
  
  for(int i=0; i < this->hash_server_bank.size(); i++){
    current_hash = this->hash_server_bank[i].first;
    tmp_dist = current_hash - target;// % half_size;
    if(tmp_dist < dist and tmp_dist >=0){
      dist = tmp_dist;
      index = i;
      std::cout << "prubezne "<< index << "  "<< this->hash_server_bank[index].first<<"  "<<this->hash_server_bank[index].second->port<<std::endl;
    }


  }
  if (index){
    std::cout << "OLEEE "<< index << "  "<< this->hash_server_bank[index].first<<"  "<<this->hash_server_bank[index].second->port<<std::endl;
  }


  while(1){

    tmp_server = &(this->servers.front());   
/*
    this->servers.push(*tmp_server);
    this->servers.pop();*/

    //std::cout << "choosed "<<&tmp_server<<std::flush<<std::endl;

    //sleep(0.1);
    if(tmp_server->used ==0){
      if(tmp_server->socket == 0){
         s = create_connection(tmp_server->host,tmp_server->port);
         //std::cout << "changing"<<std::flush<<std::endl;
         //std::cout <<"ii " <<tmp_server->host<<tmp_server->port<<std::flush<<std::endl;
         tmp_server->socket = s;
      }else{
          s = tmp_server->socket;
      }
      tmp_server->used = 1; 
      this->servers.push(*tmp_server);
      this->servers.pop();
      break;
    }
    this->servers.push(*tmp_server);
    this->servers.pop();
    pthread_mutex_unlock (&mutex);
    //sleep(1);
    pthread_mutex_lock (&mutex);

  }
  pthread_mutex_unlock (&mutex);
  return s;
}

void Transporter::release_server(int s){
  pthread_mutex_lock (&mutex);
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
  pthread_mutex_unlock (&mutex); 

}

int Transporter::reconnect_server(int s){
  pthread_mutex_lock (&mutex);
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
  pthread_mutex_unlock (&mutex);
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
  pthread_mutex_lock (&mutex);
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
  pthread_mutex_unlock (&mutex);
}

void Transporter::update_servers(server *s){
  pthread_mutex_lock (&mutex);
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
       
  } 
  if(changed == 0){
    this->servers.push(*s);
  }  
  pthread_mutex_unlock (&mutex);
}

void Transporter::invalidate_servers(){
  pthread_mutex_lock (&mutex);
  server *tmp_server;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());
        tmp_server->valid = 0;
        
        this->servers.push(*tmp_server);
        this->servers.pop();
        //this->servers.pop();
        
  } 
  pthread_mutex_unlock (&mutex);
}

void Transporter::del_invalid_servers(){
  pthread_mutex_lock (&mutex);
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
  pthread_mutex_unlock (&mutex);

}
bool compareByLength(const std::pair<int,server*> a, const std::pair<int,server*> b)
  {
      return a.first < b.first;
  }
void Transporter::update_servers_bank(){
  pthread_mutex_lock (&mutex);
  server *tmp_server;
  //std::cout <<"ii " <<std::dec<<s->host<<"/"<<s->port<<std::flush<<std::endl;
  for(int i = 0;i<this->servers.size();i++){
        tmp_server = &(this->servers.front());

        int nodeBaseHashCode = tmp_server->hash;
        int virtualNodeBaseHashCode = 0;
        int virtualNodeHashCode = 0;
        for(int id=0; id < virtual_nodes_num or id == 0; id++){
            virtualNodeBaseHashCode = 31 * id + nodeBaseHashCode;
            virtualNodeHashCode = (MurmurHash3_x64_32( (const char *) &virtualNodeBaseHashCode, 4,9001) & INT_MAX);
            
            this->hash_server_bank.push_back((std::make_pair(virtualNodeHashCode,tmp_server)));
           

            std::cout <<"\n\t"<<virtualNodeHashCode;
           
        }
        //std::cout<< "xx"<<this->servers_bank[0]->port<<std::endl;

        this->servers.push(*tmp_server);
        this->servers.pop();        
  } 

  


  std::sort(this->hash_server_bank.begin(), this->hash_server_bank.end());

  pthread_mutex_unlock (&mutex);

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