#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include "iworker_socket.h"
#include "ctcp_server.h"
#include "cstring_package.h"

#define MAX_STR_SIZE 1024

class MyWorkingSocket : public IWorkerSocket
{
public:
    explicit MyWorkingSocket(SOCKET sock_fd, std::unique_ptr<sockaddr_in> sock_addr, int id);
    inline ~MyWorkingSocket() = default;

private:
    void threadEntry() override;

    CStringPackage mPackage;
    const std::string mMessage;
    int mId;
};

MyWorkingSocket::MyWorkingSocket(SOCKET sock_fd, std::unique_ptr<sockaddr_in> sock_addr, int id) :
    IWorkerSocket::IWorkerSocket{ sock_fd, std::move(sock_addr) },
    mPackage{ MAX_STR_SIZE },
    mMessage{"Hello to client:" + std::to_string(id)},
    mId{id}
{

}

void MyWorkingSocket::threadEntry()
{
    MyWorkingSocket& _this = *this;
    while(true)
    {
        try
        {
            mPackage.setCurrentSize();
            _this >> &mPackage;
            std::cout << "ID-" << mId << ": " << mPackage.cData() << std::endl;
            mPackage.setMessage(mMessage);
            _this << &mPackage;
            mPackage.clearPackage();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        catch(const CSocketException& exception)
        {
            std::cerr << "ID-" << mId << ": " << exception.what() << std::endl;
            break;
        }
    }
}

namespace
{
    static inline std::unique_ptr<CSocket> allocWorkingSocket(SOCKET fd, std::unique_ptr<sockaddr_in> addr)
    {
        static int acceptanceId = 0;
        MyWorkingSocket* worker = new MyWorkingSocket(fd, std::move(addr), acceptanceId++);
        worker->start();
        return std::unique_ptr<MyWorkingSocket>(worker);
    }
}

int main()
{
    CTcpServer server("127.0.0.1", 10001);
    server.setAllocSocketFunc(::allocWorkingSocket);
    try
    {
        server.initServer();
        while(true)
        {
            (void)server.acceptClient();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
