#ifndef REMOTE2_H_INCLUDED
#define REMOTE2_H_INCLUDED

#include <string>
#include <iostream>
#include <transportFactory.h>
#include <operations.h>
#include <map>
#include <pthread.h>
class RemoteCache;

static void *print_message_function(void *);
// typedef struct {
//     RemoteCache * RC;
//     const std::string *key;
//     const std::string *value;
//     int lifespan;
//     int maxidle;
// } thread_args;



class RemoteCacheConfig{
    public:
        int version;
        int lifespan;
        int maxidle;
        int flags;
        std::string cache_name;
        std::string host;
        int port;
        RemoteCacheConfig();

};

class RemoteCache{
  private:
    TransportFactory *transportFactory;
    int lifespan;
    int maxidle;
    int flags;
    std::string cache_name;
  public:
    RemoteCache(RemoteCacheConfig* remote_cache_config);
    RemoteCache(std::string host, int port);
    RemoteCache(std::string host);
    RemoteCache(void);
    void init(RemoteCacheConfig* remote_cache_config);
    int stats(std::map<std::string,std::string> *stats);
    int ping();
    int clear();
    int put(const std::string *key,const std::string *value,int lifespan=-1, int maxidle=-1);
    int put(const char *key,const char *value,int lifespan=-1, int maxidle=-1);
    int putAll(std::map<std::string,std::string> *data,int lifespan=-1, int maxidle=-1);
    int putAllAsync(std::map<std::string,std::string> *data,int lifespan=-1, int maxidle=-1);
    int putIfAbsent(const std::string *key,std::string *value,int lifespan=-1, int maxidle=-1);
    int putIfAbsent(const char *key,const char *value,int lifespan=-1, int maxidle=-1);
    int replace(const std::string *key,std::string *value,int lifespan=-1, int maxidle=-1);
    int replace(const char *key,const char *value,int lifespan=-1, int maxidle=-1);
    int replaceWithVersion(const std::string *key,const std::string *value,long long version,int lifespan=-1, int maxidle=-1);
    int replaceWithVersion(const char *key,const char *value,long long version,int lifespan=-1, int maxidle=-1);
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

    int getWithMetadata(std::string * value, Metadata *meta, const std::string *key);

    int getBulk(std::map<std::string,std::string> *bulk);
    int getBulk(std::map<std::string,std::string> *bulk, int count);

    int keySet(std::vector<std::string> *keys,int scope=0);

    void print_servers();
    void close();
};

#endif