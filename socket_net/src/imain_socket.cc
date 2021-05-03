#include <memory.h>

#include "imain_socket.h"

IMainSocket::IMainSocket(const char* ip, uint16_t port) noexcept(false) :
    CSocket::CSocket(std::unique_ptr<SocketInfo>(new SocketInfo{INVALID_SOCKET, std::make_unique<sockaddr_in>()})),
    mIp{ip},
    mPort{port},
    mInitDone{false}
{
    if(CSocket::mSocketInfo->socketAddress != nullptr)
    {
        memset(CSocket::mSocketInfo->socketAddress.get(), 0, CSocket::AddrLen);
    }
#if defined(WIN32) || defined(_WIN32)
    WSADATA wsaData;
    int dResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (dResult != 0)
    {
        throw CSocketException("IMainSocket::IMainSocket : socket{%s:%u} WSAStartup failed: %d -> %s", ip, port, dResult, error_message());
    }
#endif
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

bool IMainSocket::IsInit() const
{
    return mInitDone;
}
