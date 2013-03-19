#ifndef RM_H_INCLUDED
#define RM_H_INCLUDED
#include <string>
#include <iostream>
#include <iterator>
#include <remoteCache.h>
#include "varItem.h"
#include <map>

class RMItem;

std::ostream & operator << (std::ostream& s, RMItem & p);

class RMMap{
    private:
    	std::string x;
    	RemoteCache RC; 
    	std::map<VarItem,VarItem>  bulk;
    public:
    	RMMap();
    	u_int size();
    	VarItem &operator[](const VarItem &key);
    	RMMap& operator= ( const std::map<VarItem,VarItem>& x );

    	int erase(const VarItem &key );

    	//const char *operator[](const char *key);
    	void clear();
    	
    	void set(const VarItem *key,const VarItem *value);
    	std::map<VarItem,VarItem>::iterator begin();
    	std::map<VarItem,VarItem>::iterator end();
};



// class RMItem{
// private:
//     RMMap &rm_map;
//     const std::string key;
// public:
// 	RMItem(RMMap &m,const std::string &kk);
//     operator std::string();
//     std::string &get();
//     RMItem &operator=(const std::string &value);
//     RMItem &operator=(RMItem *rm_item);
    
// };




#endif
    