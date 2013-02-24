#ifndef REMOTE2_H_INCLUDED
#define REMOTE2_H_INCLUDED

#include <string>
#include <iostream>
#include <transportFactory.h>
#include <operations.h>
#include <map>
#include <pthread.h>
class RemoteCache2;

static void *print_message_function(void *);
// typedef struct {
//     RemoteCache2 * RC;
//     const std::string *key;
//     const std::string *value;
//     int lifespan;
//     int idle;
// } thread_args;



class RemoteCacheConfig{
    public:
        int version;
        int lifespan;
        int idle;
        int flags;
        std::string cache_name;
        std::string host;
        int port;
        RemoteCacheConfig();

};

class RemoteCache2{
  private:
    TransportFactory *transportFactory;
    int lifespan;
    int idle;
    int flags;
    std::string cache_name;
  public:
    RemoteCache2(RemoteCacheConfig* remote_cache_config);
    RemoteCache2(std::string host, int port);
    RemoteCache2(std::string host);
    RemoteCache2(void);
    void init(RemoteCacheConfig* remote_cache_config);
    void stats(void);
    int ping();
    int ping(long long *value);
    int clear();
    int put(const std::string *key,const std::string *value,int lifespan=-1, int idle=-1);
    int put(const char *key,const char *value,int lifespan=-1, int idle=-1);
    int putAll(std::map<std::string,std::string> *data,int lifespan=-1, int idle=-1);
    int putAllAsync(std::map<std::string,std::string> *data,int lifespan=-1, int idle=-1);
    int putIfAbsent(const std::string *key,std::string *value,int lifespan=-1, int idle=-1);
    int putIfAbsent(const char *key,const char *value,int lifespan=-1, int idle=-1);
    int replace(const std::string *key,std::string *value,int lifespan=-1, int idle=-1);
    int replace(const char *key,const char *value,int lifespan=-1, int idle=-1);
    int replaceWithVersion(const std::string *key,std::string *value,long long version,int lifespan=-1, int idle=-1);
    int replaceWithVersion(const char *key,const char *value,long long version,int lifespan=-1, int idle=-1);
    int remove(const char *key);
    int remove(const std::string *key);
    int removeWithVersion(const char *key, long long version);
    int removeWithVersion(const std::string *key, long long version);
    int containsKey(const char *key);
    int containsKey(const std::string *key);
    int getWithVersion(const char* value, const char *key,long long *version);
    int getWithVersion(std::string *value, const std::string *key,long long *version);
    int get(std::string * value, const std::string *key);

    int get(const char* value, const char *key);

    int getBulk(std::map<std::string,std::string> *bulk);
    int getBulk(std::map<std::string,std::string> *bulk, int count);


    void print_servers();
    void close();
};

#endif