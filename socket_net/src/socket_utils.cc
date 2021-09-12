#include "socket_utils.h"

#if defined(WIN32) || defined(_WIN32)

int close(SOCKET socket)
{
    return closesocket(socket);
}

#endif

int error_code()
{
#if defined(__linux) || defined(__linux__)
    return errno;
#elif defined(WIN32) || defined(_WIN32)
    return WSAGetLastError();
#endif
}

const char* error_message(int error_code)
{
#if defined(__linux) || defined(__linux__)
    const char* error = strerror(error_code);   // error_code = errno
#elif defined(WIN32) || defined(_WIN32)

#ifndef ERROR_BUFFER_SIZE
#define ERROR_BUFFER_SIZE 256
#endif//ERROR_BUFFER_SIZE

    static char error[ERROR_BUFFER_SIZE];
    memset(error, 0, ERROR_BUFFER_SIZE);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
        NULL,               // lpsource
        error_code,         // message id => error_code = WSAGetLastError()
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // languageid
        error,              // output buffer
        ERROR_BUFFER_SIZE,  // size of msgbuf, bytes
        NULL);              // va_list of arguments
#endif
    return error;
}
