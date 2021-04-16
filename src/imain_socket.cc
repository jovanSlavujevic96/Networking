#include <memory.h>

#include "imain_socket.h"

IMainSocket::IMainSocket(const char* ip, uint16_t port) :
    CSocket::CSocket(std::unique_ptr<SocketInfo>(new SocketInfo{INVALID_SOCKET, std::make_unique<sockaddr_in>()})),
    mIp{ip},
    mPort{port}
{
    if(CSocket::mSocketInfo->socketAddress != nullptr)
    {
        memset(CSocket::mSocketInfo->socketAddress.get(), 0, CSocket::AddrLen);
    }
}

IMainSocket::~IMainSocket() = default;

const std::string& IMainSocket::getIp() const
{
    return mIp;
}

uint16_t IMainSocket::getPort() const
{
    return mPort;
}