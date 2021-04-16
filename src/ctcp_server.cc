#include "ctcp_server.h"
#include "iworker_socket.h"

CTcpServer::CTcpServer(const char* ip, uint16_t port) :
    IServer::IServer(ip, port)
{

}

void CTcpServer::initServer() noexcept(false)
{
	CSocket::mSocketInfo->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (CSocket::mSocketInfo->socket < 0)
	{
		throw CSocketException("CTcpServer::initServer : server{%s:%u} socket failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
	CSocket::mSocketInfo->socketAddress->sin_family = AF_INET;
	CSocket::mSocketInfo->socketAddress->sin_port = htons(IMainSocket::mPort);
	inet_pton(AF_INET, IMainSocket::mIp.c_str(), &CSocket::mSocketInfo->socketAddress->sin_addr);
#if defined(SO_REUSEADDR) && defined(NET_REUSEADDR)
	if (setsockopt(CSocket::mSocketInfo->socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0)
	{
		throw CSocketException("CTcpServer::initServer : server{%s:%u} setsockopt SO_REUSEADDR failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
#endif
#if defined(SO_REUSEPORT) && defined(NET_REUSERPORT)
    if (setsockopt(CSocket::mSocketInfo->socket, SOL_SOCKET, SO_REUSEPORT, (const char*)&IMainSocket::ReuseFlag, IMainSocket::cReuseSize) < 0) 
    {
		throw CSocketException("CTcpServer::initServer : server{%s:%u} setsockopt SO_REUSEPORT failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
#endif
    if (bind(CSocket::mSocketInfo->socket, (sockaddr*)CSocket::mSocketInfo->socketAddress.get(), CSocket::AddrLen) < 0)
	{
		throw CSocketException("CTcpServer::initServer : server{%s:%u} bind failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
	if (listen(CSocket::mSocketInfo->socket, SOMAXCONN) < 0)
	{
		throw CSocketException("CTcpServer::initServer : server{%s:%u} listen failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
}

void CTcpServer::acceptClient(std::unique_ptr<SocketInfo>& client_info) noexcept(false)
{
	if(nullptr == client_info)
	{
		throw CSocketException("CTcpServer::acceptClient : server{%s:%u} client_addr is nullptr", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
	acceptClient(client_info.get());
}

void CTcpServer::acceptClient(SocketInfo* client_info) noexcept(false)
{
	if(nullptr == client_info)
	{
		throw CSocketException("CTcpServer::acceptClient : server{%s:%u} client_addr is nullptr", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
	else if(nullptr == client_info->socketAddress)
	{
		client_info->socketAddress = std::make_unique<struct sockaddr_in>();
	}
	memset(client_info->socketAddress.get(), 0, CSocket::AddrLen);
	client_info->socket = accept(CSocket::mSocketInfo->socket, (sockaddr*)client_info->socketAddress.get(), (socklen_t*)&CSocket::AddrLen);
	if(INVALID_SOCKET == client_info->socket)
	{
		throw CSocketException("CTcpServer::acceptClient : server{%s:%u} accept failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, strerror(errno));
	}
}

void CTcpServer::addClient(std::unique_ptr<CSocket> client_socket) noexcept
{
	mClientSockets.push_back(std::move(client_socket));
}

const CSocket* CTcpServer::getClient(uint8_t idx) const noexcept(false) 
{ 
	return mClientSockets.at(idx).get(); 
}

const CSocket* CTcpServer::getLastClient() const noexcept(false) 
{ 
	return mClientSockets.back().get(); 
}
