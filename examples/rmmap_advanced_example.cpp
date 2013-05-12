#include "RMMap.h"

void basic(){
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

void size(){
    RMMap cache;

    cache.clear();
    cache["key"] = "value";
    cache[123] = 3.21;
    cache[4.56] = 654;
    std::cout << "size: "<< cache.size() <<std::endl;
}

int main(int, char const *[])
{
    std::cout <<"\nBasic example /////////////////////"<< std::endl;
    basic();
    std::cout <<"\nSize example /////////////////////"<< std::endl;
    size();

    return 0;
}