#include "operations.h"


AbstractOperation::AbstractOperation(TransportFactory &tF):transportFactory(tF)
{
     std::cout << "AO" <<std::endl;
}    
int AbstractOperation::execute()
{
     std::cout << "A e" <<std::endl;
     return execute_operation();
} 

int AbstractOperation::execute_operation()
{
     std::cout << "A eO" <<std::endl;

}  



GetOperation::GetOperation(std::string *value, TransportFactory &tF, const std::string *key, const std::string *cache_name, int flags):AbstractOperation(tF)
{    
     this->key = key;
     this->value = value;
}   

int GetOperation::execute_operation()
{
    transport = transportFactory.get_transport(key);
    if(transport == NULL) return FAILED_TO_CHOOSE_TRANSPORT;

    transport->write_header(GET_REQUEST);
    transport->write_array(key);
    transport->flush();
    status = transport->read_header();
    if(status == NO_ERROR_STATUS){
        transport->read_array(value);
        std::cout << *value << std::endl;
    }
    return status;
    
}  

PutOperation::PutOperation(const std::string *value, TransportFactory &tF, const std::string *key, const std::string *cache_name, int flags, int lifespan, int idle):AbstractOperation(tF)
{    
     this->key = key;
     this->value = value;
     this->lifespan = lifespan;
     this->idle = idle;
}   

int PutOperation::execute_operation()
{
    transport = transportFactory.get_transport(key);
    if(transport == NULL) return FAILED_TO_CHOOSE_TRANSPORT;

    transport->write_header(PUT_REQUEST);
    transport->write_array(key);
    transport->write_varint(this->lifespan); //lifespan
    transport->write_varint(this->idle); //idle
    transport->write_array(value);
    transport->flush();
    status = transport->read_header();
    return status;
}  