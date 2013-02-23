#include <packetAssembler.h>


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

void PacketAssembler::createBase(u_short op_code){
    struct timeval begin;

    gettimeofday(&begin, NULL);
    long long the_time;
        
    the_time = begin.tv_sec;
    the_time <<= 32;
    the_time += begin.tv_usec;


    packet += (char)0xa0; //HotRod request
    packet += encode_varlong(1);//encode_varlong(the_time); //msg ID

    packet += (char)transporter->hotrod_version; //version 10 or 11
    packet += (char)op_code; //op_code
    packet += (char)0x00; //cache name length
    packet += (char)0x00; //flags
    packet += (char)0x03; //inteligence
    packet += encode_varint(transporter->get_topology_id()); //topology
    packet += (char)0x00; //transaction type

}

int PacketAssembler::createPing(long long *value){
	
	
	createBase(0x17);
										

	transporter->send(packet.c_str(),packet.length(),&state,value);
    return state;									
}

int PacketAssembler::createClear(){
    createBase(0x13);                                   

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;                                 
}

int PacketAssembler::createPut(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
	return createPut(&tmp_key,&tmp_value,lifespan,idle);
}

int PacketAssembler::createPut(const std::string *key, const std::string *value,int lifespan, int idle){
    createBase(0x01);      

                  
    packet += encode_varlong((*key).length()); // key len
    packet += (*key); //key
        	
    packet += (char)lifespan; //Life span
    packet += (char)idle;	//Max idle

    packet += encode_varlong((*value).length()); // key len
    packet += (*value); //key
    transporter->send(packet.c_str(),packet.length(),&state);
    return state;	
}

int PacketAssembler::createPutIfAbsent(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return createPutIfAbsent(&tmp_key,&tmp_value,lifespan,idle);
}

int PacketAssembler::createPutIfAbsent(const std::string *key, std::string *value,int lifespan, int idle){
    createBase(0x05);      

                  
    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key
            
    packet += (char)lifespan; //Life span
    packet += (char)idle; //Max idle


    packet += encode_varlong((*value).length()); // key len
    packet += *value; //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;   
}

int PacketAssembler::createReplace(const char *key,const char *value,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return createReplace(&tmp_key,&tmp_value,lifespan,idle);
}

int PacketAssembler::createReplace(const std::string *key, std::string *value,int lifespan, int idle){
    createBase(0x07);      
                  
    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key
            
    packet += (char)lifespan; //Life span
    packet += (char)idle; //Max idle

    packet += encode_varlong((*value).length()); // key len
    packet += *value; //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;   
}

int PacketAssembler::createReplaceWithVersion(const char *key,const char *value, long long version,int lifespan, int idle){
    std::string tmp_key(key);
    std::string tmp_value(value);
    return createReplaceWithVersion(&tmp_key,&tmp_value,version,lifespan,idle);
}

int PacketAssembler::createReplaceWithVersion(const std::string *key, std::string *value, long long version,int lifespan, int idle){
    createBase(0x09);      

                  
    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key
            
    packet += (char)lifespan; //Life span
    packet += (char)idle; //Max idle
 
    packet += encode_version(version); //version

    packet += encode_varlong((*value).length()); // key len
    packet += *value; //key

    transporter->send(packet.c_str(),packet.length(),&state);
    return state;   
}

int PacketAssembler::createGet(const char *value,const char *key){
    std::string tmp_key(key);
    std::string val;  //zmenit
    createGet(&val,&tmp_key);
    value = (val.c_str());
	return state; // state of this packet
}

int PacketAssembler::createGet(std::string *value, const std::string *key){
    createBase(0x03);      

    packet += encode_varlong((*key).length()); // key len
    packet += (*key); //key

    response resp;
    resp.data = value;

    transporter->send(packet.c_str(),packet.length(),&state,&resp);	 
    return state;     
}


int PacketAssembler::createRemove(const char *key){
    std::string tmp_key(key);
    return createRemove(&tmp_key);
}

int PacketAssembler::createRemove(const std::string *key){
    createBase(0x0b);
          
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

int PacketAssembler::createRemoveWithVersion(const std::string *key, long long version){
    createBase(0x0d);      

    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key

    packet += encode_version(version); //version

    transporter->send(packet.c_str(),packet.length(),&state);   
    return state;       
}

int PacketAssembler::createContainsKey(const std::string *key){
    createBase(0x0f);      

    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key

    transporter->send(packet.c_str(),packet.length(),&state);         
    return state; 
}

int PacketAssembler::createGetWithVersion(const char *value,const char *key,long long *version){
    std::string tmp_key(key);
    std::string val;  //zmenit
    createGetWithVersion(&val,&tmp_key,version);
    value = (val.c_str());
    return state; // state of this packet
}

int PacketAssembler::createGetWithVersion(std::string *value,const std::string *key,long long *version){
    createBase(0x11);      

    packet += encode_varlong((*key).length()); // key len
    packet += *key; //key

    response resp;
    resp.data = value;

    transporter->send(packet.c_str(),packet.length(),&state,version,&resp);  
    return state;   
}

int PacketAssembler::createGetBulk(std::map<std::string,std::string> *bulk, int count){
    createBase(0x19);      

    packet += encode_varint(count);//entry count
    response resp;
    resp.bulk = bulk;


    transporter->send(packet.c_str(),packet.length(),&state,&resp); 
    return state;
}


