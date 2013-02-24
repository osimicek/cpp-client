#include "operations.h"


AbstractOperation::AbstractOperation(TransportFactory &tF):transportFactory(tF)
{
}    
int AbstractOperation::execute()
{
     return execute_operation();
} 

int AbstractOperation::execute_operation()
{
}  



GetOperation::GetOperation(std::string *value, TransportFactory &tF, const std::string *key, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->key = key;
     this->value = value;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int GetOperation::execute_operation()
{   
    transport = transportFactory.get_transport(key);
    if(transport == NULL) return FAILED_TO_CHOOSE_TRANSPORT;

    transport->write_header(GET_REQUEST, cache_name, flags);
    transport->write_array(key);
    if(status = transport->flush() != NO_ERROR_STATUS){
        transportFactory.release_transport(transport);
        return status;
    }

    if(status = transport->read_header() == NO_ERROR_STATUS){
        transport->read_array(value);
        if(DEBUG)std::cout << *value << std::endl;
    }
    transportFactory.release_transport(transport);
    return status;
    
}  

PutOperation::PutOperation(const std::string *value, TransportFactory &tF, const std::string *key, const std::string *cache_name, int flags, int lifespan, int idle):AbstractOperation(tF)
{    
     this->key = key;
     this->value = value;
     this->lifespan = lifespan;
     this->idle = idle;
     this->cache_name = cache_name;
     this->flags = flags;
}   

int PutOperation::execute_operation()
{
    transport = transportFactory.get_transport(key);
    if(transport == NULL) return FAILED_TO_CHOOSE_TRANSPORT;
    transport->write_header(PUT_REQUEST, cache_name, flags);
    transport->write_array(key);
    transport->write_varint(this->lifespan); //lifespan
    transport->write_varint(this->idle); //idle
    transport->write_array(value);
    if(status = transport->flush() != NO_ERROR_STATUS){
        transportFactory.release_transport(transport);
        return status;
    }
    
    status = transport->read_header();
    transportFactory.release_transport(transport);
    return status;
}  


ClearOperation::ClearOperation(TransportFactory &tF, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->cache_name = cache_name;
     this->flags = flags;
}   

int ClearOperation::execute_operation()
{
    transport = transportFactory.get_transport();
    if(transport == NULL) return FAILED_TO_CHOOSE_TRANSPORT;
    transport->write_header(CLEAR_REQUEST, cache_name, flags);
    if(status = transport->flush() != NO_ERROR_STATUS){
        transportFactory.release_transport(transport);
        return status;
    }
    
    status = transport->read_header();
    transportFactory.release_transport(transport);
    return status;
}  