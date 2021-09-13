#include <iostream>

#include "csocket.h"
#include "csocket_exception.h"

CSocket::CSocket(SOCKET fd, std::unique_ptr<sockaddr_in> runningSockAddr, std::unique_ptr<sockaddr_in> targetSockAddr) :
    mSocketFd{ fd },
    mRunningSockAddr{ std::move(runningSockAddr) },
    mTargetSockAddr{ std::move(targetSockAddr) }
{

}

CSocket::~CSocket()
{
    CSocket::closeSocket();
}

void CSocket::closeSocket()
{
    if (mSocketFd != INVALID_SOCKET)
    {
        ::shutdown(mSocketFd, SHUT_RDWR);
        ::close(mSocketFd);
        mSocketFd = INVALID_SOCKET;
    }
}

int32_t CSocket::operator<< (const IPackage* pkg) const noexcept(false)
{
    if (NULL == pkg)
    {
        throw CSocketException("CSocket::operator<< :: Message to send is NULL.");
    }
    else if ((nullptr == mTargetSockAddr) ||
             (INVALID_SOCKET == mSocketFd))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    else if(pkg->getCurrentSize() == 0)
    {
        throw CSocketException("CSocket::operator<< :: Package size is 0.");
    }
    int32_t ret = ::sendto(mSocketFd, pkg->cData(), pkg->getCurrentSize(), 0, (sockaddr*)mTargetSockAddr.get(), CSocket::AddrLen);
    if(ret < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CSocket::operator<< :: Send failed -> %s.", ::error_message(code));
    }
    return ret;
}

int32_t CSocket::operator>> (IPackage* pkg) noexcept(false)
{
    if(NULL == pkg)
    {
        throw CSocketException("CSocket::operator>> :: pointer to receive message is NULL.");
    }
    else if ((nullptr == mTargetSockAddr) ||
             (INVALID_SOCKET == mSocketFd))
    {
        throw CSocketException("CSocket::operator>> :: Bad socket initialization.");
    }
    int32_t ret = ::recvfrom(mSocketFd, pkg->data(), pkg->getMaxSize(), 0, (sockaddr*)mTargetSockAddr.get(), (socklen_t*)&CSocket::AddrLen);
    if(ret < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CSocket::operator>> :: Receive failed -> %s.", ::error_message(code));
    }
    else if(0 == ret)
    {
        throw CSocketException("CSocket::operator>> :: Receive failed -> returned 0.");
    }
    return ret;
}

int32_t CSocket::operator<< (const char* message) const noexcept(false)
{
    int32_t ret = (int32_t)std::strlen(message);
    if (NULL == message)
    {
        throw CSocketException("CSocket::operator<< :: Message to send is NULL.");
    }
    else if (ret <= 0)
    {
        throw CSocketException("CSocket::operator<< :: Message to send length is %d.", ret);
    }
    else if ((nullptr == mTargetSockAddr) ||
             (INVALID_SOCKET == mSocketFd))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    ret = ::sendto(mSocketFd, message, ret /*strlen*/, 0, (sockaddr*)mTargetSockAddr.get(), CSocket::AddrLen);
    if (ret < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CSocket::operator<< :: Send failed -> %s.", ::error_message(code));
    }
    return ret;
}

int32_t CSocket::operator<< (const std::string& message) const noexcept(false)
{
    int32_t ret = (int32_t)message.size();
    if (ret <= 0)
    {
        throw CSocketException("CSocket::operator<< :: Message to send length is 0.");
    }
    else if ((nullptr == mTargetSockAddr) ||
             (INVALID_SOCKET == mSocketFd))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    ret = ::sendto(mSocketFd, message.c_str(), ret /*string size*/, 0, (sockaddr*)mTargetSockAddr.get(), CSocket::AddrLen);
    if (ret < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CSocket::operator<< :: Send failed -> %s.", ::error_message(code));
    }
    return ret;
}

template<class T>
int32_t CSocket::operator<< (const std::vector<T>& data) const noexcept(false)
{
    int32_t ret = (int32_t)data.size();
    if (ret <= 0)
    {
        throw CSocketException("CSocket::operator<< :: data vector to send size is 0.");
    }
    else if ((nullptr == mTargetSockAddr) ||
             (INVALID_SOCKET == mSocketFd))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    int32_t ret = ::sendto(mSocketFd, data.data(), ret*sizeof(class T) /*vector size x size of template object*/, 0, (sockaddr*)mTargetSockAddr.get(), CSocket::AddrLen);
    if (ret < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CSocket::operator<< :: Send failed -> %s.", ::error_message(code));
    }
    return ret;
}
