#include "cudp_mcast_server.h"
#include "cudp_server.h"
#include "cmulticast_ip_generator.h"
#include "cport_generator.h"
#include "crtsp_media_session.h"

std::unique_ptr<CRtspMediaSession> instatiateMediaSession(const char* suffix, bool multicast)
{
	return std::make_unique<CRtspMediaSession>(suffix, multicast);
}

CRtspMediaSession::CRtspMediaSession(const char* suffix, bool multicast) :
	mSuffix{ suffix },
	mIsMulticast{ multicast }
{

}

void CRtspMediaSession::setMulticast(bool multicast)
{
	if (mIsMulticast && !mClientStreamerMap.empty())
	{
		/* session has already started / ignoring request */
		return;
	}
	mIsMulticast = multicast;
}

bool CRtspMediaSession::getMulticast() const
{
	return mIsMulticast;
}

void CRtspMediaSession::startSession()
{
	if (mIsMulticast && mClientStreamerMap.empty())
	{
		std::unique_ptr<CUdpMcastServer> multicast_server;
		while (true)
		{
			const std::string& mcastIp = CMutlicastIpGenerator::instance().getAddr();
			uint16_t mcastPort = CPortGenerator::instance().getPort();
			multicast_server = std::make_unique<CUdpMcastServer>(mcastIp.c_str(), mcastPort);
			try
			{
				multicast_server->initServer();
			}
			catch (...)
			{
				continue;
			}
			break;
		}
		mClientStreamerMap.insert({(CRtspClientHandler*)NULL, std::move(multicast_server)});
	}
}

const std::string& CRtspMediaSession::getIp() const noexcept(false)
{
	return mClientStreamerMap.begin()->second->getIp(); /* any */
}

uint16_t CRtspMediaSession::getPort(CRtspClientHandler* client) const noexcept(false)
{
	if (mIsMulticast && client)
	{
		client = NULL;
	}
	return mClientStreamerMap.at(client)->getPort();
}


const std::string& CRtspMediaSession::getSuffix() const
{
	return mSuffix;
}
