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
#include <errno.h>
#include <algorithm>

#include <queue>

#include <packetParser.h>

typedef struct {
    std::string host;
    int port;
    int hash;
    int used;
    int socket;
    int valid;
} server;


class Transporter{
	public:
    
    std::string host, cache_name;
    int port;
    
    std::queue<server> servers;

    std::vector<std::pair<int,server*> > hash_server_bank;
    pthread_mutex_t mutex, mutex_t_id;

    int sock;
    int hotrod_version;
    int topology_id;
    int virtual_nodes_num;
    int max_hash_size;

  
    Transporter(std::string,int);
    void set_topology_id(int id);
    int get_topology_id();
    void set_virtual_nodes_num(int id);
    int get_virtual_nodes_num();
    void set_max_hash_size(int id);
    int get_max_hash_size();
    int create_connection(std::string h,int p);
    int send(const char *packet,int len,int *state,response *resp = NULL);
    int send(const char *packet,int len,int *state,long long *data,response *resp = NULL);

    
    void close_connection(int s);
    void close_servers();
	
    int choose_socket();
	
    
        
	void write_data(const char *data,int len,int s);
    void update_servers(server *s);
    void invalidate_servers();
    void del_invalid_servers();
    void update_servers_bank();
    int reconnect_server(int s);
    void release_server(int s);
    //int connected(int ssocket);
};

#endif


