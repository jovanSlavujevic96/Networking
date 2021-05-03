#pragma once

#include "iclient.h"

class CUdpMcastClient : public IClient
{
public:
    CUdpMcastClient(const char* ip, uint16_t port);
    ~CUdpMcastClient() = default;

    void initClient() override;
private:
    static inline constexpr size_t cGroupSize = sizeof(ip_mreq);
};
