#ifndef RM_H_INCLUDED
#define RM_H_INCLUDED
#include <string>
#include <iostream>
#include <iterator>
#include <remoteCache2.h>
#include <map>

class RMItem;

std::ostream & operator << (std::ostream& s, RMItem & p);

class RMMap{
    private:
    	std::string x;
    	RemoteCache RC; 
    	std::map<std::string,std::string>  bulk;
    public:
    	RMMap();
    	u_int size();
    	RMItem &operator[](const std::string &key);
    	RMMap& operator= ( const std::map<std::string,std::string>& x );

    	int erase(const std::string &key );

    	//const char *operator[](const char *key);
    	void clear();
    	
    	std::string *get(const std::string *key);
    	void set(const std::string *key,const std::string *value);
    	std::map<std::string,std::string>::iterator begin();
    	std::map<std::string,std::string>::iterator end();
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
    RMItem &operator=(RMItem *rm_item);
    
};




#endif
    