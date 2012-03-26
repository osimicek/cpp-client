#ifndef TRANS_H_INCLUDED
#define TRANS_H_INCLUDED

#include <string>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "error.h"
#include <stdio.h>


#include "packetParser.h"




class Transporter{
	public:
    
    std::string host, port, cache_name;
    


    int sock;

  
    Transporter(std::string,std::string);
    void create_connection();
    response send(const char *packet,int len,int *state);
    response send(const char *packet,int len,int *state,long long *data);

    
    void close_connection();
private:
	
    int choose_socket();
	
    
        
	void write_data(const char *data,int len);
};

#endif
