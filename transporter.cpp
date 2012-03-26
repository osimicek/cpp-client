#include "transporter.h"


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



response Transporter::send(const char *packet,int len, int *state){
        //std::cout << "send"<<std::endl;
        response ret;
        int s;

        write_data(packet,len);
        
        /*if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
        }*/
        s = choose_socket();
        PacketParser *PP = new PacketParser(s);
        ret = PP->read_data(state);

        return ret;
}

response Transporter::send(const char *packet,int len, int *state,long long *data){
        //std::cout << "send"<<std::endl;
        response ret;
        int s;

        write_data(packet,len);
        
        /*if(DEBUG){
          std::cout <<"********** END RESPONSE **********\n**********************************\n";
          //std::cout <<"\n"<<*data<<std::endl;
        }*/
        s = choose_socket();
        PacketParser *PP = new PacketParser(s);
        ret = PP->read_data(state,data);

        return ret;
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

int Transporter::choose_socket(){
    return sock;
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

