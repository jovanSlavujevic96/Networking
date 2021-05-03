#pragma once

#include <cstdint>

class CSocket;

class IPackage
{
public:
    explicit IPackage() = default;
    virtual ~IPackage() = default;

    virtual const char* cData() const = 0;
    virtual uint16_t getCurrentSize() const = 0;
    virtual uint16_t getMaxSize() const = 0;

private:
    friend class CSocket;
    virtual char* data() = 0;
};
