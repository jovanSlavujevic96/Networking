#pragma once

#include <cstdint>
#include <string.h>

#if defined(__linux) || defined(__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/route.h>
#include <netdb.h>
#include <net/if.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

typedef int32_t SOCKET;
#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)

#elif defined(WIN32) || defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WINDOWS_IGNORE_PACKING_MISMATCH

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2

int close(SOCKET socket);

#endif

int error_code();
#if defined(__linux) || defined(__linux__)
const char* error_message(int error_code=errno);
#elif defined(WIN32) || defined(_WIN32)
const char* error_message(int error_code=WSAGetLastError());
#endif
