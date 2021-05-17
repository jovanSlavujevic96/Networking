#include <algorithm>    // std::find
#include <random>
#include <string.h>
#include <mutex>

#include "socket_utils.h"
#include "cmulticast_ip_generator.h"
#include "cport_generator.h"

#define MULTICAST_MAX_IPV4 0xE8FFFFFF
#define MULTICAST_MIN_IPV4 0xE8000100

static std::random_device rd;
static std::vector<std::string>::iterator it_str;
static std::vector<uint16_t>::iterator it_num;
static std::mutex mutex;

static struct Instantiator
{
	CMutlicastIpGenerator multicast_generator;
	CPortGenerator port_generator;
}_instantiator;

CMutlicastIpGenerator& CMutlicastIpGenerator::instance()
{
	return _instantiator.multicast_generator;
}

const std::string& CMutlicastIpGenerator::getAddr()
{
	static struct in_addr addr = { 0 };
	static constexpr uint32_t range = MULTICAST_MAX_IPV4 - MULTICAST_MIN_IPV4;
	static const std::string empty_string = "";

	if (mMulticastIp.size() == range)
	{
		/* impossible usecase */
		return empty_string;
	}

	std::lock_guard<std::mutex> lock(mutex);
	std::string addr_str;
	
	while(true)
	{
		addr.S_un.S_addr = htonl(MULTICAST_MIN_IPV4 + (rd()) % range);
		addr_str = inet_ntoa(addr);

		it_str = std::find(mMulticastIp.begin(), mMulticastIp.end(), addr_str);
		if (it_str != mMulticastIp.end())
		{
			addr_str.clear();
		}
		else 
		{
			mMulticastIp.push_back(addr_str);
			break;
		}
	}
	return mMulticastIp.back();
}

void CMutlicastIpGenerator::releaseAddr(const char* addr)
{
	std::lock_guard<std::mutex> lock(mutex);
	uint16_t iterator = 0;
	for (auto& ip : mMulticastIp)
	{
		if (!strcmp(addr, ip.c_str()))
		{
			mMulticastIp.erase(mMulticastIp.begin() + iterator);
			break;
		}
		iterator++;
	}
}

void CMutlicastIpGenerator::releaseAddr(const std::string& addr)
{
	std::lock_guard<std::mutex> lock(mutex);
	uint16_t iterator = 0;
	for (auto& ip : mMulticastIp)
	{
		if (addr == ip)
		{
			mMulticastIp.erase(mMulticastIp.begin() + iterator);
			break;
		}
		iterator++;
	}
}

CPortGenerator& CPortGenerator::instance()
{
	return _instantiator.port_generator;
}

uint16_t CPortGenerator::getPort()
{
	if (mPorts.size() == USHRT_MAX)
	{
		/* almost impossible case */
		return 0;
	}

	std::lock_guard<std::mutex> lock(mutex);
	uint16_t port;

	while (true)
	{
		port = rd() & 0xfffe;
		it_num = std::find(mPorts.begin(), mPorts.end(), port);
		if (it_num == mPorts.end())
		{
			mPorts.push_back(port);
			break;
		}
	}
	return port;
}

void CPortGenerator::addPort(uint16_t port)
{
	std::lock_guard<std::mutex> lock(mutex);
	it_num = std::find(mPorts.begin(), mPorts.end(), port);
	if (it_num != mPorts.end())
	{
		mPorts.push_back(port);
	}
}

void CPortGenerator::releasePort(uint16_t port)
{
	std::lock_guard<std::mutex> lock(mutex);
	uint16_t iterator = 0;
	for (const auto& _port : mPorts)
	{
		if (_port == port)
		{
			mPorts.erase(mPorts.begin() + iterator);
			break;
		}
		iterator++;
	}
}

bool CPortGenerator::isPortFree(uint16_t port)
{
	std::lock_guard<std::mutex> lock(mutex);
	it_num = std::find(mPorts.begin(), mPorts.end(), port);
	return (it_num != mPorts.end());
}
