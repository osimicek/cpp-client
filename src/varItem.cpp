#include "varItem.h"



std::ostream & operator << (std::ostream& _stream, const VarItem & varItem) 
{
    varItem.print(_stream);
    return _stream;
}

VarItem::VarItem(){init();}
VarItem::VarItem(const std::string value){
    init();
    marshaller->dump(&value, &marshalled);
}
VarItem::VarItem(const std::string *value){
    init();
    marshaller->dump(value, &marshalled);
}
VarItem::VarItem(const char *value){
    init(); 
    marshaller->dump(value, &marshalled);
}
VarItem::VarItem(const double value){
    init();
    marshaller->dump(value, &marshalled);
}

VarItem::VarItem(const int value){
    init();
    marshaller->dump(value, &marshalled);
}
VarItem::VarItem(const VarItem *origin){
    init();
    marshalled = origin->marshalled;
}
VarItem::VarItem(const VarItem& origin){
    //copy constructor
    //std::cout <<"new new "<<this <<" z "<<&origin<<std::flush<<std::endl;
    init();
    this->marshalled = origin.marshalled;
    // std::cout <<"/copy"<<std::flush<<std::endl;
}
VarItem::~VarItem(){
    // std::cout <<"free "<<this <<" "<<marshaller<<std::flush<<std::endl;
    if(alocated_marshaller){
        //causes error in std::map
        //delete marshaller;
        marshaller = NULL;
    }
}

void VarItem::init(){
    alocated_marshaller = false;
    if(DEFAULT_MARSHALLER != NULL){
        // std::cout<<std::dec<< "WORKED"<<DEFAULT_MARSHALLER<< std::flush;
        marshaller = DEFAULT_MARSHALLER;
    }else{
        alocated_marshaller = true;
        marshaller = new MarshallerJBoss();
        // std::cout <<"new "<<this <<" "<< marshaller<<std::flush<<std::endl;
    }
}

int VarItem::get_type(){
    return marshaller->get_type(&marshalled);
}

void VarItem::clear(){
    marshalled  = "";
}

void VarItem::set_value(std::string *value){
    marshalled = *value;
}

std::ostream & VarItem::print(std::ostream& _stream) const{
    marshaller->print(_stream, &marshalled);
    return _stream;
}

bool VarItem::operator==(const VarItem &other) const { 
    return marshalled == other.marshalled;
 }

bool VarItem::operator!=(const VarItem &other) const { 
    return marshalled != other.marshalled;
 }

bool VarItem::operator==(const std::string  &other) const { 
    std::string strVal; 
    marshaller->load(&marshalled, &strVal);
    return  strVal == other;
 }

bool VarItem::operator!=(const std::string  &other) const { 
    std::string strVal; 
    marshaller->load(&marshalled, &strVal);
    return  strVal != other;
 }

bool VarItem::operator==(const char*  other) const { 
    return  *this == std::string(other);
 }

 bool VarItem::operator!=(const char*  other) const { 
    return  *this != std::string(other);
 }

bool VarItem::operator==(const int &other) const { 
    int intVal;
    marshaller->load(&marshalled, &intVal);
    return intVal == other;
 }

bool VarItem::operator!=(const int &other) const { 
    int intVal;
    marshaller->load(&marshalled, &intVal);
    return intVal == other;
 }

bool VarItem::operator==(const double &other) const { 
    double doubleVal;
    marshaller->load(&marshalled, &doubleVal);
    return doubleVal == other;
 }

bool VarItem::operator!=(const double &other) const { 
    double doubleVal;
    marshaller->load(&marshalled, &doubleVal);
    return doubleVal == other;
 }