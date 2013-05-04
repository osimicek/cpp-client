#include <RMMap.h>
#include "varItem.h"


RMMap::RMMap(){
  RemoteCacheConfig remote_cache_config;
  remote_cache_config.intelligence = CLIENT_INTELLIGENCE_BASIC;
	RC = new RemoteCache(&remote_cache_config);
}

RMMap::RMMap(RemoteCacheConfig *remote_cache_config){
  RC = new RemoteCache(remote_cache_config);
}

RMMap::~RMMap(){
    delete RC;
}


u_int RMMap::size(){
    int status;
    std::map<VarItem,VarItem> bulk;
    status = RC->getBulk(&bulk);
    if(status != NO_ERROR_STATUS){
        throw status;
    }

    return bulk.size();
 }

RMItem &RMMap::operator[](const VarItem &key)
 {
    // std::cout <<"PP"<< key.marshalled<<"  "<<key.marshalled.length()<<std::endl;
    RMItem *item = new RMItem(this, &key);
    return *item;
 }


void RMMap::clear()
{
    int status;
 	  status = RC->clear();
    if(status != NO_ERROR_STATUS){
        throw status;
    }
}

 // RMMap& RMMap::operator= ( const std::map<VarItem,VarItem>& x ){
 // 	return (*this);	
 // }

int RMMap::erase (const VarItem key){
  	int status;
  	status = RC->remove(key);
  	if(status != NO_ERROR_STATUS){
          throw status;
      }
}

VarItem *RMMap::get(const VarItem *key){
    int status;
    VarItem *value = new VarItem();
    status = RC->get(key,value);
    if(status != NO_ERROR_STATUS){
        throw status;
    }

    return value;

}

void RMMap::set(const VarItem *key,const VarItem *value){
    int status;
    status = RC->put(key,value);		
    if(status != NO_ERROR_STATUS){
        throw status;
    }

}

void RMMap::close(){
    RC->close();
}

RMMap::iterator RMMap::begin(){
    int status;
  	status = RC->getBulk(&bulk);	
    if(status != NO_ERROR_STATUS){
        throw status;
    }
  	return bulk.begin();	
}

RMMap::iterator RMMap::end(){
	 return bulk.end();	
}


///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

std::ostream & operator << (std::ostream& _stream, RMItem & rmItem)
{
    rmItem.print(_stream);
    return _stream;
}
RMItem::RMItem(){}

RMItem::RMItem(RMMap *m,const VarItem *key):	rm_map(m),key(key){
}
RMItem::~RMItem(){
    // std::cout <<"DEL item"<<std::endl;
}



VarItem &RMItem::get() const{ 
	return *rm_map->get(key); 
}

void RMItem::print(std::ostream& _stream){ 
  VarItem *value = &get();

  value->print(_stream);
}

RMItem &RMItem::operator=(const VarItem &value)
{
    rm_map->set(key, &value);
    return *this;
}

RMItem &RMItem::operator=(RMItem &rm_item)
{
    rm_map->set(key, &rm_item.get());
    return *this;
}

bool RMItem::operator==(const RMItem &other) const { 
  if(key == other.key){
    return get() == other.get();
  }
  return 0;
 }

 bool RMItem::operator!=(const RMItem &other) const { 
  return !(*this == other);
 }

bool RMItem::operator==(const VarItem &other) const { 
  if(key != NULL){
    return get() == other;
  }
  return 0;
 }

 bool RMItem::operator!=(const VarItem &other) const { 
  return !(*this == other);
 }
