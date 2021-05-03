#pragma once

#include <string>
#include <cstdint>

const constexpr uint16_t DEFAULT_RTSP_PORT = 554u;
#define DEFAULT_RTSP_VERSION 1.0f
#define MAX_IP_STR_LEN 64u
#define MAX_SUFFIX_STR_LEN 64u
#define MAX_AUTHENTICATION_STR_LEN 32u

enum class RtspTransport
{
	RTP_OVER_TCP,
	RTP_OVER_UDP,
	RTP_OVER_MULTICAST
};

struct RtspInfo
{
	char ip[MAX_IP_STR_LEN];
	uint16_t port;
	char suffix[MAX_SUFFIX_STR_LEN];
	float rtspVersion;
	uint16_t currentSeq;
	char authenticationResponse[MAX_AUTHENTICATION_STR_LEN];
	RtspTransport transport;
	uint16_t rtpPort;
	uint16_t rtcpPort;
	uint8_t channel;
	uint32_t mediaSessionId;
};
