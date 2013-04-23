#include "RMMap.h"

void example1(){
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
}

int main(int, char const *[])
{
    example1();

    return 0;
}