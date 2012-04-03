#include "RMMap.h"

RMMap::RMMap(){
	RC = RemoteCache();

}


u_int RMMap::size() const{
        //stahni_data();
        //return std::map<string,string>::size();
        return 5;
 }

std::string &RMMap::operator[](const std::string &key)
 {
 	x.clear();
  	RC.get(&x,&key);
  	return x;
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

 RMMap& RMMap::operator= ( const std::map<std::string,std::string>& x ){
 	std::cout<< "used ="<<std::flush<<std::endl;
 	return (*this);	
 }

int RMMap::erase (const std::string &key ){
	int ret;
	ret = RC.remove(&key);
	if(ret == 0) return 1;
	return 0;
}