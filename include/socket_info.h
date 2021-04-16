#pragma once

#include <memory>

#include "socket_utils.h"

struct SocketInfo
{
    SOCKET socket = INVALID_SOCKET;
    std::unique_ptr<sockaddr_in> socketAddress = nullptr;
};
