#include "cstring_package.h"

CStringPackage::CStringPackage(const char* cstr) :
    mMessage{cstr}
{

}

CStringPackage::CStringPackage(uint16_t len) :
    mMaxLen{len}
{

}

CStringPackage& CStringPackage::operator+=(const CStringPackage& pkg)
{
    this->mMessage += pkg.mMessage;
    return *this;
}

CStringPackage& CStringPackage::operator+=(const char* cstr)
{
    this->mMessage += cstr;
    return *this;
}

CStringPackage& CStringPackage::operator+=(const std::string& cstr)
{
    this->mMessage += cstr;
    return *this;
}

void CStringPackage::clearPackage()
{
    mMessage.clear();
}

void CStringPackage::setMessage(const char* message)
{
    mMessage = message;
}

void CStringPackage::setMessage(const std::string& message)
{
    mMessage = message;
}

void CStringPackage::setMaxSize(uint16_t len)
{
    mMaxLen = len;
}

void CStringPackage::setCurrentSize()
{
    mMessage.resize(mMaxLen);
}
   
void CStringPackage::setCurrentSize(uint16_t len)
{
    mMessage.resize(len);
}

void* CStringPackage::getStorage() const
{
    return (void*)mMessage.c_str();
}

uint16_t CStringPackage::getCurrentSize() const
{
    return mMessage.size();
}

uint16_t CStringPackage::getMaxSize() const
{
    return mMaxLen;
}