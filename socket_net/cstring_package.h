#pragma once

#include <string>
#include <climits>

#include "ipackage.h"

class CStringPackage : public IPackage
{
public:
    CStringPackage(const char* cstr);
    CStringPackage(uint16_t len);

    CStringPackage() = default;
    ~CStringPackage() = default;

    CStringPackage& operator+=(const CStringPackage& pkg);
    CStringPackage& operator+=(const char* cstr);
    CStringPackage& operator+=(const std::string& cstr);

    void clearPackage();
    void setMessage(const char* message);
    void setMessage(const std::string& message);
    void setMaxSize(uint16_t len);
    void setCurrentSize();
    void setCurrentSize(uint16_t len);

    const char* cData() const override;
    uint16_t getCurrentSize() const override;
    uint16_t getMaxSize() const override;

private:
    char* data() override;

    std::string mMessage;
    uint16_t mMaxLen = USHRT_MAX;    
};
