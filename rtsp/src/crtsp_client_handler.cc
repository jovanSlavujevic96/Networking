#include <iostream>
#include <string.h>
#include <memory.h>
#include <mutex>
#include <ctime>

#include "crtsp_client_handler.h"
#include "crtsp_master.h"
#include "crtsp_request_package.h"
#include "crtsp_media_session.h"

#define KEY_CRLF "\r\n"
#define KEY_CRLFCRLF KEY_CRLF KEY_CRLF
#define LINE_LEN 100u
#define ONE_KB 1024u

/** @brief buffer size is 10 kB for receiving of  */
static constexpr uint16_t BUFFER_SIZE = ONE_KB * 10;
extern std::hash<std::string> hash_generator;

CRtspClientHandler::CRtspClientHandler(std::unique_ptr<SocketInfo> socket_info) :
	IWorkerSocket::IWorkerSocket(std::move(socket_info)),
	mRequestState{RtspRequest::NONE},
	mRtspInfo{0},
	mPkg{BUFFER_SIZE},
	mMediaSession{NULL}
{
	mRtspInfo.suffix[MAX_SUFFIX_STR_LEN - 1] = '\0';
	mRtspInfo.ip[MAX_IP_STR_LEN - 1] = '\0';
}

CRtspClientHandler::~CRtspClientHandler()
{

}

std::unique_ptr<CRtspClientHandler> CRtspClientHandler::instatiateClientHandler(CRtspMaster* master, std::unique_ptr<SocketInfo>& socket_info) noexcept
{
	std::unique_ptr<CRtspClientHandler> handler = std::unique_ptr<CRtspClientHandler>(new CRtspClientHandler(std::move(socket_info)));
	handler->mMaster = master;
	return handler;
}

void CRtspClientHandler::threadEntry()
{
	/* Handle RTCP/RTSP communication */
	CSocket& this_ = *this;
	int32_t ret;
	bool bRun = true;
	while (bRun)
	{
		try
		{
			ret = this_ >> &mPkg;
			mPkg[(uint16_t)ret] = '\0';

#if defined(_DEBUG)
			std::cout << "REQ: \n" << mPkg.cData() << std::endl;
#endif
			if (!onReceive(mPkg.cData()))
			{
#if defined(_DEBUG)
				std::cout << "onReceive() failed\n";
#endif
				break;
			}
			else if(!onSend() )
			{
#if defined(_DEBUG)
				std::cout << "onSend() failed\n";
#endif
				bRun = false;
			}
#if defined(_DEBUG)
			std::cout << "RES: \n" << mPkg.cData() << std::endl;
#endif
			this_ << &mPkg;
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			break;
		}
		
	}
#if defined(_DEBUG)
	std::cout << "BREAK\n";
#endif
	mMaster->eraseRtspClient(this);
}

bool CRtspClientHandler::onReceive(const char* message)
{
	char* endOfRequestLine = (char*)std::strstr(message, KEY_CRLF);
	char* enfOfHeadersLine = (char*)std::strstr(message, KEY_CRLFCRLF);
	if (!endOfRequestLine || !enfOfHeadersLine)
	{
		return false;
	}
	*endOfRequestLine = '\0';
	*enfOfHeadersLine = '\0';

	bool ret = parseRequestLine(message);
	if (ret)
	{
		ret = parseHeaderLines(endOfRequestLine+2);
	}

#if defined(_DEBUG)
	std::cout << "----------------------------------------\n";
	std::cout << "ip            = " << mRtspInfo.ip << std::endl;
	std::cout << "port          = " << std::dec << mRtspInfo.port << std::endl;
	std::cout << "suffix        = " << mRtspInfo.suffix << std::endl;
	std::cout << "version       = " << std::defaultfloat << mRtspInfo.rtspVersion << std::endl;
	std::cout << "cseq          = " << std::dec << mRtspInfo.currentSeq << std::endl;
	std::cout << "auth          = " << mRtspInfo.authenticationResponse << std::endl;
	std::cout << "transport     = " << std::dec << (int)mRtspInfo.transport << std::endl;
	std::cout << "rtp port      = " << std::dec << mRtspInfo.rtpPort << std::endl;
	std::cout << "rtcp port     = " << std::dec << mRtspInfo.rtcpPort << std::endl;
	std::cout << "channel       = " << std::dec << (int)mRtspInfo.channel << std::endl;
	std::cout << "media ssesion = " << std::dec << mRtspInfo.mediaSessionId << std::endl << std::endl;

	std::cout << "ret           = " << std::boolalpha << ret << std::endl;
	std::cout << "----------------------------------------\n\n\n";
#endif
	return ret;
}

bool CRtspClientHandler::parseRequestLine(const char* line)
{
	char* request_end = (char*)std::strstr(line, " "), * url_end = NULL;
	if (request_end)
	{
		url_end = (char*)std::strstr(request_end + 1, " ");
	}
	if (!request_end || !url_end)
	{
		return false;
	}
	*request_end = '\0';
	*url_end = '\0';

	/* parse RTSP request/method */
	bool ret = parseRequest(line);
	
	/* parse RTSP URL */
	if (ret)
	{
		ret = parseUrl(request_end+1);
	}

	/* parse RTSP version */
	if (ret)
	{
		ret = parseVersion(url_end+1);
	}
	return ret;
}

bool CRtspClientHandler::parseRequest(const char* request)
{
	try
	{
		mRequestState = cRequestMap.at(request);
	}
	catch (...)
	{
		std::cerr << "CRtspClientHandler::parseRequestLine : '" << request << "' RTSP method/request not found\n";
		mRequestState = RtspRequest::NONE;
		return false;
	}
	return true;
}

bool CRtspClientHandler::parseUrl(const char* url)
{
	if (std::strncmp(url, "rtsp://", 7) != 0)
	{
		/* rtsp url must include 'rtsp://' at the begining  */
		return false;
	}
#if defined(__linux) || defined(__linux__)
	else if (std::sscanf(url + 7, "%[^:]:%hu/%s", mRtspInfo.ip, &mRtspInfo.port, mRtspInfo.suffix) == 3)
#elif defined(WIN32) || defined(_WIN32)
	else if (::sscanf_s(url + 7, "%[^:]:%hu/%s", mRtspInfo.ip, MAX_IP_STR_LEN, &mRtspInfo.port, mRtspInfo.suffix, MAX_SUFFIX_STR_LEN) == 3)
#endif		
	{
		/* do nothing */
	}
#if defined(__linux) || defined(__linux__)
	else if (std::sscanf(url + 7, "%[^/]/%s", mRtspInfo.ip, mRtspInfo.suffix) == 3)
#elif defined(WIN32) || defined(_WIN32)
	else if (::sscanf_s(url + 7, "%[^/]/%s", mRtspInfo.ip, MAX_IP_STR_LEN, mRtspInfo.suffix, MAX_SUFFIX_STR_LEN) == 3)
#endif	
	{
		mRtspInfo.port = DEFAULT_RTSP_PORT;
	}
	else
	{
		return false;
	}
	return true;
}

bool CRtspClientHandler::parseVersion(const char* version)
{
	if (std::strncmp(version, "RTSP/", 5) != 0)
	{
		/* mandator part of version */
		return false;
	}
#if defined(__linux) || defined(__linux__)
	return (std::sscanf(version + 5, "%f", &mRtspInfo.rtspVersion) == 1);
#elif defined(WIN32) || defined(_WIN32)
	return (::sscanf_s(version + 5, "%f", &mRtspInfo.rtspVersion) == 1);
#endif
}

bool CRtspClientHandler::parseHeaderLines(const char* lines)
{
	/* parse RTSP CSeq */
	bool ret = parseCSeq(lines);

	/* parse Authentication/Authorization */
	if (ret && mMaster->getAuthentication())
	{
		if ((RtspRequest::DESCRIBE == mRequestState) ||
			(RtspRequest::SETUP == mRequestState) ||
			(RtspRequest::PLAY == mRequestState))
		{
			ret = parseAuthentication(lines);
		}
	}
	if (ret)
	{
		switch (mRequestState)
		{
			case RtspRequest::DESCRIBE:
			{
				ret = parseAccept(lines);
				break;
			}
			case RtspRequest::SETUP:
			{
				ret = parseTransport(lines);
				if (ret)
				{
					ret = parseMediaChannel(lines);
				}
				break;
			}
			case RtspRequest::PLAY:
			{
				ret = parseSessionId(lines);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	return ret;
}

bool CRtspClientHandler::parseCSeq(const char* lines)
{
	const char* helper_pointer = helper_pointer = std::strstr(lines, "CSeq");
	if (helper_pointer)
	{
#if defined(__linux) || defined(__linux__)
		return (std::sscanf(helper_pointer, "%*[^:]: %hu", &mRtspInfo.currentSeq) == 1);
#elif defined(WIN32) || defined(_WIN32)
		return (::sscanf_s(helper_pointer, "%*[^:]: %hu", &mRtspInfo.currentSeq) == 1);
#endif
	}
	return false;
}

bool CRtspClientHandler::parseAuthentication(const char* lines)
{
	if ((std::strstr(lines, "Authorization")) ||
		(std::strstr(lines, "Authentication")))
	{
		const char* response = std::strstr(lines, "response=");
		if (response && strlen(response) >= 42)
		{
			memcpy(mRtspInfo.authenticationResponse, response + 10, 32);
			return true;
		}
	}
	return false;
}

bool CRtspClientHandler::parseAccept(const char* lines)
{
	return (std::strstr(lines, "Accept") &&
			std::strstr(lines, "sdp") );
}

bool CRtspClientHandler::parseTransport(const char* lines)
{
	const char* line_ptr = std::strstr(lines, "Transport");
	const char* helper_ptr = NULL;

	if (line_ptr)
	{
		if ((helper_ptr = std::strstr(line_ptr, "RTP/AVP/TCP")) != NULL)
		{
			mRtspInfo.transport = RtspTransport::RTP_OVER_TCP;
#if defined(__linux) || defined(__linux__)
			return (std::sscanf(helper_ptr, "%*[^;];%*[^;];%*[^=]=%hu-%hu", &mRtspInfo.rtpPort, &mRtspInfo.rtcpPort) == 2);
#elif defined(WIN32) || defined(_WIN32)
			return (::sscanf_s(helper_ptr, "%*[^;];%*[^;];%*[^=]=%hu-%hu", &mRtspInfo.rtpPort, &mRtspInfo.rtcpPort) == 2);
#endif 
		}
		else if ((helper_ptr = std::strstr(line_ptr, "RTP/AVP")) != NULL)
		{
			if (std::strstr(line_ptr, "unicast"))
			{
				mRtspInfo.transport = RtspTransport::RTP_OVER_UDP;
#if defined(__linux) || defined(__linux__)
				return (std::sscanf(helper_ptr, "%*[^;];%*[^;];%*[^=]=%hu-%hu", &mRtspInfo.rtpPort, &mRtspInfo.rtcpPort) == 2);
#elif defined(WIN32) || defined(_WIN32)
				return (::sscanf_s(helper_ptr, "%*[^;];%*[^;];%*[^=]=%hu-%hu", &mRtspInfo.rtpPort, &mRtspInfo.rtcpPort) == 2);
#endif 
			}
			else if (std::strstr(line_ptr, "multicast"))
			{
				mRtspInfo.transport = RtspTransport::RTP_OVER_MULTICAST;
				return true;
			}
		}
	}
	return false;
}

bool CRtspClientHandler::parseMediaChannel(const char* lines)
{
	mRtspInfo.channel = 0;
	if (std::strstr(lines, "url"))
	{
		if (std::strstr(lines, "track1"))
		{
			mRtspInfo.channel = 1;
		}
	}
	return true;
}

bool CRtspClientHandler::parseSessionId(const char* lines)
{
	const char* session = std::strstr(lines, "Session");
	if(session)
	{
#if defined(__linux) || defined(__linux__)
		return (std::sscanf(session, "%*[^:]: %u", &mRtspInfo.mediaSessionId) == 1);
#elif defined(WIN32) || defined(_WIN32)
		return (::sscanf_s(session, "%*[^:]: %u", &mRtspInfo.mediaSessionId) == 1);
#endif
	}
	return false;
}

bool CRtspClientHandler::onSend()
{
	mPkg.clear();	/* clear package */
	switch (mRequestState)
	{
		case RtspRequest::OPTIONS:
		{
			return handleCmdOption();
		}
		case RtspRequest::DESCRIBE:
		{
			return handleCmdDescribe();
		}
		case RtspRequest::SETUP:
		{
			return handleCmdSetup();
		}
		case RtspRequest::PLAY:
		{
			return handleCmdPlay();
		}
		case RtspRequest::TEARDOWN:
		{
			return handleCmdTeardown();
		}
		case RtspRequest::GET_PARAMETER:
		{
			return handleCmdGetParameter();
		}
		default:
		{
			break;
		}
	}
	return false;
}

bool CRtspClientHandler::handleCmdOption()
{
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 200 OK\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n"
		<< "Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY\r\n\r\n";
	return true;
}

bool CRtspClientHandler::handleCmdDescribe()
{
	if (!mMediaSession)
	{
		uint64_t suffix_hash = ::hash_generator(mRtspInfo.suffix);
		mMediaSession = mMaster->getMediaSession(suffix_hash);
		if (!mMediaSession)
		{
			goto stream_not_found;
		}
	}

	if (!mMediaSession->getMulticast())
	{
		/* create udp unicast server for new rtsp client */
		if (!mMediaSession->addClient(this, mRtspInfo.rtpPort))
		{
			goto server_error;
		}
	}

	mPkg << "v=0\r\n"
		<< "o=- 9" << std::time(NULL) << "1 IN IP4 " << mRtspInfo.ip << "\r\n"
		<< "t=0 0\r\n"
		<< "a=control:*\r\n";

	if (mMediaSession->getMulticast())
	{
		mPkg << "a=type:broadcast\r\n"
			<< "a=rtcp-unicast: reflection\r\n";
	}

	mPkg << "m=video " << mMediaSession->getPort(this) << " RTP/AVP 96\r\n";

	if (mMediaSession->getMulticast())
	{
		mPkg << "c=IN INP4 " << mMediaSession->getIp() << "/255\r\n";
	}

	mPkg << "rtpmap:96 H264/90000\r\n"
		<< "a=control:track0\r\n";

	uint16_t sdp_len = mPkg.getCurrentSize();

	mPkg >> "Content-Type: application/sdp\r\n\r\n"
		>> "\r\n" >> sdp_len >> "Content-Length: "
		>> "\r\n" >> mRtspInfo.currentSeq >> "CSeq: "
		>> " 200 OK\r\n" >> mRtspInfo.rtspVersion >> "RTSP/";
	return true;

server_error:
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 500 Internal Server Error\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n\r\n";
	return false;

stream_not_found:
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 404 Stream Not Found\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n\r\n";
	return false;
}

bool CRtspClientHandler::handleCmdSetup()
{
	if (!mMediaSession)
	{
		uint64_t suffix_hash = ::hash_generator(mRtspInfo.suffix);
		mMediaSession = mMaster->getMediaSession(suffix_hash);
		if (!mMediaSession)
		{
			goto stream_not_found;
		}
	}

	if (mMediaSession->getMulticast())
	{
		if (mRtspInfo.transport != RtspTransport::RTP_OVER_MULTICAST)
		{
			goto transport_unsupport;
		}
		/* mutlicast streaming client */
		mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 200 OK\r\n"
			<< "CSeq: " << mRtspInfo.currentSeq << "\r\n"
			<< "Transport: RTP/AVP;multicast;destination=" << mMediaSession->getIp() << ";source=" << mRtspInfo.ip << ";port=" << mMediaSession->getPort() << "-0;ttl=255\r\n"
			<< "Session: " << (uint64_t)mMediaSession /*media session address*/ << "\r\n\r\n";
	}
	else
	{
		if (mRtspInfo.transport != RtspTransport::RTP_OVER_UDP)
		{
			goto transport_unsupport;
		}
		/* create new udp unicast server for new rtsp client if it's not created */
		else if (!mMediaSession->addClient(this, mRtspInfo.rtpPort))
		{
			goto server_error;
		}
		/* udp unicast streaming client */
		mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 200 OK\r\n"
			<< "CSeq: " << mRtspInfo.currentSeq << "\r\n"
			<< "Transport: RTP/AVP;unicast;client_port=" << mRtspInfo.rtpPort << '-' << mRtspInfo.rtpPort << ";server_port=" << mMediaSession->getPort(this) << "-0\r\n"
			<< "Session: " << (uint64_t)mMediaSession /*media session address*/ << "\r\n\r\n";
	}
	return true;

server_error:
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 500 Internal Server Error\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n\r\n";
	return false;

transport_unsupport:
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 461 Unsupported transport\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n\r\n";
	return false;

stream_not_found:
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 404 Stream Not Found\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n\r\n";
	return false;
}

bool CRtspClientHandler::handleCmdPlay()
{
	if (!mMediaSession)
	{
		return false;
	}
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << " 200 OK\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n"
		<< "Range: npt=0.000-\r\n"
		<< "Session: " << (uint64_t)mMediaSession << "; timeout=60\r\n\r\n";
	return true;
}

bool CRtspClientHandler::handleCmdTeardown()
{
	if (!mMediaSession)
	{
		return false;
	}
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << "200 OK\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n"
		<< "Session: " << (uint64_t)mMediaSession << "\r\n\r\n";
	return true;
}

bool CRtspClientHandler::handleCmdGetParameter()
{
	if (!mMediaSession)
	{
		return false;
	}
	mPkg << "RTSP/" << mRtspInfo.rtspVersion << "200 OK\r\n"
		<< "CSeq: " << mRtspInfo.currentSeq << "\r\n"
		<< "Session: " << (uint64_t)mMediaSession << "\r\n\r\n";
	return true;
}
