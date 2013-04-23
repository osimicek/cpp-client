#include "remoteCache.h"

int main(int, char const *[])
{
    RemoteCache cache;
    std::string value;
    int status;

    cache.put("key","value");
    status = cache.get("key",&value);
    std::cout << status << " " << value <<std::endl;

    return 0;
}