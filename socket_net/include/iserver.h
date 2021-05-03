#pragma once

#include <vector>
#include <memory>

#include "imain_socket.h"

class IServer : public IMainSocket
{
public:
    IServer(const char* ip, uint16_t port) : IMainSocket{ip,port} {}
    ~IServer() = default;

    virtual void initServer() noexcept(false) = 0;
};
