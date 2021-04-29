#include "ctcp_client.h"

CTcpClient::CTcpClient(const char* ip, uint16_t port) :
    IClient::IClient(ip, port)
{

}

void CTcpClient::initClient() noexcept(false)
{
    CSocket::mSocketInfo->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(CSocket::mSocketInfo->socket < 0)
    {
        throw CSocketException("CTcpClient::initClient : client{%s:%u} socket failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
    }
    CSocket::mSocketInfo->socketAddress->sin_family = AF_INET;
	CSocket::mSocketInfo->socketAddress->sin_port = htons(IMainSocket::mPort);
	inet_pton(AF_INET, IMainSocket::mIp.c_str(), &CSocket::mSocketInfo->socketAddress->sin_addr);
    if (connect(CSocket::mSocketInfo->socket, (sockaddr*)CSocket::mSocketInfo->socketAddress.get(), CSocket::AddrLen) < 0)
	{
		throw CSocketException("CTcpClient::initClient : client{%s:%u} connect failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
	}
}
