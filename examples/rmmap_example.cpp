#include "RMMap.h"

int main(int, char const *[])
{
    RMMap cache;

    cache.clear();
    cache["key"] = "value";
    cache[123] = 3.21;
    cache[4.56] = 654;
    std::cout << cache[123] <<std::endl;

    RMMap::iterator pos;
    for (pos = cache.begin(); pos != cache.end(); ++pos) {
        std::cout << "key: " << pos->first  << " "<< "value: " << pos->second << std::endl;
    }

    return 0;
}