#pragma once

#include <utility>
#include <memory>
#include <vector>
#include <mutex>
#include <unordered_map>

#include "ithread.h"
#include "ctcp_server.h"
#include "crtsp_media_session.h"

class CRtspClientHandler;

class CRtspMaster : 
	public CTcpServer,
	public IThread
{
public:
	CRtspMaster(const char* rtsp_ip, uint16_t rtsp_port, bool authentication=false);
	~CRtspMaster() = default;
	 
	/** @brief add passed session if it doesnt exist already 
	 *	@param session - unique ptr desired session 
	 *	
	 *	@return session id
	 *	@retval ZERO	 - add session failed 
	 *	@retval NON-ZERO - id (hash) of added media session */
	_NODISCARD uint64_t addSession(std::unique_ptr<CRtspMediaSession> session) noexcept;
	void eraseRtspClient(CRtspClientHandler* client);

	uint16_t getSessionPort(uint64_t session_id, CRtspClientHandler* client = NULL) const noexcept(false);
	const std::string& getSessionIp(uint64_t session_id) const noexcept(false);
	CRtspMediaSession* getMediaSession(uint64_t session_id) const noexcept(false);
	
	void initServer() noexcept(false) override final;
	void start() noexcept(false);

	bool getAuthentication() const;

private:
	std::unordered_map<uint64_t, std::unique_ptr<CRtspMediaSession>> mSessionHashMap;
	std::mutex mMutex;
	bool mAuthentication;

	void threadEntry() override final;
};
