#pragma once

#include "iserver.h"

class CUdpMcastServer : public IServer
{
public:
    CUdpMcastServer(const char* ip, uint16_t port);
    ~CUdpMcastServer() = default;

    void initServer() override;
private:
    static inline constexpr size_t cInAddr = sizeof(in_addr);
};
