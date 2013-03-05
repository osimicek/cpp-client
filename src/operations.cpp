#include "operations.h"

Metadata::Metadata(){
    flag = -1;
    lifespan = -1;
    maxidle = -1;
    version = -1;
    created = -1;
    lastused = -1;
}

AbstractOperation::AbstractOperation(TransportFactory &tF):transportFactory(tF)
{
    this->flags = 0x00;
    this->key = NULL;
}    
int AbstractOperation::execute()
{
    int state;
    for(int i=0; i<3; i++){
        if(key != NULL){
            transport = transportFactory.get_transport(key);
        }else{
            transport = transportFactory.get_transport();
        }
        if(transport == NULL){
            state =  FAILED_TO_CHOOSE_TRANSPORT;
        }else{
            state = execute_operation();
            transportFactory.release_transport(transport);
            if(status != FAILED_TO_SEND) break;
        }
    }
    return state;
} 

int AbstractOperation::execute_operation(){}  

void AbstractOperation::return_possible_prev_value(std::string *prev_value)
{
    if(this->flags & 0x0001){
        transport->read_array(prev_value);
    }
} 




GetOperation::GetOperation(std::string *value, const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->value = value;
     this->key = key;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int GetOperation::execute_operation()
{  

    transport->write_header(GET_REQUEST, cache_name, flags);
    transport->write_array(key);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }

    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        transport->read_array(value);
        if(DEBUG)std::cout << *value << std::endl;
    }
    return status;
    
}  

GetWithVersionOperation::GetWithVersionOperation(std::string *value, const std::string *key, long long *version, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->version = version;
     this->value = value;
     this->key = key;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int GetWithVersionOperation::execute_operation()
{  

    transport->write_header(GET_WITH_VERSION, cache_name, flags);
    transport->write_array(key);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }

    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        *version = transport->read_8bytes();
        transport->read_array(value);
        if(DEBUG)std::cout << *value << std::endl;
    }
    return status;
    
}  


GetWithMetadataOperation::GetWithMetadataOperation(std::string *value, Metadata *meta, const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->value = value;
     this->key = key;
     this->meta = meta;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int GetWithMetadataOperation::execute_operation()
{  

    transport->write_header(GET_WITH_METADATA, cache_name, flags);
    transport->write_array(key);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    char flag; 
    int lifespan, maxidle;
    long long version, created, lastused;
    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        flag = transport->read_byte();
        if(DEBUG)std::cout << "* flag " <<(int) flag<<" - "<<(int) flags << std::endl;
        meta->flag = flag;
        
        if(!(flag & INFINITE_LIFESPAN)){
            created = transport->read_8bytes();
            if(DEBUG)std::cout << "* created " << created << std::endl;
            meta->created = created;
            lifespan = transport->read_varint();
            if(DEBUG)std::cout << "* lifespan " << lifespan << std::endl;
            meta->lifespan = lifespan;
        }

        if(!(flag & INFINITE_MAXIDLE)){
            lastused = transport->read_8bytes();
            if(DEBUG)std::cout << "* lastused " << lastused << std::endl;
            meta->lastused = lastused;
            maxidle = transport->read_varint();
            if(DEBUG)std::cout << "* maxidle " << maxidle << std::endl;
            meta->maxidle = maxidle;
        }
        version = transport->read_8bytes();
        if(DEBUG)std::cout << "* version " << version << std::endl;
        meta->version = version;
        transport->read_array(value);
        if(DEBUG)std::cout << *value << std::endl;
    }
    return status;
    
}  

RemoveOperation::RemoveOperation(std::string *prev_value, const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{   
    this->prev_value = prev_value;
    this->key = key; 
    this->cache_name = cache_name;
    this->flags = flags;
}   

int RemoveOperation::execute_operation()
{  

    transport->write_header(REMOVE_REQUEST, cache_name, flags);
    transport->write_array(key);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }

    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        return_possible_prev_value(prev_value);
        if(DEBUG)std::cout << *prev_value << std::endl;
    }
    return status; 
}  

RemoveIfUnmodifiedOperation::RemoveIfUnmodifiedOperation(std::string *prev_value, const std::string *key, long long version, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{   
    
    this->version = version;
    this->prev_value = prev_value;
    this->key = key; 
    this->cache_name = cache_name;
    this->flags = flags;
}   

int RemoveIfUnmodifiedOperation::execute_operation()
{  

    transport->write_header(REMOVE_IF_UNMODIFIED_REQUEST, cache_name, flags);
    transport->write_array(key);
    transport->write_8bytes(version);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }

    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        return_possible_prev_value(prev_value);
        if(DEBUG)std::cout << *prev_value << std::endl;
    }
    return status; 
} 

ContainsKeyOperation::ContainsKeyOperation(const std::string *key, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->key = key;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int ContainsKeyOperation::execute_operation()
{  

    transport->write_header(REMOVE_REQUEST, cache_name, flags);
    transport->write_array(key);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }

    status = transport->read_header();
    return status;
    
} 

PutBasedOperation::PutBasedOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle):AbstractOperation(tF)
{    
    this->value = value; 
    this->key = key; 
    this->prev_value = prev_value;
    this->lifespan = lifespan;
    this->idle = idle;
    this->cache_name = cache_name;
    this->flags = flags;
}   

int PutBasedOperation::execute_operation(int op_code){
    transport->write_header(op_code, cache_name, flags);
    transport->write_array(key);
    transport->write_varint(this->lifespan); //lifespan
    transport->write_varint(this->idle); //idle
    transport->write_array(value);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    
    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        return_possible_prev_value(prev_value);
        if(DEBUG)std::cout << *prev_value << std::endl;
    }
    return status;
}  

PutOperation::PutOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle):PutBasedOperation(value, key, prev_value, tF, cache_name, flags, lifespan, idle)
{ }   

int PutOperation::execute_operation()
{
    PutBasedOperation::execute_operation(PUT_REQUEST);
}  

PutIfAbsentOperation::PutIfAbsentOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle):PutBasedOperation(value, key, prev_value, tF, cache_name, flags, lifespan, idle)
{ }   

int PutIfAbsentOperation::execute_operation()
{
    PutBasedOperation::execute_operation(PUT_IF_ABSENT_REQUEST);
}  

ReplaceOperation::ReplaceOperation(const std::string *value, const std::string *key, std::string *prev_value, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle):PutBasedOperation(value, key, prev_value, tF, cache_name, flags, lifespan, idle)
{ }   

int ReplaceOperation::execute_operation()
{
    PutBasedOperation::execute_operation(REPLACE_REQUEST);
}  

ReplaceIfUnmodifiedOperation::ReplaceIfUnmodifiedOperation(const std::string *value, const std::string *key, std::string *prev_value, long long version, TransportFactory &tF, const std::string *cache_name, int flags, int lifespan, int idle):AbstractOperation(tF)
{    
    this->version = version; 
    this->value = value; 
    this->key = key; 
    this->prev_value = prev_value;
    this->lifespan = lifespan;
    this->idle = idle;
    this->cache_name = cache_name;
    this->flags = flags;
}   

int ReplaceIfUnmodifiedOperation::execute_operation()
{
    transport->write_header(REPLACE_IF_UNMODIFIED_REQUEST, cache_name, flags);
    transport->write_array(key);
    transport->write_varint(this->lifespan); //lifespan
    transport->write_varint(this->idle); //idle
    transport->write_8bytes(version);
    transport->write_array(value);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        return_possible_prev_value(prev_value);
        if(DEBUG)std::cout << *prev_value << std::endl;
    }
    return status;
}

GetBulkOperation::GetBulkOperation(std::map<std::string,std::string> *bulk, int count, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->bulk = bulk;
     this->count = count;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int GetBulkOperation::execute_operation()
{
    transport->write_header(BULK_GET_REQUEST, cache_name, flags);
    transport->write_varint(count);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    
    status = transport->read_header();
    std::string key, value;
    if(status == NO_ERROR_STATUS){
        while(transport->read_byte()){
            transport->read_array(&key);
            transport->read_array(&value);
            (*bulk)[key] = value;
        }
    }
    return status;
}  

BulkKeysGetOperation::BulkKeysGetOperation(std::vector<std::string> *keys, int scope, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->keys = keys;
     this->scope = scope;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int BulkKeysGetOperation::execute_operation()
{
    transport->write_header(BULK_GET_KEYS_REQUEST, cache_name, flags);
    transport->write_varint(scope);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    
    status = transport->read_header();
    std::string key;
    if(status == NO_ERROR_STATUS){
        while(transport->read_byte()){
            transport->read_array(&key);
            keys->push_back(key);
        }
    }
    return status;
}  


ClearOperation::ClearOperation(TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->cache_name = cache_name;
     this->flags = flags;
}   

int ClearOperation::execute_operation()
{
    transport->write_header(CLEAR_REQUEST, cache_name, flags);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    
    status = transport->read_header();
    return status;
}  

PingOperation::PingOperation(TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->cache_name = cache_name;
     this->flags = flags;
}   

int PingOperation::execute_operation()
{
    transport->write_header(PING_REQUEST, cache_name, flags);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
    
    status = transport->read_header();
    return status;
} 


StatsOperation::StatsOperation(std::map<std::string,std::string> *stats, TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->stats = stats;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int StatsOperation::execute_operation()
{
    std::cout << "ok" <<std::endl;
    transport->write_header(STATS_REQUEST, cache_name, flags);
    if(status = transport->flush() != NO_ERROR_STATUS){
        return status;
    }
        std::cout << "ok"<< status <<std::endl;
    status = transport->read_header();
     std::cout << "ok" <<std::endl;
    std::string name, value;

    if(status == NO_ERROR_STATUS){
        int count = transport->read_varint();
        std::cout << count <<std::endl;
        for(int i=0; i<count; i++){
            transport->read_array(&name);
            transport->read_array(&value);
            (*stats)[name] = value;
        }
    }
    return status;
} 