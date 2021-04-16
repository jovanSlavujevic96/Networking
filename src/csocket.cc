#include "csocket_exception.h"
#include "csocket.h"

CSocket::CSocket(std::unique_ptr<SocketInfo> socket_info) :
    mSocketInfo{std::move(socket_info)}
{

}

CSocket::~CSocket()
{
    shutdown(mSocketInfo->socket, SHUT_RDWR);
    close(mSocketInfo->socket);
}

int32_t CSocket::operator<< (const IPackage* pkg) const noexcept(false)
{
    if (NULL == pkg)
    {
        throw CSocketException("CSocket::operator<< :: Message to send is NULL.");
    }
    else if((nullptr == mSocketInfo->socketAddress) || 
            (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator<< :: Bad socket initialization.");
    }
    else if(pkg->getCurrentSize() == 0)
    {
        throw CSocketException("CSocket::operator<< :: Package size is 0.");
    }
    int32_t ret = sendto(mSocketInfo->socket, pkg->getStorage(), pkg->getCurrentSize(), MSG_WAITALL, (sockaddr*)mSocketInfo->socketAddress.get(), CSocket::AddrLen);
    if(ret < 0)
    {
        throw CSocketException("CSocket::operator<< :: Send failed -> %s.", strerror(errno));
    }
    return ret;
}

int32_t CSocket::operator>> (IPackage* pkg) const noexcept(false)
{
    if(NULL == pkg)
    {
        throw CSocketException("CSocket::operator>> :: pointer to receive message is NULL.");
    }
    else if((nullptr == mSocketInfo->socketAddress) || 
            (INVALID_SOCKET == mSocketInfo->socket))
    {
        throw CSocketException("CSocket::operator>> :: Bad socket initialization.");
    }
    else if(pkg->getCurrentSize() != pkg->getMaxSize())
    {
        throw CSocketException("CSocket::operator>> :: Package current size is different then it's max (%u != %u).", pkg->getCurrentSize(), pkg->getMaxSize());
    }
    int32_t ret = recvfrom(mSocketInfo->socket, pkg->getStorage(), pkg->getMaxSize(), 0, (sockaddr*)mSocketInfo->socketAddress.get(), (socklen_t*)&CSocket::AddrLen);
    if(ret < 0)
    {
        throw CSocketException("CSocket::operator>> :: Receive failed -> %s.", strerror(errno));
    }
    else if(0 == ret)
    {
        throw CSocketException("CSocket::operator>> :: Receive failed -> returned 0.");
    }
    return ret;
}
