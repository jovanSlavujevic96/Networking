#include <memory.h>

#include "imain_socket.h"

IMainSocket::IMainSocket() noexcept(false) :
    CSocket::CSocket{ INVALID_SOCKET /*sock fd*/, nullptr /*running sock addr*/, nullptr /*target sock addr*/ }
{
#if defined(WIN32) || defined(_WIN32)
    WSADATA wsaData;
    int dResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (dResult != 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "IMainSocket::IMainSocket : %llu WSAStartup failed: %d -> %s", (size_t)this, dResult, ::error_message(code));
    }
#endif
}

IMainSocket::~IMainSocket() = default;

std::string IMainSocket::getRunningIp() const noexcept(false)
{
    if (nullptr == CSocket::mRunningSockAddr)
    {
        throw CSocketException("Can't get Running IP. Running Socket Address is NULL.");
    }
    return ::inet_ntoa(*(in_addr*)(CSocket::mRunningSockAddr.get()));
}

uint16_t IMainSocket::getRunningPort() const noexcept(false)
{
    if (nullptr == CSocket::mRunningSockAddr)
    {
        throw CSocketException("Can't get Running port. Running Socket Address is NULL.");
    }
    return ::htons(CSocket::mRunningSockAddr->sin_port);
}

std::string IMainSocket::getTargetIp() const noexcept(false)
{
    if (nullptr == CSocket::mTargetSockAddr)
    {
        throw CSocketException("Can't get Target IP. Target Socket Address is NULL.");
    }
    return ::inet_ntoa(*(in_addr*)(CSocket::mTargetSockAddr.get()));
}

uint16_t IMainSocket::getTargetPort() const noexcept(false)
{
    if (nullptr == CSocket::mRunningSockAddr)
    {
        throw CSocketException("Can't get Target port. Target Socket Address is NULL.");
    }
    return ::htons(CSocket::mRunningSockAddr->sin_port);
}
