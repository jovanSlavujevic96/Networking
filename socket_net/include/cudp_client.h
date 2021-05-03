#pragma once

#include "iclient.h"

class CUdpClient : public IClient
{
public:
    CUdpClient(const char* ip, uint16_t port);
    ~CUdpClient() = default;

    void initClient() override;
};
