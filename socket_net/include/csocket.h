#pragma once
#pragma pack(1)

#include <memory>
#include <string>
#include <vector>

#include "socket_utils.h"
#include "csocket_exception.h"
#include "ipackage.h"

class CSocket
{
public:
    CSocket(SOCKET fd, std::unique_ptr<sockaddr_in> runningSockAddr, std::unique_ptr<sockaddr_in> targetSockAddr);
    virtual ~CSocket();

    void closeSocket();

    int32_t operator<< (const IPackage* pkg) const noexcept(false);
    int32_t operator>> (IPackage* pkg) noexcept(false);

    int32_t operator<< (const char* message) const noexcept(false);
    int32_t operator<< (const std::string& message) const noexcept(false);
    template<class T>
    int32_t operator<< (const std::vector<T>& data) const noexcept(false);

protected:
    SOCKET mSocketFd = INVALID_SOCKET;
    std::unique_ptr<sockaddr_in> mRunningSockAddr = nullptr;
    std::unique_ptr<sockaddr_in> mTargetSockAddr = nullptr;

    inline static constexpr size_t cAddrLen = sizeof(sockaddr_in);
    inline static int32_t AddrLen = sizeof(sockaddr_in);
};
