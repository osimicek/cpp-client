#include "RMMap.h"

void example1(){
    RMMap cache;

    cache.clear();
    cache["key"] = "value";
    cache[123] = 3.21;
    cache[4.56] = 654;
    std::cout << cache[123] <<std::endl;

    for (auto entry = cache.begin(); entry != cache.end(); ++entry) {
        std::cout << "key: " << entry->first  << " "<< "value: " << entry->second << std::endl;
    }
}

int main(int, char const *[])
{
    example1();

    return 0;
}