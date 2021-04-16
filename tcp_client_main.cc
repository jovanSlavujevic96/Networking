#include <iostream>

#include "ctcp_client.h"
#include "cstring_package.h"

#define MAX_STR_SIZE 1024

#define MESSAGE "Hello from client"

int main()
{
    CTcpClient client("127.0.0.1", 10001);
    
    try
    {
        client.initClient();
        CStringPackage msg(MAX_STR_SIZE);
        while(true)
        {
            msg.setMessage(MESSAGE);
            client << &msg;
            msg.setCurrentSize();
            client >> &msg;
            std::cout << (const char*)msg.getStorage() << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}