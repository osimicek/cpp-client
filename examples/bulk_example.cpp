#include "remoteCache.h"

int main(int, char const *[])
{
    RemoteCache cache;
    std::map<VarItem,VarItem>  bulk;

    cache.clear();
    cache.put(10, 100);
    cache.put(10.1, 100.1);
    cache.getBulk(&bulk);
    for (auto entry = bulk.begin(); entry != bulk.end(); ++entry) {
        std::cout << "key: " << entry->first<< " "<< "value: " << entry->second << std::endl;
    }

    return 0;
}