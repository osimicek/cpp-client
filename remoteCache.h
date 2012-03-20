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
    void print_info(void);
    int ping();
    int ping(long long *value);
    int clear();
    int put(std::string key,std::string value);
    int put(const char *key,const char *value);
    int putIfAbsent(std::string key,std::string value);
    int putIfAbsent(const char *key,const char *value);
    int replace(std::string key,std::string value);
    int replace(const char *key,const char *value);
    int replaceWithVersion(std::string key,std::string value,long long version);
    int replaceWithVersion(const char *key,const char *value,long long version);
    int remove(const char *key);
    int remove(std::string key);
    int removeWithVersion(const char *key, long long version);
    int removeWithVersion(std::string key, long long version);
    int containsKey(const char *key);
    int containsKey(std::string key);
    const char* getWithVersion(const char *key,long long *version);
    std::string getWithVersion(std::string key,long long *version);
    std::string get(std::string key);

    const char* get(const char *key);

    std::map<std::string,std::string> getBulk();
    std::map<std::string,std::string> getBulk(int count);
};