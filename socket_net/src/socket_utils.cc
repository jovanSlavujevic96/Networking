#include "socket_utils.h"

#if defined(WIN32) || defined(_WIN32)

int close(SOCKET socket)
{
    return closesocket(socket);
}

#endif

const char* error_message()
{
#if defined(__linux) || defined(__linux__)
    const char* error = strerror(errno);
#elif defined(WIN32) || defined(_WIN32)
#define ERROR_BUFFER_SIZE 256
    static char error[ERROR_BUFFER_SIZE];
    memset(error, 0, ERROR_BUFFER_SIZE);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
        NULL,               // lpsource
        WSAGetLastError(),  // message id
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // languageid
        error,              // output buffer
        ERROR_BUFFER_SIZE,  // size of msgbuf, bytes
        NULL);              // va_list of arguments
#endif
    return error;
}