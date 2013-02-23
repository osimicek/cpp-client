#ifndef REMOTE_H_INCLUDED
#define REMOTE_H_INCLUDED

#include <string>
#include <iostream>
#include <transporter.h>
#include <packetAssembler.h>
#include <map>
#include <pthread.h>
class RemoteCache;

static void *print_message_function(void *);
typedef struct {
    RemoteCache * RC;
    const std::string *key;
    const std::string *value;
    int lifespan;
    int idle;
} thread_args;


class RemoteCache{
    
    Transporter *transporter;
  public:
    RemoteCache(std::string,int);
    RemoteCache(std::string);
    RemoteCache(void);
    void stats(void);
    int ping();
    int ping(long long *value);
    int clear();
    int put(const std::string *key,const std::string *value,int lifespan=0, int idle=0);
    int put(const char *key,const char *value,int lifespan=0, int idle=0);
    int putAll(std::map<std::string,std::string> *data,int lifespan=0, int idle=0);
    int putAllAsync(std::map<std::string,std::string> *data,int lifespan=0, int idle=0);
    int putIfAbsent(const std::string *key,std::string *value,int lifespan=0, int idle=0);
    int putIfAbsent(const char *key,const char *value,int lifespan=0, int idle=0);
    int replace(const std::string *key,std::string *value,int lifespan=0, int idle=0);
    int replace(const char *key,const char *value,int lifespan=0, int idle=0);
    int replaceWithVersion(const std::string *key,std::string *value,long long version,int lifespan=0, int idle=0);
    int replaceWithVersion(const char *key,const char *value,long long version,int lifespan=0, int idle=0);
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