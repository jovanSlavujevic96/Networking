#pragma once

#include "imain_socket.h"

class IServer : public IMainSocket
{
public:
    inline explicit IServer() = default;
    inline ~IServer() = default;

    virtual void initServer() noexcept(false) = 0;
};
