#include "cudp_mcast_server.h"

CUdpMcastServer::CUdpMcastServer(const char* ip, uint16_t port) :
    IServer::IServer(ip, port)
{

}

void CUdpMcastServer::initServer()
{
	CSocket::mSocketInfo->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (CSocket::mSocketInfo->socket < 0)
	{
		throw CSocketException("CUdpMcastServer::initServer : server{%s:%u} socket failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
	CSocket::mSocketInfo->socketAddress->sin_family = AF_INET;
	CSocket::mSocketInfo->socketAddress->sin_port = htons(IMainSocket::mPort);
	inet_pton(AF_INET, IMainSocket::mIp.c_str(), &CSocket::mSocketInfo->socketAddress->sin_addr);
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
	if (setsockopt(CSocket::mSocketInfo->socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		throw CSocketException("CUdpMcastServer::initServer : server{%s:%u} setsockopt SO_REUSEADDR failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
#endif
	struct in_addr localInterface;
	memset(&localInterface, 0, sizeof(localInterface));
	if (setsockopt(CSocket::mSocketInfo->socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, CUdpMcastServer::cInAddr) < 0)
	{
		throw CSocketException("CUdpMcastServer::initServer : server{%s:%u} setsockopt multicast failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
	IMainSocket::mInitDone = true;
}
