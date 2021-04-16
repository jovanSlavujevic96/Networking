#pragma once

#include "ithread.h"
#include "csocket.h"

class IWorkerSocket : 
    public CSocket,
    public IThread
{
public:
    IWorkerSocket(std::unique_ptr<SocketInfo> socket_info) : CSocket(std::move(socket_info)) {}
    ~IWorkerSocket() = default;
};
