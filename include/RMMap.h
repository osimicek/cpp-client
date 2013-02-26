#ifndef RM_H_INCLUDED
#define RM_H_INCLUDED
#include <string>
#include <iostream>
#include <remoteCache2.h>
#include <RMItem.h>
#include <map>

class RMItem;

std::ostream & operator << (std::ostream& s, RMItem & p);

class RMMap: public std::map<std::string, std::string>{
public:
	std::string x;
	RemoteCache RC; 
	std::map<std::string,std::string>  bulk;

	RMMap();
	u_int size();
	RMItem &operator[](const std::string &key);
	RMMap& operator= ( const std::map<std::string,std::string>& x );

	int erase(const std::string &key );

	//const char *operator[](const char *key);
	void clear();
	
	std::string *get(const std::string *key);
	void set(const std::string *key,const std::string *value);
	RMMap::iterator begin();
	RMMap::iterator end();
};



class RMItem{
private:
    RMMap &rm_map;
    const std::string key;
public:
	RMItem(RMMap &m,const std::string &kk);
    operator std::string();
    std::string &get();
    RMItem &operator=(const std::string &value);
    
};




#endif
    