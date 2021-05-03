#pragma once

#include <stdexcept>
#include <string>

class CSocketException : public std::exception
{
public:
    CSocketException(const char* error_message, ...);
    ~CSocketException();

    const char* what() const throw() override;
private:
    const char* mException = NULL;
};
