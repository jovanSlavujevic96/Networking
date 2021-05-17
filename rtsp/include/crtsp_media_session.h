#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "crtsp_client_handler.h"
#include "iserver.h"

class CRtspMediaSession
{
public:
	static std::unique_ptr<CRtspMediaSession> instatiateMediaSession(const char* suffix, bool multicast = true);
	
	CRtspMediaSession(const CRtspMediaSession&) = delete;
	~CRtspMediaSession() = default;

	/** @brief this method checks is selected media session doing multicast or unicast streaming
	 * 
	 *  @return is media session multicast
	 *  @retval true  - multicast streaming session
	 *  @retval false - unicast streaming session 
	 */
	bool getMulticast() const;

	/** @brief ip address getter
	 *
	 *	@exception std::exception for empty mStreamers -> can't call getIp() before startSession() method
	 *
	 *	@return ip address - unicast/multicast 
	 */
	const std::string& getIp() const noexcept(false);

	/** @brief streaming port getter for particular client or multicast port  
	 *	@param client - passed client handler refers to port. Passed NULL also return multicast port 
	 *	
	 *	@exception std::exception for empty mStreamers -> can't call getPort() before startSession() method
	 *
	 *	@return desired port
	 *	@retval ZERO	 - for passed NULL client if session is not in multicast mode/if client doesn't exist in client list
	 *	@retval NON-ZERo - multicast port or separate port for upd unicast client 
	 */
	uint16_t getPort(CRtspClientHandler* client = NULL) const noexcept(false);

	/** @brief suffix getter 
	 *
	 *  @return original (reference) suffix string
	 */
	const std::string& getSuffix() const;

	bool addClient(CRtspClientHandler* client);
	bool addClient(CRtspClientHandler* client, uint16_t port);

private:
	CRtspMediaSession(const char* suffix, bool multicast = true);

	std::unordered_map<CRtspClientHandler*, std::unique_ptr<IServer>> mClientStreamerMap;
	std::string mSuffix;
	bool mIsMulticast;
};
