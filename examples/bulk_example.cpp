#include "remoteCache.h"

int main(int, char const *[])
{
    RemoteCache cache;
    std::map<VarItem,VarItem>  bulk;
    std::map<VarItem,VarItem>::iterator pos;

    cache.clear();
    cache.put(10, 100);
    cache.put(10.1, 100.1);
    cache.getBulk(&bulk);
    for (pos = bulk.begin(); pos != bulk.end(); ++pos) {
        std::cout << "key: " << pos->first<< " "<< "value: " << pos->second << std::endl;
    }

    return 0;
}