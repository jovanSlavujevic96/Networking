#include "ctcp_client.h"

CTcpClient::CTcpClient(const char* target_ip, uint16_t target_port) noexcept(false)
{
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
		throw CSocketException(code, "CTcpClient::CTcpClient : Bad inet_pton for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}

void CTcpClient::initClient() noexcept(false)
{
    CSocket::mSocketFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(CSocket::mSocketFd < 0)
    {
		int code = ::error_code();
		throw CSocketException(code, "CTcpClient::initClient : socket init failed for socket %llu -> %s", (size_t)this, ::error_message(code));
    }
    if (::connect(CSocket::mSocketFd, (sockaddr*)CSocket::mTargetSockAddr.get(), CSocket::AddrLen) < 0)
	{
		int code = ::error_code();
		throw CSocketException(code, "CTcpClient::initClient : connect failed for socket %llu -> %s", (size_t)this, ::error_message(code));
	}
}
