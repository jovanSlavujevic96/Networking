#pragma once

#include "csocket.h"

class IMainSocket : public CSocket
{
public:
    explicit IMainSocket() noexcept(false);
    virtual ~IMainSocket() = 0;

    std::string getRunningIp() const noexcept(false);
    uint16_t getRunningPort() const noexcept(false);
    std::string getTargetIp() const noexcept(false);
    uint16_t getTargetPort() const noexcept(false);
protected:
    inline static int32_t ReuseFlag = 1;
    inline constexpr static uint8_t cReuseSize = sizeof(ReuseFlag); 
};
