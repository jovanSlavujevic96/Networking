#pragma once

#include <cstdint>

class IPackage
{
public:
    explicit IPackage() = default;
    virtual ~IPackage() = default;

    virtual void* getStorage() const = 0;
    virtual uint16_t getCurrentSize() const = 0;
    virtual uint16_t getMaxSize() const = 0;
};
