#include "cudp_server.h"

CUdpServer::CUdpServer(const char* running_ip, uint16_t running_port, const char* target_ip, uint16_t target_port) noexcept(false)
{
	if (nullptr == CSocket::mRunningSockAddr)
	{
		CSocket::mRunningSockAddr = std::make_unique<sockaddr_in>();
	}
	std::memset(CSocket::mRunningSockAddr.get(), 0, CSocket::cAddrLen);
	CSocket::mRunningSockAddr->sin_family = AF_INET;
	CSocket::mRunningSockAddr->sin_port = ::htons(running_port);
	if (::inet_pton(AF_INET, running_ip, &CSocket::mRunningSockAddr->sin_addr) <= 0)
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
	CSocket::mTargetSockAddr->sin_port = ::htons(target_port);
	if (::inet_pton(AF_INET, target_ip, &CSocket::mTargetSockAddr->sin_addr) <= 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpServer::CUdpServer : Bad inet_pton for target IP for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}

void CUdpServer::initServer()
{
    CSocket::mSocketFd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (CSocket::mSocketFd < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpServer::initServer : socket init failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
	if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpServer::initServer : setsockopt SO_REUSEADDR failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#endif
#if defined(SO_REUSEPORT) && defined(NET_REUSERPORT)
	if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEPORT, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpServer::initServer : setsockopt SO_REUSEPORT failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#endif
	if (::bind(CSocket::mSocketFd, (sockaddr*)mRunningSockAddr.get(), CSocket::AddrLen) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CUdpServer::initServer : bind failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}
