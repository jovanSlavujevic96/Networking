#pragma once
#pragma pack(1)

#include <memory>

#include "socket_utils.h"
#include "socket_info.h"
#include "csocket_exception.h"
#include "ipakcage.h"

class CSocket
{
public:
    CSocket(std::unique_ptr<SocketInfo> socket_info);
    virtual ~CSocket();

    int32_t operator<< (const IPackage* pkg) const noexcept(false);
    int32_t operator>> (IPackage* pkg) const noexcept(false);

protected:
    std::unique_ptr<SocketInfo> mSocketInfo = nullptr;

    inline static int32_t AddrLen = sizeof(sockaddr_in);
};
