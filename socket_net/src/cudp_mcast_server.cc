#include "cudp_mcast_server.h"

CUdpMcastServer::CUdpMcastServer(const char* mcast_ip, uint16_t mcast_port) noexcept(false)
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
		throw CSocketException(code, "CUdpMcastServer::CUdpMcastServer : Bad inet_pton for running IP for socket %llu -> %s", (size_t)this, ::error_message(code));
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
		throw CSocketException(code, "CUdpMcastServer::CUdpMcastServer : Bad inet_pton for target IP for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}

void CUdpMcastServer::initServer()
{
	CSocket::mSocketFd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (CSocket::mSocketFd < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpMcastServer::initServer : socket init failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
	if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpMcastServer::initServer : setsockopt SO_REUSEADDR failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#endif
#if defined(SO_REUSEPORT) && defined(NET_REUSERPORT)
	if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEPORT, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpMcastServer::initServer : setsockopt SO_REUSEPORT failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#endif
	if (::bind(CSocket::mSocketFd, (sockaddr*)mRunningSockAddr.get(), CSocket::cAddrLen) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpMcastServer::initServer : bind failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
	struct in_addr localInterface = { 0 };
	std::memcpy(&localInterface, &mRunningSockAddr->sin_addr, CUdpMcastServer::cInAddr);
	if (::setsockopt(CSocket::mSocketFd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, CUdpMcastServer::cInAddr) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpMcastServer::initServer : setsockopt multicast failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}
