#include <iostream>

#include "cudp_client.h"
#include "cstring_package.h"
#include "cport_generator.h"

#define MAX_STR_SIZE 1024
#define MESSAGE "Hello from udp client"

int main()
{
    uint16_t local_port = CPortGenerator::instance().getPort();
    CUdpClient client("0.0.0.0", local_port, "127.0.0.1", 10002);
    try
    {
        client.initClient();
        CStringPackage msg(MAX_STR_SIZE);
        while(true)
        {
            msg.setMessage(MESSAGE);
            client << &msg;
            std::cout << "sent:     " << msg.cData() << std::endl;
            msg.setCurrentSize();
            client >> &msg;
            std::cout << "received: " << msg.cData() << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
