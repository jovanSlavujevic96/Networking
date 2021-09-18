#include "cstring_package.h"

CStringPackage::CStringPackage(const char* cstr) :
    mMessage{cstr}
{

}

CStringPackage::CStringPackage(size_t len) :
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

void CStringPackage::setMaxSize(size_t len)
{
    mMaxLen = len;
}

void CStringPackage::setCurrentSize()
{
    mMessage.resize(mMaxLen);
}
   
void CStringPackage::setCurrentSize(size_t len)
{
    mMessage.resize(len);
}

char* CStringPackage::data()
{
    return &mMessage[0];
}

const char* CStringPackage::cData() const
{
    return mMessage.c_str();
}

size_t CStringPackage::getCurrentSize() const
{
    return mMessage.size();
}

size_t CStringPackage::getMaxSize() const
{
    return mMaxLen;
}