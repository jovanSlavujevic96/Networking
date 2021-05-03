#pragma once

#include "imain_socket.h"

class IClient : public IMainSocket
{
public:
    IClient(const char* ip, uint16_t port) : IMainSocket{ip, port} {}
    ~IClient() = default;

    virtual void initClient() noexcept(false) = 0;
};
