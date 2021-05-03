#include <iostream>

#include "cudp_mcast_client.h"
#include "cstring_package.h"

#define MAX_STR_SIZE 1024

int main()
{
    /* Multicast IP addresses (224.0.0.0 to 239.255.255.255) */
    CUdpMcastClient client("224.0.0.251", 9094);
    try
    {
        client.initClient();
        CStringPackage msg(MAX_STR_SIZE);
        while (true)
        {
            msg.setCurrentSize();
            client >> &msg;
            std::cout << msg.cData() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
