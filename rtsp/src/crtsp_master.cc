#include <iostream>

#include "crtsp_client_handler.h"
#include "crtsp_master.h"

std::hash<std::string> hash_generator;

CRtspMaster::CRtspMaster(const char* rtsp_ip, uint16_t rtsp_port, bool authentication) :
	CTcpServer{rtsp_ip, rtsp_port},
    mAuthentication{authentication}
{
	
}

_NODISCARD uint64_t CRtspMaster::addSession(std::unique_ptr<CRtspMediaSession> session) noexcept
{
    uint64_t suffix_hash = ::hash_generator(session->getSuffix());
    try
    {
        (void)mSessionHashMap.at(suffix_hash);
    }
    catch (...)
    {
        mSessionHashMap.insert( { suffix_hash, std::move(session) } );
    }
    return suffix_hash;
}

void CRtspMaster::eraseRtspClient(CRtspClientHandler* client)
{
    std::lock_guard<std::mutex> lock(mMutex);
    uint16_t iterator = 0;
    client->closeSocket();
    for (std::unique_ptr<CSocket>& socket_client : CTcpServer::mClientSockets)
    {
        if (socket_client.get() == (CSocket*)client)
        {
            client->detach();
            socket_client.release();
            mClientSockets.erase(mClientSockets.begin() + iterator);
            break;
        }
        iterator++;
    }
}

uint16_t CRtspMaster::getSessionPort(uint64_t session_id, CRtspClientHandler* client) const noexcept(false)
{
    return mSessionHashMap.at(session_id)->getPort(client);
}

const std::string& CRtspMaster::getSessionIp(uint64_t session_id) const noexcept(false)
{
    return mSessionHashMap.at(session_id)->getIp();
}

CRtspMediaSession* CRtspMaster::getMediaSession(uint64_t session_id) const noexcept(false)
{
    return mSessionHashMap.at(session_id).get();
}

void CRtspMaster::initServer() noexcept(false)
{
    CTcpServer::initServer();
}

void CRtspMaster::start() noexcept(false)
{
	if (!CTcpServer::IsInit())
	{
		throw CSocketException("CRtspMaster::start : start of thread failed -> server is not initialized.");
	}
    else if (mSessionHashMap.empty())
    {
        throw CSocketException("CRtspMaster::start : start of thread failed -> there are not any session added.");
    }
	IThread::start();
}

bool CRtspMaster::getAuthentication() const
{
    return mAuthentication;
}

void CRtspMaster::threadEntry()
{
	/* accept new RTSP clients */
    std::unique_ptr<SocketInfo> socket_info = nullptr;
    std::unique_ptr<CRtspClientHandler> rtsp_client = NULL;
    while (true)
    {
        socket_info = std::make_unique<SocketInfo>();
        try
        {
            CTcpServer::acceptClient(socket_info);
        }
        catch (const std::exception& e)
        {
            std::cerr << "CRtspMaster::threadEntry : " << e.what() << '\n';
            break;
        }
        rtsp_client = CRtspClientHandler::instatiateClientHandler(this, socket_info);
        rtsp_client->start();
        CTcpServer::mClientSockets.push_back(std::move(rtsp_client));
    }
}
