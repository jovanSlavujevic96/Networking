#pragma once

#include "iclient.h"

class CUdpClient : public IClient
{
public:
    explicit CUdpClient(const char* running_ip, uint16_t running_port, const char* target_ip, uint16_t target_port) noexcept(false);
    inline ~CUdpClient() = default;

    void initClient() override;
};
