#include <iostream>

#include "csocket.h"
#include "csocket_exception.h"

CSocket::CSocket(std::unique_ptr<SocketInfo> socket_info) :
    mSocketInfo{std::move(socket_info)}
{

}

CSocket::~CSocket()
{
    if (mSocketInfo->socket != INVALID_SOCKET)
    {
        closeSocket();
    }
}

void CSocket::closeSocket()
{
    shutdown(mSocketInfo->socket, SHUT_RDWR);
    close(mSocketInfo->socket);
    mSocketInfo->socket = INVALID_SOCKET;
}

int32_t CSocket::operator<< (const IPackage* pkg) const noexcept(false)
{
    if (NULL == pkg)
    {
        throw CSocketException("CSocket::operator<< :: Message to send is NULL.");
    }
    else if ((nullptr == mSocketInfo->socketAddress) || 
             (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    else if(pkg->getCurrentSize() == 0)
    {
        throw CSocketException("CSocket::operator<< :: Package size is 0.");
    }
    int32_t ret = sendto(mSocketInfo->socket, pkg->cData(), pkg->getCurrentSize(), 0, (sockaddr*)mSocketInfo->socketAddress.get(), CSocket::AddrLen);
    if(ret < 0)
    {
        throw CSocketException("CSocket::operator<< :: Send failed -> %s.", error_message());
    }
    return ret;
}

int32_t CSocket::operator>> (IPackage* pkg) noexcept(false)
{
    if(NULL == pkg)
    {
        throw CSocketException("CSocket::operator>> :: pointer to receive message is NULL.");
    }
    else if ((nullptr == mSocketInfo->socketAddress) || 
             (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator>> :: Bad socket initialization.");
    }
    int32_t ret = recvfrom(mSocketInfo->socket, pkg->data(), pkg->getMaxSize(), 0, (sockaddr*)mSocketInfo->socketAddress.get(), (socklen_t*)&CSocket::AddrLen);
    if(ret < 0)
    {
        throw CSocketException("CSocket::operator>> :: Receive failed -> %s.", error_message());
    }
    else if(0 == ret)
    {
        throw CSocketException("CSocket::operator>> :: Receive failed -> returned 0.");
    }
    return ret;
}

int32_t CSocket::operator<< (const char* message) const noexcept(false)
{
    int32_t ret;
    if (NULL == message)
    {
        throw CSocketException("CSocket::operator<< :: Message to send is NULL.");
    }
    else if ((ret = (int32_t)strlen(message)) <= 0)
    {
        throw CSocketException("CSocket::operator<< :: Message to send length is %d.", ret);
    }
    else if ((nullptr == mSocketInfo->socketAddress) ||
             (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    ret = sendto(mSocketInfo->socket, message, ret /*strlen*/, 0, (sockaddr*)mSocketInfo->socketAddress.get(), CSocket::AddrLen);
    if (ret < 0)
    {
        throw CSocketException("CSocket::operator<< :: Send failed -> %s.", error_message());
    }
    return ret;
}

int32_t CSocket::operator<< (const std::string& message) const noexcept(false)
{
    if (message.size() == 0)
    {
        throw CSocketException("CSocket::operator<< :: Message to send length is 0.");
    }
    else if ((nullptr == mSocketInfo->socketAddress) ||
             (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    int32_t ret = sendto(mSocketInfo->socket, message.c_str(), (int32_t)message.size(), 0, (sockaddr*)mSocketInfo->socketAddress.get(), CSocket::AddrLen);
    if (ret < 0)
    {
        throw CSocketException("CSocket::operator<< :: Send failed -> %s.", error_message());
    }
    return ret;
}

template<class T>
int32_t CSocket::operator<< (const std::vector<T>& data) const noexcept(false)
{
    if (data.size() == 0)
    {
        throw CSocketException("CSocket::operator<< :: data vector to send size is 0.");
    }
    else if ((nullptr == mSocketInfo->socketAddress) ||
             (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    int32_t ret = sendto(mSocketInfo->socket, data.data(), (int32_t)data.size()*sizeof(class T), 0, (sockaddr*)mSocketInfo->socketAddress.get(), CSocket::AddrLen);
    if (ret < 0)
    {
        throw CSocketException("CSocket::operator<< :: Send failed -> %s.", error_message());
    }
    return ret;
}
