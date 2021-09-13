#include "ctcp_server.h"
#include "iworker_socket.h"

namespace
{
	static inline std::unique_ptr<CSocket> allocSocket(SOCKET fd, std::unique_ptr<sockaddr_in> addr)
	{
		return std::make_unique<CSocket>(fd, nullptr, std::move(addr));
	}
}

CTcpServer::CTcpServer(const char* running_ip, uint16_t running_port) noexcept(false)
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
		throw CSocketException(code, "CTcpServer::CTcpServer : Bad inet_pton for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
	mAllocSocketFunction = ::allocSocket;
}

CTcpServer::CTcpServer(uint16_t running_port) noexcept
{
	if (nullptr == CSocket::mRunningSockAddr)
	{
		CSocket::mRunningSockAddr = std::make_unique<sockaddr_in>();
	}
	std::memset(CSocket::mRunningSockAddr.get(), 0, CSocket::cAddrLen);
	CSocket::mRunningSockAddr->sin_family = AF_INET;
	CSocket::mRunningSockAddr->sin_port = ::htons(running_port);
	::inet_pton(AF_INET, "0.0.0.0" /*IPADDR_ANY*/, &CSocket::mRunningSockAddr->sin_addr);
	mAllocSocketFunction = ::allocSocket;
}

void CTcpServer::initServer() noexcept(false)
{
	CSocket::mSocketFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (CSocket::mSocketFd < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CTcpServer::initServer : socket init failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
	if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CTcpServer::initServer : setsockopt SO_REUSEADDR failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#endif
#if defined(SO_REUSEPORT) && defined(NET_REUSERPORT)
    if (::setsockopt(CSocket::mSocketFd, SOL_SOCKET, SO_REUSEPORT, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
    {
		int code = ::error_code();
		throw CSocketException(code, "CTcpServer::initServer : setsockopt SO_REUSEPORT failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
#endif
    if (::bind(CSocket::mSocketFd, (sockaddr*)mRunningSockAddr.get(), CSocket::AddrLen) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CTcpServer::initServer : bind failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
	if (::listen(CSocket::mSocketFd, SOMAXCONN) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CTcpServer::initServer : listen failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}

size_t CTcpServer::acceptClient() noexcept(false)
{
	if (INVALID_SOCKET == CSocket::mSocketFd)
	{
		throw CSocketException("CTcpServer::acceptClient : invalid server fd.");
	}
	std::unique_ptr<sockaddr_in> target_socket_addr = std::make_unique<sockaddr_in>();
	SOCKET target_fd = INVALID_SOCKET;
	std::memset(target_socket_addr.get(), 0, CSocket::AddrLen);
	target_fd = ::accept(CSocket::mSocketFd, (sockaddr*)target_socket_addr.get(), (socklen_t*)&CSocket::AddrLen);
	if (INVALID_SOCKET == target_fd)
	{
		int code = ::error_code();
		throw CSocketException(code, "CTcpServer::acceptClient : accept failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
	mClientSockets.push_back(mAllocSocketFunction(target_fd, std::move(target_socket_addr)));
	return mClientSockets.size()-1;
}

const CSocket* CTcpServer::getClient(size_t idx) const noexcept(false)
{ 
	return mClientSockets.at(idx).get(); 
}

const CSocket* CTcpServer::getLastClient() const noexcept(false)
{ 
	return mClientSockets.back().get(); 
}
