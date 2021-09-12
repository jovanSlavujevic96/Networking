#include <iostream>
#include <chrono>
#include <thread>

#include "cudp_server.h"
#include "cstring_package.h"

#define MAX_STR_SIZE 1024
#define MESSAGE "Hello from udp server"

int main()
{
    CUdpServer server("0.0.0.0", 10002, "0.0.0.0", 0);
    try
    {
        server.initServer();
        CStringPackage pkg(MAX_STR_SIZE);
        while(true)
        {
            pkg.setCurrentSize();
            server >> &pkg;
            std::cout << "received: " << pkg.cData() << std::endl;
            pkg.setMessage(MESSAGE);
            server << &pkg;
            std::cout << "sent:     " << pkg.cData() << std::endl;
            pkg.clearPackage();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
