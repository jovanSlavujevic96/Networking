#pragma once

#include "iserver.h"

class CUdpMcastServer : public IServer
{
public:
    explicit CUdpMcastServer(const char* mcast_ip, uint16_t mcast_port) noexcept(false);
    inline ~CUdpMcastServer() = default;

    void initServer() override;
private:
    static inline constexpr size_t cInAddr = sizeof(in_addr);
};
