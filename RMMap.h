#ifndef RM_H_INCLUDED
#define RM_H_INCLUDED
#include <string>
#include <iostream>
#include "remoteCache.h"
#include <map>


class RMMap: public std::map<std::string, std::string>{
public:
	std::string x;
	RemoteCache RC; 

	RMMap();
	u_int size();
	std::string &operator[](const std::string &key);
	RMMap& operator= ( const std::map<std::string,std::string>& x );

	int erase(const std::string &key );

	//const char *operator[](const char *key);
	void clear();
};

#endif
    