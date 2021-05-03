#pragma once
#pragma pack(1)

#include <memory>
#include <string>
#include <vector>

#include "socket_utils.h"
#include "socket_info.h"
#include "csocket_exception.h"
#include "ipackage.h"

class CSocket
{
public:
    CSocket(std::unique_ptr<SocketInfo> socket_info);
    virtual ~CSocket();

    void closeSocket();

    int32_t operator<< (const IPackage* pkg) const noexcept(false);
    int32_t operator>> (IPackage* pkg) noexcept(false);

    int32_t operator<< (const char* message) const noexcept(false);
    int32_t operator<< (const std::string& message) const noexcept(false);
    template<class T>
    int32_t operator<< (const std::vector<T>& data) const noexcept(false);

protected:
    std::unique_ptr<SocketInfo> mSocketInfo = nullptr;

    inline static int32_t AddrLen = sizeof(sockaddr_in);
};
