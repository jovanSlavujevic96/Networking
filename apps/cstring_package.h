#pragma once

#include <string>
#include <climits>

#include "ipackage.h"

class CStringPackage : public IPackage
{
public:
    CStringPackage(const char* cstr);
    CStringPackage(size_t len);

    CStringPackage() = default;
    ~CStringPackage() = default;

    CStringPackage& operator+=(const CStringPackage& pkg);
    CStringPackage& operator+=(const char* cstr);
    CStringPackage& operator+=(const std::string& cstr);

    void clearPackage();
    void setMessage(const char* message);
    void setMessage(const std::string& message);
    void setMaxSize(size_t len);
    void setCurrentSize();
    void setCurrentSize(size_t len);

    const char* cData() const override;
    size_t getCurrentSize() const override;
    size_t getMaxSize() const override;

private:
    char* data() override;

    std::string mMessage;
    size_t mMaxLen = USHRT_MAX;
};
