#pragma once

#include "imain_socket.h"

class IClient : public IMainSocket
{
public:
    inline explicit IClient() = default;
    inline ~IClient() = default;

    virtual void initClient() noexcept(false) = 0;
};
