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

/**
    * Implements "RMMap" containers as interface of Infinispan cache
    * \warning does't support map reduce, you can't iterate over distributed cache
    *
    * @author ondrejsimicek@seznam.cz
*/
class RMMap{
    private:
    	std::string x;
    	RemoteCache *RC;
    	std::map<VarItem,VarItem>  bulk;
    public:
        
        typedef std::map<VarItem,VarItem>::iterator iterator;
    	RMMap();
        RMMap(RemoteCacheConfig *remote_cache_config);
        ~RMMap();
    	u_int size();
    	RMItem &operator[](const VarItem &key);
    	RMMap& operator= ( const std::map<VarItem,VarItem>& x );

    	int erase(const VarItem key);

    	//const char *operator[](const char *key);
    	void clear();
    	VarItem *get(const VarItem *key);
    	void set(const VarItem *key,const VarItem *value);
        void close();
    	iterator begin();
    	iterator end();
};


/**
    * Implements remote item, that is returned by #RMMap.
    * It can set and get entry from cache.
    *
    * @author ondrejsimicek@seznam.cz
*/
class RMItem{
private:
    RMMap *rm_map;
    const VarItem *key;
public:
    RMItem();
    ~RMItem();
	RMItem(RMMap *m,const VarItem *key);
    template <typename TYPE>
    operator TYPE();

    VarItem &get() const;
    void print(std::ostream& _stream);
    RMItem &operator=(const VarItem &value);
    RMItem &operator=(RMItem &rm_item);
    bool operator==(const RMItem &other) const ;
    bool operator!=(const RMItem &other) const ;
    bool operator==(const VarItem &other) const ;
    bool operator!=(const VarItem &other) const ;
};

template <typename TYPE>
RMItem::operator TYPE(){
    VarItem *value = new VarItem();
    rm_map->RC->get(value,key);
      return *value; 
}


#endif
    