#include <vector>

#include "socket_utils.h"
#include "csocket_exception.h"

#if defined(WIN32) || defined(_WIN32)

int close(SOCKET socket)
{
    return ::closesocket(socket);
}

#endif

std::string getOwnIpV4Address() noexcept(false)
{
    std::vector<std::string> addresses;
#if defined(WIN32) || defined(_WIN32)
    PMIB_IPADDRTABLE pIPAddrTable = NULL;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    char buffer[100];

    pIPAddrTable = (MIB_IPADDRTABLE*)std::malloc(sizeof(MIB_IPADDRTABLE));
    if (!pIPAddrTable)
    {
        throw std::bad_alloc();
    }
    if (GetIpAddrTable(pIPAddrTable, &dwSize, true) == ERROR_INSUFFICIENT_BUFFER)
    {
        PMIB_IPADDRTABLE tmp = pIPAddrTable;
        pIPAddrTable = (MIB_IPADDRTABLE*)std::realloc(tmp, dwSize);
        if (!pIPAddrTable)
        {
            std::free(tmp);
            throw std::bad_alloc();
        }
    }
    if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR)
    {
        std::free(pIPAddrTable);
        throw CSocketException(dwRetVal, "getOwnIpV4Addresses :: GetIpAddrTable failed -> %s", ::error_message(dwRetVal));
    }
    for (DWORD i = 0; i < (int)pIPAddrTable->dwNumEntries; i++)
    {
        if ((pIPAddrTable->table[i].wType & MIB_IPADDR_PRIMARY) && (pIPAddrTable->table[i].wType & MIB_IPADDR_DYNAMIC))
        {
            ::inet_ntop(AF_INET, &pIPAddrTable->table[i].dwAddr, buffer, sizeof(buffer));
            addresses.push_back(buffer);
        }
    }
    std::free(pIPAddrTable);
#else
#pragma message ( "Not implemented" )
#endif // defined(WIN32) || defined(_WIN32)
    if (!addresses.size())
    {
        addresses.push_back("127.0.0.1"); /*dummy*/
    }
    return addresses.front();
}

int error_code()
{
#if defined(__linux) || defined(__linux__)
    return errno;
#elif defined(WIN32) || defined(_WIN32)
    return ::WSAGetLastError();
#endif
}

const char* error_message(int error_code)
{
#if defined(__linux) || defined(__linux__)
    const char* error = ::strerror(error_code);   // error_code = errno
#elif defined(WIN32) || defined(_WIN32)

#ifndef ERROR_BUFFER_SIZE
#define ERROR_BUFFER_SIZE 256
#endif//ERROR_BUFFER_SIZE

    static char error[ERROR_BUFFER_SIZE];
    std::memset(error, 0, ERROR_BUFFER_SIZE);
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
