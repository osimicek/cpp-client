#include <RMMap.h>
#include "varItem.h"


RMMap::RMMap(){
	RC = RemoteCache();

}


u_int RMMap::size(){
        //stahni_data();
        std::map<VarItem,VarItem> bulk;
        RC.getBulk(&bulk);

        return bulk.size();
 }

VarItem &RMMap::operator[](const VarItem &key)
 {
    VarItem *value = new VarItem();
 	RC.get(value,&key);
    //std::cout << "val "<<*value <<std::endl;
    return *value;
 }
/*
const char *RMMap::operator[](const char *key)
 {
  	std::cout<< key<<std::flush<<std::endl;
  	std::string x;
  	x ="blaaaa";
  	return x.c_str();
      //return std::map<string,string>
 }*/

void RMMap::clear()
{
 	RC.clear();
}

/* RMMap& RMMap::operator= ( const std::map<VarItem,VarItem>& x ){
 	std::cout<< "used ="<<std::flush<<std::endl;
 	return (*this);	
 }*/

int RMMap::erase (const VarItem &key ){
	int ret;
	ret = RC.remove(&key);
	if(ret == 0) return 1;
	return 0;
}


void RMMap::set(const VarItem *key,const VarItem *value){
	RC.put(key,value);		

}

std::map<VarItem,VarItem>::iterator RMMap::begin(){
	RC.getBulk(&bulk);	
	return bulk.begin();	
}

std::map<VarItem,VarItem>::iterator RMMap::end(){
	return bulk.end();	
}




// RMItem::RMItem(RMMap &m,const std::string &kk):	rm_map(m),key(kk){
	
// }


// RMItem::operator std::string(){
// 	//std::cout<< "pretypovani"<<std::flush<<std::endl; 
// 	return *rm_map.get(&key); 
// }
// std::string &RMItem::get(){ 
// 	return *rm_map.get(&key); 
// }

// RMItem &RMItem::operator=(const std::string &value)
// {
//     rm_map.set(&key, &value);
//     return *this;
// }

// RMItem &RMItem::operator=(RMItem *rm_item)
// {
//     rm_map.set(&key, &rm_item->get());
//     return *this;
// }

