#include "cudp_mcast_client.h"

CUdpMcastClient::CUdpMcastClient(const char* ip, uint16_t port) :
    IClient::IClient(ip, port)
{

}

void CUdpMcastClient::initClient()
{
    CSocket::mSocketInfo->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (CSocket::mSocketInfo->socket < 0)
    {
        throw CSocketException("CUdpMcastClient::initClient : client{%s:%u} socket failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
    }
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
    if (setsockopt(CSocket::mSocketInfo->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&ReuseFlag, IMainSocket::cReuseSize) < 0)
    {
        throw CSocketException("CUdpMcastClient::initClient : client{%s:%u} setsockopt failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
    }
#endif
    CSocket::mSocketInfo->socketAddress->sin_family = AF_INET;
    CSocket::mSocketInfo->socketAddress->sin_port = htons(IMainSocket::mPort);
    inet_pton(AF_INET, "0.0.0.0", &CSocket::mSocketInfo->socketAddress->sin_addr);
    if (bind(CSocket::mSocketInfo->socket, (struct sockaddr*)CSocket::mSocketInfo->socketAddress.get(), CSocket::AddrLen))
    {
        throw CSocketException("CUdpMcastClient::initClient : client{%s:%u} bind failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
    }
    struct ip_mreq mGroup;
    memset(&mGroup, 0, cGroupSize);
    mGroup.imr_multiaddr.s_addr = inet_addr(IMainSocket::mIp.c_str());
    mGroup.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(CSocket::mSocketInfo->socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mGroup, CUdpMcastClient::cGroupSize) < 0)
    {
        throw CSocketException("CUdpMcastClient::initClient : client{%s:%u} adding multicast group failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
    }
    IMainSocket::mInitDone = true;
}
