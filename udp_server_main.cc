#include <iostream>
#include <chrono>
#include <thread>

#include "cudp_server.h"
#include "cstring_package.h"

#define MAX_STR_SIZE 1024
#define MESSAGE "Hello from udp server"

int main()
{
    CUdpServer server("127.0.0.1",10002);
    try
    {
        server.initServer();
        CStringPackage pkg(MAX_STR_SIZE);
        while(true)
        {
            pkg.setCurrentSize();
            server >> &pkg;
            std::cout << "received: " << (const char*)pkg.getStorage() << std::endl;
            pkg.setMessage(MESSAGE);
            server << &pkg;
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