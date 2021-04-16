#pragma once

#include "iserver.h"

class CUdpServer : public IServer
{
public:
    CUdpServer(const char* ip, uint16_t port);
    ~CUdpServer() = default;

    void initServer() override;
};
