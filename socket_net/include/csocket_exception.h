#pragma once

#include <stdexcept>
#include <string>

class CSocketException : public std::exception
{
public:
    CSocketException(int error_code, const char* error_message, ...);
    CSocketException(const char* error_message, ...);
    ~CSocketException();

    const char* what() const throw() override;
    int getErrorCode() const;
private:
    const char* mException = NULL;
    int mErrorCode = -1;
};
