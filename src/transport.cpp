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
        codec = new Codec12(*this);
    }   
} 
Transport::~Transport(){
    delete codec;
    close_connection();
}

/**
* Encodes int to varInt and writes it to the packet.
*
* @param value value to use
*/
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

/**
* Encode long long to varLong and writes it to the packet.
*
* @param value value to use
*/
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
/**
* Writes char to the packet.
*
* @param value value to use
*/
void Transport::write_char(char value){
    packet += value; 
}

/**
* Writes byte (short) to the packet.
*
* @param value value to use
*/
void Transport::write_byte(short value){
    packet += (char)value;
}
/**
* Writes long long to the packet.
*
* @param value value to use
*/
void Transport::write_8bytes(long long value){
    for(int k=0; k<8; k++){
        write_char((char) (((long long)0xff00000000000000 &  value ) >> 56 ));
        value <<= 8;
    }
}

/**
* Writes header of request to the packet.
*
* @param op_code code of operation
* @param cache_name name of the cache to comunicate with
* @param flags flags to use
*/
void Transport::write_header(char op_code, const std::string *cache_name, int flags){
    int hotrod_version = transportFactory.get_hotrod_version();
    codec->write_header(op_code, cache_name, flags);
}

/**
* Encodes and writes string array to the packet.
*
* @param arr array to use
*/
void Transport::write_array(const std::string *arr){
    write_varlong((*arr).length()); // array len
    packet += (*arr); //key
}

/**
* Sends request to the server.
*
* @return status of operation
*/
int Transport::flush(){
    // sends data to server
    int status = 0;
    if(this->_socket == 0){
        create_connection();
    }
    if(this->_socket == 0){
        return FAILED_TO_SEND_STATUS;
    }
    for(int i=0; i<2; i++){ //when keeping dead socket
        if(write(this->_socket, this->packet.c_str(), this->packet.length()) < 0)  // odeslani pozadavku na server 
        {
            
            status = create_connection();
            if(status == 0){
                return FAILED_TO_SEND_STATUS;
            }
        }else{
            break;
        }
    }

    return NO_ERROR_STATUS;
}

/**
* Reads and decodes varInt from response.
*
* @return decoded varInt
*/
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
        if (shift >= 32) {}
          //raise DecodeError("Too many bytes when decoding varint.")
    }
}

/**
* Reads and decodes varLong from response.
*
* @return decoded varLong
*/
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
        if (shift >= 64) {}
          //raise DecodeError("Too many bytes when decoding varint.")
    }

}
/**
* Reads byte from response.
*
* @return value of the byte
*/
int Transport::read_byte(){
    int n;
    char loaded_data[1]; 
    n = recv(this->_socket, loaded_data, 1,0);
    return ((int)0x000000ff) & loaded_data[0];
}
/**
* Reads 2bytes from response.
*
* @return value
*/
int Transport::read_2bytes(){
    int data;
    data = ((int)0x000000ff) & read_byte();      
    data <<= 8;
    data |= (((int)0x000000ff) & read_byte());
    return data;
}
/**
* Reads 4bytes from response.
*
* @return value
*/
int Transport::read_4bytes(){
    int data = 0;
    for(int k=0; k<4; k++){
          data <<= 8;
          data |= (((int)0x000000ff) & read_byte());
    }
    return data;
}
/**
* Reads 8bytes from response.
*
* @return value
*/
long long Transport::read_8bytes(){
    long long data = 0;
    for(int k=0; k<8; k++){
          data <<= 8;
          data |= (((long long)0x00000000000000ff) & read_byte());
    }
    return data;
}





/**
* Reads and decodes string array from the response.
*
* @param arr pointer to return array
*/
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

/**
    * Reads a response header from the transport and returns the status
    * of the response.
*/
int Transport::read_header(){
    return codec->read_header();
}

//creates connection to the server
int Transport::create_connection(){
  struct sockaddr_in sin;
  sin.sin_family = PF_INET;
  sin.sin_port = htons(this->port);
  struct hostent *hptr;
  int ret_socket;

  if((ret_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)  // vytvoreni socketu
  {
    if(DEBUG) std::cerr << "error socket"<<std::endl;
    return 0;//EXIT_FAILURE;
  }

  if((hptr = gethostbyname(this->host.c_str())) == NULL)  // preklad URL adresy
  {
    if(DEBUG) std::cerr << "error host"<<std::endl;
    return 0;//EXIT_FAILURE;
  }
  
  memcpy(&sin.sin_addr, hptr->h_addr_list[0], hptr->h_length);

  if(connect(ret_socket, (struct sockaddr*) &sin, sizeof(sin)) < 0)  // navazani spojeni se serverem
  {
    if(DEBUG) std::cerr << "error connect"<<std::endl;
    return 0;//EXIT_FAILURE;
  }
  this->_socket = ret_socket;
  return ret_socket;


}

/**
    * Closes connection to the server.
    * @return status
*/
int Transport::close_connection(){
    return close(_socket);
}


