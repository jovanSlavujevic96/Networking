#pragma once

#include <cstdint>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

typedef int32_t SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
