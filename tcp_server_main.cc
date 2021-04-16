#include <iostream>

#include "iworker_socket.h"
#include "ctcp_server.h"
#include "cstring_package.h"

#define MAX_STR_SIZE 1024

class MyWorkingSocet : public IWorkerSocket
{
public:
    MyWorkingSocet(std::unique_ptr<SocketInfo> socket_info, int id);
    ~MyWorkingSocet() = default;

private:
    void threadEntry() override;

    CStringPackage mPackage;
    const std::string mMessage;
    int mId;
};

MyWorkingSocet::MyWorkingSocet(std::unique_ptr<SocketInfo> socket_info, int id) : 
    IWorkerSocket::IWorkerSocket(std::move(socket_info)), 
    mPackage(MAX_STR_SIZE),
    mMessage{"Hello to client:"+std::to_string(id)},
    mId{id}
{

}

void MyWorkingSocet::threadEntry()
{
    MyWorkingSocet& _this = *this;
    while(true)
    {
        try{
            mPackage.setCurrentSize();
            _this >> &mPackage;
            std::cout << "ID-" << mId << ": " << (const char*)mPackage.getStorage() << std::endl;
            mPackage.setMessage(mMessage);
            _this << &mPackage;
            mPackage.clearPackage();
            sleep(1);
        }
        catch(const CSocketException& exception)
        {
            std::cerr << "ID-" << mId << ": " << exception.what() << std::endl;
            break;
        }
    }
}

int main()
{
    CTcpServer server("127.0.0.1", 10001);
    try
    {
        server.initServer();
        std::unique_ptr<SocketInfo> socket_info = nullptr;
        while(true)
        {
            static int acceptanceId = 0;
            socket_info = std::make_unique<SocketInfo>();
            server.acceptClient(socket_info);
            std::unique_ptr<IWorkerSocket> client_socket = std::make_unique<MyWorkingSocet>(std::move(socket_info), acceptanceId++); 
	        client_socket->start();
            server.addClient(std::move(client_socket));
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}