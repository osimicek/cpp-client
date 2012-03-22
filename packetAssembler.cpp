#include "packetAssembler.h"


PacketAssembler::PacketAssembler(Transporter *t):transporter(t)
{
        
}



std::string PacketAssembler::encode_version(long long value){
    std::string result;

    for(int i=0;i<8;i++){
        char bits = value>>56 & 0xff;
        value <<= 8;
        result += (char)(bits);
    }
    return result;
}

std::string PacketAssembler::encode_varint(int value){
	std::string result;
	char bits = value & 0x7f;
	value >>= 7;
	
	while (value){
	    result += (char)(0x80|bits);
	    
	    bits = value & 0x7f;
	    value >>= 7;
	}
	
	result += bits;
	return result;
}

std::string PacketAssembler::encode_varlong(long long value){
	std::string result;
	char bits = value & 0x7f;
	value >>= 7;
	
	while (value){
	    result += (char)(0x80|bits);
	    
	    bits = value & 0x7f;
	    value >>= 7;
	}
	
	result += bits;
	return result;
}

void PacketAssembler::createBase(long long msg_id, u_short op_code){
    
    packet += (char)0xa0; //HotRod request
    packet += encode_varlong(msg_id); //msg ID

    packet += (char)0x0a; //version 10 or 11
    packet += (char)op_code; //op_code
    packet += (char)0x00; //cache name length
    packet += (char)0x00; //flags
    packet += (char)0x01; //inteligence
    packet += (char)0x00; //topology
    packet += (char)0x00; //transaction type

}

int PacketAssembler::createPing(long long *value){
	struct timeval begin;

    gettimeofday(&begin, NULL);
    long long the_time;
        
    the_time = begin.tv_sec;
    the_time <<= 32;
    the_time += begin.tv_usec;
	
	createBase(the_time,0x17);
										

	transporter->send(packet.c_str(),packet.length(),&state,value);
    return state;									
}

int PacketAssembler::createClear(){
    createBase(0x09,0x13);                                   

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;                                 
}

int PacketAssembler::createPut(const char *key,const char *value){
    std::string tmp_key(key);
    std::string tmp_value(value);
	return createPut(&tmp_key,&tmp_value);
}

int PacketAssembler::createPut(const std::string *key, const std::string *value){
    createBase(0x09,0x01);      

                  
    packet += encode_varlong((*key).length()); // key len
    packet += (*key); //key
        	
    packet += (char)0x00; //Life span
    packet += (char)0x00;	//Max idle

    packet += encode_varlong((*value).length()); // key len
    packet += (*value); //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;	
}

int PacketAssembler::createPutIfAbsent(const char *key,const char *value){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return createPutIfAbsent(&tmp_key,&tmp_value);
}

int PacketAssembler::createPutIfAbsent(std::string *key, std::string *value){
    std::string  *s_key, *s_value;
    s_key = key;
    s_value = value;

    createBase(0x09,0x05);      

                  
    packet += encode_varlong((*s_key).length()); // key len
    packet += (*s_key); //key
            
    packet += (char)0x00; //Life span
    packet += (char)0x00; //Max idle


    packet += encode_varlong((*s_value).length()); // key len
    packet += (*s_value); //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;   
}

int PacketAssembler::createReplace(const char *key,const char *value){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return createReplace(&tmp_key,&tmp_value);
}

int PacketAssembler::createReplace(std::string *key, std::string *value){
    std::string  *s_key, *s_value;
    s_key = key;
    s_value = value;

    createBase(0x09,0x07);      
                  
    packet += encode_varlong((*s_key).length()); // key len
    packet += (*s_key); //key
            
    packet += (char)0x00; //Life span
    packet += (char)0x00; //Max idle

    packet += encode_varlong((*s_value).length()); // key len
    packet += (*s_value); //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;   
}

int PacketAssembler::createReplaceWithVersion(const char *key,const char *value, long long version){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return createReplaceWithVersion(&tmp_key,&tmp_value,version);
}

int PacketAssembler::createReplaceWithVersion(std::string *key, std::string *value, long long version){
    std::string  *s_key, *s_value;
    s_key = key;
    s_value = value;

    createBase(0x09,0x09);      

                  
    packet += encode_varlong((*s_key).length()); // key len
    packet += (*s_key); //key
            
    packet += (char)0x00; //Life span
    packet += (char)0x00; //Max idle
 
    packet += encode_version(version); //version

    packet += encode_varlong((*s_value).length()); // key len
    packet += (*s_value); //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;   
}

const char * PacketAssembler::createGet(const char *key){
    std::string tmp_key(key);
	return (*createGet(&tmp_key)).c_str();
}

std::string *PacketAssembler::createGet(std::string *key){
    createBase(0x09,0x03);      

    packet += encode_varlong((*key).length()); // key len
    packet += (*key); //key

    return (transporter->send(packet.c_str(),packet.length(),&state).data);	      
}


int PacketAssembler::createRemove(const char *key){
    std::string tmp_key(key);
    return createRemove(&tmp_key);
}

int PacketAssembler::createRemove(std::string *key){
    createBase(0x09,0x0b);
          
    packet += encode_varlong((*key).length()); // key len
    packet += (*key); //key

    transporter->send(packet.c_str(),packet.length(),&state);   
    return state;       
}

int PacketAssembler::createContainsKey(const char *key){
    std::string tmp_key(key);
    return createContainsKey(&tmp_key);
}

int PacketAssembler::createRemoveWithVersion(const char *key,long long version){
    std::string tmp_key(key);
    return createRemoveWithVersion(&tmp_key,version);
}

int PacketAssembler::createRemoveWithVersion(std::string *key, long long version){
    createBase(0x09,0x0d);      

    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key

    packet += encode_version(version); //version

    transporter->send(packet.c_str(),packet.length(),&state);   
    return state;       
}

int PacketAssembler::createContainsKey(std::string *key){
    createBase(0x09,0x0f);      

    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key

    transporter->send(packet.c_str(),packet.length(),&state);         
    return state; 
}

const char * PacketAssembler::createGetWithVersion(const char *key,long long *version){
    std::string tmp_key(key);
    return (*createGetWithVersion(&tmp_key,version)).c_str();
}

std::string *PacketAssembler::createGetWithVersion(std::string *key,long long *version){
    createBase(0x09,0x11);      

    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key

    return (transporter->send(packet.c_str(),packet.length(),&state,version).data);     
}

std::map<std::string,std::string> *PacketAssembler::createGetBulk(int count){
    createBase(0x09,0x19);      

    packet += encode_varint(count);//entry count

    return  (transporter->send(packet.c_str(),packet.length(),&state).bulk); 
}


