#pragma once

#include "iclient.h"

class CTcpClient : public IClient
{
public:
    CTcpClient(const char* ip, uint16_t port);
    ~CTcpClient() = default;

    void initClient() noexcept(false) override;
};
