#include "RMMap.h"

int main(int, char const *[])
{
    RMMap cache;

    cache.clear();
    cache["key"] = "value";
    cache[123] = 3.21;
    cache[4.56] = 654;
    std::cout << cache[123] <<std::endl;

    for (auto entry = cache.begin(); entry != cache.end(); ++entry) {
        std::cout << "key: " << entry->first  << " "<< "value: " << entry->second << std::endl;
    }

    return 0;
}