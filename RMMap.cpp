#include "RMMap.h"

std::ostream & operator << (std::ostream& s, RMItem & i)
{
	std::cout<< "<<"<<(std::string)i <<std::flush<<std::endl;
   	s << (std::string)i;
    return s;
}


RMMap::RMMap(){
	RC = RemoteCache();

}


u_int RMMap::size(){
        //stahni_data();
        std::map<std::string,std::string> bulk;
        RC.getBulk(&bulk);

        return bulk.size();
 }

RMItem &RMMap::operator[](const std::string &key)
 {
 	RMItem *x = new RMItem(*this,key);
  	return *x;
      //return std::map<string,string>
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

/* RMMap& RMMap::operator= ( const std::map<std::string,std::string>& x ){
 	std::cout<< "used ="<<std::flush<<std::endl;
 	return (*this);	
 }*/

int RMMap::erase (const std::string &key ){
	int ret;
	ret = RC.remove(&key);
	if(ret == 0) return 1;
	return 0;
}

std::string *RMMap::get(const std::string *key){
	std::string *value = new std::string();
	//std::cout << "key "<<*key<<std::endl;
	RC.get(value,key);
	//std::cout << "val "<<*value <<std::endl;
	return value;

}
void RMMap::set(const std::string *key,const std::string *value){
	RC.put(key,value);		

}

std::map<std::string,std::string>::iterator RMMap::begin(){
	RC.getBulk(&bulk);	
	return bulk.begin();	
}

std::map<std::string,std::string>::iterator RMMap::end(){
	return bulk.end();	
}




RMItem::RMItem(RMMap &m,const std::string &kk):	rm_map(m),key(kk){
	
}


RMItem::operator std::string(){
	//std::cout<< "pretypovani"<<std::flush<<std::endl; 
	return *rm_map.get(&key); 
}
std::string &RMItem::get(){ 
	return *rm_map.get(&key); 
}

RMItem &RMItem::operator=(const std::string &value)
{
    rm_map.set(&key, &value);
    return *this;
}

