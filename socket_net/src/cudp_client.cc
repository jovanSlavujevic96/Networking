#include "cudp_client.h"

CUdpClient::CUdpClient(const char* ip, uint16_t port) :
    IClient::IClient(ip, port)
{

}

void CUdpClient::initClient() 
{
    CSocket::mSocketInfo->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(CSocket::mSocketInfo->socket < 0)
    {
        throw CSocketException("CUdpClient::initClient : client{%s:%u} socket failed -> %s", IMainSocket::mIp.c_str(), IMainSocket::mPort, error_message());
    }
    std::memset(CSocket::mSocketInfo->socketAddress.get(), 0, sizeof(*CSocket::mSocketInfo->socketAddress.get()));
    CSocket::mSocketInfo->socketAddress->sin_family = AF_INET;
	CSocket::mSocketInfo->socketAddress->sin_port = htons(IMainSocket::mPort);
	inet_pton(AF_INET, IMainSocket::mIp.c_str(), &CSocket::mSocketInfo->socketAddress->sin_addr);
    IMainSocket::mInitDone = true;
}
