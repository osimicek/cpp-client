#ifndef ASSEMBL_H_INCLUDED
#define ASSEMBL_H_INCLUDED
#include "transporter.h"
#include "packetParser.h"
#include <sys/time.h>

class PacketAssembler{
	public:
    Transporter *transporter;
    std::string packet;
    int state;  
  
    void createBase(long long msg_id, u_short op_code);
  	std::string encode_varint(int value);
  	std::string encode_varlong(long long value);
    std::string encode_version(long long value);
    PacketAssembler(Transporter *t);
    int createPing(long long *value);
    int createClear();
    int createPut(const char *key, const char *value,int lifespan, int idle);
    int createPut(const std::string *key, const std::string *value,int lifespan, int idle);
    int createPutIfAbsent(const char *key, const char *value,int lifespan, int idle);
    int createPutIfAbsent(std::string *key, std::string *value,int lifespan, int idle);
    int createReplace(const char *key, const char *value,int lifespan, int idle);
    int createReplace(std::string *key, std::string *value,int lifespan, int idle);
    int createReplaceWithVersion(const char *key, const char *value, long long version,int lifespan, int idle);
    int createReplaceWithVersion(std::string *key, std::string *value, long long version,int lifespan, int idle);
    int createRemove(const char *key);
    int createRemove(std::string *key);
    int createRemoveWithVersion(const char *key, long long version);
    int createRemoveWithVersion(std::string *key, long long version);
    int createContainsKey(const char *key);
    int createContainsKey(std::string *key);
    int createGetWithVersion(const char *value , const char *key,long long *version);
    int createGetWithVersion(std::string *value, std::string *key,long long *version);
    int createGet(std::string *value, std::string *key);
    int createGet(const char * value, const char *key);
    int createGetBulk(std::map<std::string,std::string>  *bulk, int count);
    
};

#endif