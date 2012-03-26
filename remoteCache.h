#include <string>
#include <iostream>
#include "transporter.h"
#include "packetAssembler.h"
#include <map>

class RemoteCache{
    
    Transporter *transporter;
  public:
    RemoteCache(std::string,std::string);
    RemoteCache(std::string);
    RemoteCache(void);
    void stats(void);
    int ping();
    int ping(long long *value);
    int clear();
    int put(const std::string *key,const std::string *value,int lifespan=0, int idle=0);
    int put(const char *key,const char *value,int lifespan=0, int idle=0);
    int putAll(std::map<std::string,std::string> *data,int lifespan=0, int idle=0);
    int putIfAbsent(std::string *key,std::string *value,int lifespan=0, int idle=0);
    int putIfAbsent(const char *key,const char *value,int lifespan=0, int idle=0);
    int replace(std::string *key,std::string *value,int lifespan=0, int idle=0);
    int replace(const char *key,const char *value,int lifespan=0, int idle=0);
    int replaceWithVersion(std::string *key,std::string *value,long long version,int lifespan=0, int idle=0);
    int replaceWithVersion(const char *key,const char *value,long long version,int lifespan=0, int idle=0);
    int remove(const char *key);
    int remove(std::string *key);
    int removeWithVersion(const char *key, long long version);
    int removeWithVersion(std::string *key, long long version);
    int containsKey(const char *key);
    int containsKey(std::string *key);
    const char* getWithVersion(const char *key,long long *version);
    std::string *getWithVersion(std::string *key,long long *version);
    std::string *get(std::string *key);

    const char* get(const char *key);

    std::map<std::string,std::string> *getBulk();
    std::map<std::string,std::string> *getBulk(int count);
};