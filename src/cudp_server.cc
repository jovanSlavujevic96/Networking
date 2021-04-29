#include "cudp_server.h"

CUdpServer::CUdpServer(const char* ip, uint16_t port) :
    IServer::IServer(ip, port)
{

}

void CUdpServer::initServer()
{
    CSocket::mSocketInfo->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (CSocket::mSocketInfo->socket < 0)
	{
		throw CSocketException("CUdpServer::initServer : server{%s:%u} socket failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
	CSocket::mSocketInfo->socketAddress->sin_family = AF_INET;
	CSocket::mSocketInfo->socketAddress->sin_port = htons(IMainSocket::mPort);
	inet_pton(AF_INET, IMainSocket::mIp.c_str(), &CSocket::mSocketInfo->socketAddress->sin_addr);
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
	if (setsockopt(CSocket::mSocketInfo->socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		throw CSocketException("CUdpServer::initServer : server{%s:%u} setsockopt SO_REUSEADDR failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
#endif
#if defined(SO_REUSEPORT) && defined(NET_REUSERPORT)
    if (setsockopt(CSocket::mSocketInfo->socket, SOL_SOCKET, SO_REUSEPORT, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0) 
    {
		throw CSocketException("CUdpServer::initServer : server{%s:%u} setsockopt SO_REUSEPORT failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
#endif
    if (bind(CSocket::mSocketInfo->socket, (sockaddr*)CSocket::mSocketInfo->socketAddress.get(), CSocket::AddrLen) < 0)
	{
		throw CSocketException("CUdpServer::initServer : server{%s:%u} bind failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
    memset(CSocket::mSocketInfo->socketAddress.get(), 0, CSocket::AddrLen);
}
