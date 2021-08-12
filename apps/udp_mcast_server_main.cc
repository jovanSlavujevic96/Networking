#include <iostream>
#include <chrono>
#include <thread>

#include "cudp_mcast_server.h"
#include "cstring_package.h"

#define MESSAGE "Hello from multicast udp server"

int main()
{
    /* Multicast IP addresses (224.0.0.0 to 239.255.255.255) */
    CUdpMcastServer server("224.0.0.251", 9094);
    try
    {
        server.initServer();
        const CStringPackage pkg(MESSAGE);
        while (true)
        {
            server << &pkg;
            std::cout << "sent: " << pkg.cData() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
