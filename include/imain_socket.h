#pragma once

#include "csocket.h"

class IMainSocket : public CSocket
{
public:
    explicit IMainSocket(const char* ip, uint16_t port) noexcept(false);
    virtual ~IMainSocket() = 0;

    const std::string& getIp() const;
    uint16_t getPort() const;
protected:
    std::string mIp;
    uint16_t mPort;

    inline static int32_t ReuseFlag = 1;
    inline constexpr static uint8_t cReuseSize = sizeof(ReuseFlag); 
};
