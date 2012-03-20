#include "transporter.h"
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
    int createPut(const char *key, const char *value);
    int createPut(std::string key, std::string value);
    int createPutIfAbsent(const char *key, const char *value);
    int createPutIfAbsent(std::string key, std::string value);
    int createReplace(const char *key, const char *value);
    int createReplace(std::string key, std::string value);
    int createReplaceWithVersion(const char *key, const char *value, long long version);
    int createReplaceWithVersion(std::string key, std::string value, long long version);
    int createRemove(const char *key);
    int createRemove(std::string key);
    int createRemoveWithVersion(const char *key, long long version);
    int createRemoveWithVersion(std::string key, long long version);
    int createContainsKey(const char *key);
    int createContainsKey(std::string key);
    const char * createGetWithVersion(const char *key,long long *version);
    std::string createGetWithVersion(std::string key,long long *version);
    std::string createGet(std::string key);
    const char * createGet(const char *key);
    std::map<std::string,std::string>  createGetBulk(int count);
    
};