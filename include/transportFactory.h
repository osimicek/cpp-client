#ifndef TRANSPORTFACTORY_H_INCLUDED
#define TRANSPORTFACTORY_H_INCLUDED
#include "transport.h"
#include "consistentHash10.h"
#include "consistentHash11.h"

#include <iostream>
#include <queue>
#include <algorithm>


#include "constants.h"

class Transport;

class TransportFactory{
    public:
        Transport *first_transport; //smazat


        pthread_mutex_t mutex, mutex_t_id;

        std::queue<Transport*> transports; // predelat na vektor <std::string, Transport>
        int transports_queue_possition;
        //std::vector<std::pair<std::string, Transport> > hostport_transport; //host+port: transport
        std::vector<std::pair<int, Transport*> > hash_transport_bank;
        std::vector<int> hash_vector; // for binary search of index
        int hotrod_version;
        int topology_id;
        int virtual_nodes_num;
        int max_hash_size;
        int num_key_owners;
        void *consistentHash;

        explicit TransportFactory(std::string host, int port, int version);
        void execute();
        void set_topology_id(int id);
        int get_topology_id();
        // void set_hotrod_version(int version);
        int get_hotrod_version();
        void set_virtual_nodes_num(int num);
        int get_virtual_nodes_num();
        void set_max_hash_size(int size);
        int get_max_hash_size();
        void set_num_key_owners(int num);
        int get_num_key_owners();
        Transport *get_transport();
        Transport *get_transport(const std::string *key);
        Transport * get_transport(std::string *host, int port);
        void release_transport(Transport * transport);
        Transport * create_transport(std::string *host, int port);
        void invalidate_transports();
        void del_invalid_transports();
        void print_hash_bank();


};


#endif