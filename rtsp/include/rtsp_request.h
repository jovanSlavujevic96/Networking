#pragma once

#include <unordered_map>
#include <string>

enum class RtspRequest
{
/** @brief An OPTIONS request returns the request types the server will accept.*/
	OPTIONS,

/** @brief A DESCRIBE request includes an RTSP URL (rtsp://...), and includes the presentation description,
 *	typically in Session Description Protocol (SDP) format */
	DESCRIBE,

/** @brief  A SETUP request specifies how a single media stream must be transported.
 *	This must be done before a PLAY request is sent. The request contains the media stream URL and a transport specifier */
	SETUP,

/** @brief A PLAY request will cause one or all media streams to be played */
	PLAY,

/** @brief A PAUSE request temporarily halts one or all media streams, so it can later be resumed with a PLAY request */
	PAUSE,

/** @brief This method initiates recording a range of media data according to the presentation description.
 *	The time stamp reflects start and end time(UTC). If no time range is given, use the start or end time provided in
 *	the presentation description. If the session has already started, commence recording immediately */
	RECORD,

/** @brief The ANNOUNCE method serves two purposes:
 *	C->S, ANNOUNCE posts the description of a presentation or media object identified by the request URL to a server.
 *	S->C, ANNOUNCE updates the session description in real-time. If a new media stream is added to a presentation,
 *	the whole presentation description should be sent again, rather than just the additional components, so that components can be deleted. */
	ANNOUNCE,

/** @brief A TEARDOWN request is used to terminate the session. It stops all media streams and frees all session related data on the server. */
	TEARDOWN,

/** @brief The GET_PARAMETER request retrieves the value of a parameter of a presentation or stream specified in the URI.
 *	The content of the reply and response is left to the implementation.
 *	@example GET_PARAMETER with no entity body may be used to test client or server liveness ("ping").*/
	GET_PARAMETER,

/** @brief The SET_PARAMETER method requests to set the value of a parameter for a presentation or stream specified by the URI. */
	SET_PARAMETER,

/** @brief A REDIRECT request informs the client that it must connect to another server location.
 *	It contains the mandatory header Location, which indicates that the client should issue requests for that URL.
 *	It may contain the parameter Range, which indicates when the redirection takes effect.
 *	If the client wants to continue to send or receive media for this URI, the client MUST issue a TEARDOWN request for the current
 *	session and a SETUP for the new session at the designated host. */
	REDIRECT,

	NONE
};

inline const std::unordered_map<std::string, RtspRequest> cRequestMap = 
{
	{"OPTIONS", RtspRequest::OPTIONS},
	{"DESCRIBE", RtspRequest::DESCRIBE},
	{"SETUP", RtspRequest::SETUP},
	{"PLAY", RtspRequest::PLAY},
	{"PAUSE", RtspRequest::PAUSE},
	{"RECORD", RtspRequest::RECORD},
	{"ANNOUNCE", RtspRequest::ANNOUNCE},
	{"TEARDOWN", RtspRequest::TEARDOWN},
	{"GET_PARAMETER", RtspRequest::GET_PARAMETER},
	{"SET_PARAMETER", RtspRequest::SET_PARAMETER},
	{"REDIRECT", RtspRequest::REDIRECT},
};
