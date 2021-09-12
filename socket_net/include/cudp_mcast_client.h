#pragma once

#include "iclient.h"

class CUdpMcastClient : public IClient
{
public:
    explicit CUdpMcastClient(const char* mcast_ip, uint16_t mcast_port) noexcept(false);
    inline ~CUdpMcastClient() = default;

    void initClient() override;
private:
    static inline constexpr size_t cGroupSize = sizeof(ip_mreq);
};
