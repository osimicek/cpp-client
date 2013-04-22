#include <RMMap.h>
#include "varItem.h"


RMMap::RMMap(){
  RemoteCacheConfig remote_cache_config;
  remote_cache_config.intelligence = CLIENT_INTELLIGENCE_BASIC;
	RC = RemoteCache(&remote_cache_config);

}


u_int RMMap::size(){
        //stahni_data();
        std::map<VarItem,VarItem> bulk;
        RC.getBulk(&bulk);

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
 	RC.clear();
}

 // RMMap& RMMap::operator= ( const std::map<VarItem,VarItem>& x ){
 // 	return (*this);	
 // }

int RMMap::erase (const VarItem &key ){
	int ret;
	ret = RC.remove(&key);
	if(ret == 0) return 1;
	return 0;
}

VarItem *RMMap::get(const VarItem *key){
  VarItem *value = new VarItem();
  RC.get(key,value);

  return value;

}

void RMMap::set(const VarItem *key,const VarItem *value){
	RC.put(key,value);		

}

void RMMap::close(){
  RC.close();
}

std::map<VarItem,VarItem>::iterator RMMap::begin(){
	RC.getBulk(&bulk);	
	return bulk.begin();	
}

std::map<VarItem,VarItem>::iterator RMMap::end(){
	return bulk.end();	
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

std::ostream & operator << (std::ostream& _stream, RMItem & rmItem)
{
    rmItem.print(_stream);
    return _stream;
}

RMItem::RMItem(RMMap *m,const VarItem *key):	rm_map(m),key(key){
	
}



VarItem &RMItem::get() const{ 
	return *rm_map->get(key); 
}

void RMItem::print(std::ostream& _stream){ 
  VarItem *value = new VarItem();
  rm_map->RC.get(key, value);

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
