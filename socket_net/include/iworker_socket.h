#pragma once

#include "ithread.h"
#include "csocket.h"

class IWorkerSocket : 
    public CSocket,
    public IThread
{
public:
    inline explicit IWorkerSocket(SOCKET sock_fd, std::unique_ptr<sockaddr_in> sock_addr) : CSocket{ sock_fd, nullptr, std::move(sock_addr) } {}
    ~IWorkerSocket() = default;
};
