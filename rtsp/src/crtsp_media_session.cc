#include "cudp_mcast_server.h"
#include "cudp_server.h"
#include "cudp_server.h"
#include "cmulticast_ip_generator.h"
#include "cport_generator.h"
#include "crtsp_media_session.h"

std::unique_ptr<CRtspMediaSession> CRtspMediaSession::instatiateMediaSession(const char* suffix, bool multicast)
{
	std::unique_ptr<CRtspMediaSession> mediaSession = std::unique_ptr<CRtspMediaSession>(new CRtspMediaSession(suffix, multicast));
	if (multicast)
	{
		std::unique_ptr<CUdpMcastServer> multicast_server;
		uint16_t mcastPort;
		while (true)
		{
			const std::string& mcastIp = CMutlicastIpGenerator::instance().getAddr();
			mcastPort = CPortGenerator::instance().getPort();
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
		mediaSession->mClientStreamerMap.insert({ (CRtspClientHandler*)NULL, std::move(multicast_server) });
	}
	return mediaSession;
}

CRtspMediaSession::CRtspMediaSession(const char* suffix, bool multicast) :
	mSuffix{ suffix },
	mIsMulticast{ multicast }
{

}

bool CRtspMediaSession::getMulticast() const
{
	return mIsMulticast;
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

bool CRtspMediaSession::addClient(CRtspClientHandler* client)
{
	if (!mIsMulticast)
	{
		try
		{
			/* check does client already exists */
			(void)mClientStreamerMap.at(client);
		}
		catch (...)
		{
			/* create client with random port */
			std::unique_ptr<CUdpServer> unicast_server;
			uint16_t ucastPort;
			while (true)
			{
				ucastPort = CPortGenerator::instance().getPort();
				unicast_server = std::make_unique<CUdpServer>("0.0.0.0" /*Address any*/, ucastPort);
				try
				{
					unicast_server->initServer();
				}
				catch (...)
				{
					continue;
				}
				break;
			}
			mClientStreamerMap.insert({ client, std::move(unicast_server) });
		}
	}
	return true;
}

bool CRtspMediaSession::addClient(CRtspClientHandler* client, uint16_t port)
{
	if (!mIsMulticast)
	{
		try
		{
			/* check does client already exists */
			(void)mClientStreamerMap.at(client);
		}
		catch (...)
		{
			if (!CPortGenerator::instance().isPortFree(port))
			{
				return false;
			}
			/* create client with random port */
			std::unique_ptr<CUdpServer> unicast_server;
			unicast_server = std::make_unique<CUdpServer>("0.0.0.0" /*Address any*/, port);
			try
			{
				unicast_server->initServer();
				CPortGenerator::instance().addPort(port);
			}
			catch (...)
			{
				return false;
			}
			mClientStreamerMap.insert({ client, std::move(unicast_server) });
		}
	}
	return true;
}
