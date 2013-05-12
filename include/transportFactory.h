#ifndef TRANSPORTFACTORY_H_INCLUDED
#define TRANSPORTFACTORY_H_INCLUDED

#include "transport.h"
#include "consistentHash.h"

#include <murmur/MurmurHash2.h>
#include <murmur/MurmurHash3.h>

#include <iostream>
#include <queue>
#include <algorithm>


#include "constants.h"

class ConsistentHash;
/**
 * transport factory for building and managing Transport objects
 * and topology of the cluster.
 *
 * @author ondrejsimicek@seznam.cz
 */

class TransportFactory{
    #ifndef NOT_THREAD_SAFE
        #define LOCK() pthread_mutex_lock (&mutex);
        #define LOCK_ID() pthread_mutex_lock (&mutex_tf_id);
        #define UNLOCK() pthread_mutex_unlock (&mutex);
        #define UNLOCK_ID() pthread_mutex_unlock (&mutex_tf_id);
    #else
        #define LOCK();
        #define LOCK_ID() ;
        #define UNLOCK() ;
        #define UNLOCK_ID();
    #endif
    private:
        int message_id;
        int hotrod_version;
        int topology_id;
        int virtual_nodes_num;
        int max_hash_size;
        int num_key_owners;
        int hash_ver;
        char intelligence;
    public:
        int transports_in_use;
        pthread_mutex_t mutex, mutex_tf_id;
        std::queue<Transport*> transports; 
        std::vector<std::pair<int, Transport*> > hash_transport_bank;
        std::vector<int> hash_vector; // for binary search of index
        
        ConsistentHash *consistentHash;

        explicit TransportFactory(std::string host, int port, int version, char intelligence);
        ~TransportFactory();
        int get_and_inc_message_id();
        char get_intelligence();
        void set_topology_id(int id);
        int get_topology_id();
        // void set_hotrod_version(int version);
        int get_hotrod_version();
        void set_virtual_nodes_num(int num);
        int get_virtual_nodes_num();
        void set_max_hash_size(int size);
        int get_max_hash_size();
        void set_key_owners_num(int num);
        int get_key_owners_num();
        void set_hash_ver(int ver);
        int get_hash_ver();
        int get_hash(const char* key, int length);
        Transport *get_transport();
        Transport *get_transport(const std::string *key);
        Transport * get_transport(std::string *host, int port, int hash);
        void release_transport(Transport * transport);
        Transport * create_transport(std::string *host, int port, int hash);
        void invalidate_transports();
        void del_invalid_transports();
        //void print_hash_bank();
        void close_servers();

};


#endif