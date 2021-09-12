#pragma once

#include "iclient.h"

class CTcpClient : public IClient
{
public:
    explicit CTcpClient(const char* target_ip, uint16_t target_port) noexcept(false);
    inline ~CTcpClient() = default;

    void initClient() noexcept(false) override;
};
