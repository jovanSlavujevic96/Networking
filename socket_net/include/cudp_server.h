#pragma once

#include "iserver.h"

class CUdpServer : public IServer
{
public:
    explicit CUdpServer(const char* running_ip, uint16_t running_port, const char* target_ip, uint16_t target_port) noexcept(false);
    inline ~CUdpServer() = default;

    void initServer() override;
};
