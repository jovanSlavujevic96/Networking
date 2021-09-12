#include "cudp_mcast_client.h"

CUdpMcastClient::CUdpMcastClient(const char* mcast_ip, uint16_t mcast_port) noexcept(false)
{
    if (nullptr == CSocket::mRunningSockAddr)
    {
        CSocket::mRunningSockAddr = std::make_unique<sockaddr_in>();
    }
    std::memset(CSocket::mRunningSockAddr.get(), 0, CSocket::cAddrLen);
    CSocket::mRunningSockAddr->sin_family = AF_INET;
    CSocket::mRunningSockAddr->sin_port = ::htons(mcast_port);
    if (::inet_pton(AF_INET, "0.0.0.0", &CSocket::mRunningSockAddr->sin_addr) <= 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpServer::CUdpServer : Bad inet_pton for running IP for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
    if (nullptr == CSocket::mTargetSockAddr)
    {
        CSocket::mTargetSockAddr = std::make_unique<sockaddr_in>();
    }
    std::memset(CSocket::mTargetSockAddr.get(), 0, CSocket::cAddrLen);
    CSocket::mTargetSockAddr->sin_family = AF_INET;
    CSocket::mTargetSockAddr->sin_port = ::htons(mcast_port);
    if (::inet_pton(AF_INET, mcast_ip, &CSocket::mTargetSockAddr->sin_addr) <= 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpServer::CUdpServer : Bad inet_pton for target IP for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
}

void CUdpMcastClient::initClient()
{
    CSocket::mSocketFd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (CSocket::mSocketFd < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpMcastClient::initClient : socket init failed for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
    if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpMcastClient::initClient : setsockopt SO_REUSEADDR failed for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
#endif
#if defined(SO_REUSEPORT) && defined(NET_REUSERPORT)
    if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEPORT, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpMcastClient::initClient : setsockopt SO_REUSEPORT failed for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
#endif
    if (::bind(CSocket::mSocketFd, (sockaddr*)mRunningSockAddr.get(), CSocket::cAddrLen) < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpMcastClient::initClient : bind failed for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
    struct ip_mreq mGroup;
    std::memset(&mGroup, 0, cGroupSize);
    std::memcpy(&mGroup.imr_multiaddr.s_addr, &mTargetSockAddr->sin_addr, sizeof(in_addr));
    std::memcpy(&mGroup.imr_interface.s_addr, &mRunningSockAddr->sin_addr, sizeof(in_addr));
    if (::setsockopt(CSocket::mSocketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mGroup, CUdpMcastClient::cGroupSize) < 0)
    {
        int code = ::error_code();
        throw CSocketException(code, "CUdpMcastClient::initClient : adding multicast group failed for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
}
