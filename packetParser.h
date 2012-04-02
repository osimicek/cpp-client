#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED


#include <iostream>
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
#include <map>

#include <sys/time.h>//time for ping



class Transporter;

typedef struct {
    union {
        std::string *data;
        std::map<std::string, std::string>*bulk;
    };
} response;

class PacketParser{
	public:
    Transporter *transporter;
    int sock;
  
    PacketParser(int s);
    int read_data(int *state,long long *data, response *resp);
    
    private:
    int decode_varint();
	long long decode_varlong(); 
};

#endif