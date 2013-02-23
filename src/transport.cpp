#include "transport.h"


Transport::Transport(std::string host, int port, TransportFactory &tF):transportFactory(tF)
{
    this->host.assign(host);
    this->port = port;
    this->hash = 0;
    this->used = 0;
    this->_socket = 0;
    this->valid = 1;

    int hotrod_version = transportFactory.get_hotrod_version();
    if(hotrod_version == VERSION_10){
        codec = new Codec10(*this);
    }else if(hotrod_version == VERSION_11){
        codec = new Codec11(*this);
    }else{
        //return ERROR
    }

    //((Codec11 *) codec)->write_header(20);


    
} 

void Transport::write_version(long long value){
    std::string result;
    for(int i=0;i<8;i++){
        char bits = value>>56 & 0xff;
        value <<= 8;
        result += (char)(bits);
    }
    packet += result;
}

void Transport::write_varint(int value){
    std::string result;
    char bits = value & 0x7f;
    value >>= 7;
    while (value){
        result += (char)(0x80|bits);
        
        bits = value & 0x7f;
        value >>= 7;
    }
    result += bits;
    packet += result;
}

void Transport::write_varlong(long long value){
    std::string result;
    char bits = value & 0x7f;
    value >>= 7;
    while (value){
        result += (char)(0x80|bits);
        
        bits = value & 0x7f;
        value >>= 7;
    } 
    result += bits;
    packet += result;
}

void Transport::write_char(char value){
    packet += value; 
}
void Transport::write_byte(short byte){
    packet += (char)byte;
}

void Transport::write_header(char op_code){
    int hotrod_version = transportFactory.get_hotrod_version();
    if(hotrod_version == VERSION_10){
        ((Codec10 *) codec)->write_header(op_code);
    }else if(hotrod_version == VERSION_11){
        ((Codec11 *) codec)->write_header(op_code);
    }else{
        //return ERROR
    }
}

void Transport::write_array(const std::string *arr){
    write_varlong((*arr).length()); // array len
    packet += (*arr); //key
}



int Transport::flush(){
    // sends data to server
    int status = 0;
    std::cout << "_SOCKET " << this->_socket << std::endl;
    if(this->_socket == 0){
        create_connection();
    }
    std::cout << "PACKET " << (u_short)(this->packet.c_str()[3])<< " "<<this->packet.length()<<" "<<this->_socket << std::endl;
    if(write(this->_socket, this->packet.c_str(), this->packet.length()) < 0)  // odeslani pozadavku na server 24
    {
        // std::cerr << "error zapis"<<std::endl;
        if(this->_socket != 0){
            status = create_connection();
        }
        if(status == 0){
            return FAILED_TO_SEND;
        }
    }
}


int Transport::read_varint(){
    int result = 0;
    int shift = 0;
    int n;
    u_short loaded_data[1];
    while (1){
        n = recv(this->_socket, loaded_data, 1,0);  // cteni dat ze socketu
        result |= ((int)(loaded_data[0] & 0x7f) << shift);
        if (!(loaded_data[0] & 0x80)){
          return result;
        }
        shift += 7;
        if (shift >= 32) {}//todo
          //raise DecodeError("Too many bytes when decoding varint.")
    }
}

long long Transport::read_varlong(){
    long long result = 0;
    int shift = 0;
    int n;
    u_short loaded_data[1];
    while (1){
        n = recv(this->_socket, loaded_data, 1,0);  // cteni dat ze socketu
        result |= ((long long)(loaded_data[0] & 0x7f) << shift);
        if (!(loaded_data[0] & 0x80)){
          return result;
        }
        shift += 7;
        if (shift >= 64) {}//todo
          //raise DecodeError("Too many bytes when decoding varint.")
    }

}

char Transport::read_byte(){
    int n;
    char loaded_data[1]; 
    n = recv(this->_socket, loaded_data, 1,0);
   // std::cout << std::hex <<"hex " <<(u_short)loaded_data[0] << std::endl;
    return loaded_data[0];
}

int Transport::read_2bytes(){
    int data;
    data = ((int)0x000000ff) & read_byte();      
    data <<= 8;
    data |= (((int)0x000000ff) & read_byte());
    return data;
}

int Transport::read_4bytes(){
    int data = 0;
    for(int k=0; k<4; k++){
          data <<= 8;
          data |= (((int)0x000000ff) & read_byte());
    }
    return data;
}






void Transport::read_array(std::string *arr){
    int n;
    u_short loaded_data[9];
    int array_len = read_varint();

    (*arr).clear();
    for(int i = 0; i < array_len; i++){
        n = recv(this->_socket, loaded_data, 1,0);  // cteni dat ze socketu
        loaded_data[n] = '\0';
        *arr += loaded_data[0];
    }
}



int Transport::read_header(){
    int hotrod_version = transportFactory.get_hotrod_version();
    if(hotrod_version == VERSION_10){
        return ((Codec10 *) codec)->read_header();
    }else if(hotrod_version == VERSION_11){
        return ((Codec11 *) codec)->read_header();
    }else{
        //return ERROR
    }
}


int Transport::create_connection(){
  struct sockaddr_in sin;
  sin.sin_family = PF_INET;
  sin.sin_port = htons(this->port);
  struct hostent *hptr;
  int ret_socket;

  if((ret_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)  // vytvoreni socketu
  {
    std::cerr << "error socket"<<std::endl;
    return 0;//EXIT_FAILURE;
  }

  if((hptr = gethostbyname(this->host.c_str())) == NULL)  // preklad URL adresy
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
  this->_socket = ret_socket;
  return ret_socket;

    //std::cout << "cconnected"<<std::endl;
}


// void Transport::execute()
// {
//      std::cout << "G e" <<std::endl;
//      AbstractOperation::execute();
// }  