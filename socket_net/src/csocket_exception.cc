#include "csocket_exception.h"

#include <cstdarg>
#include <cstdint>

CSocketException::CSocketException(int error_code, const char* error_message, ...) :
    mErrorCode{error_code}
{
    /* create arguments lists and extract them via format */
    std::va_list args, args2;
    va_start(args, error_message);
    va_copy(args2, args);

    /* get passed format exception's size then allocate exception string if anything exists */
    const int32_t size = std::vsnprintf(NULL, 0, error_message, args) + 1;
    if (size > 1)
    {
        mException = (const char*)malloc(size);
        if (mException)
        {
            (void)std::vsnprintf((char*)mException, size, error_message, args2);
        }
    }

    /* free arguments lists */
    va_end(args);
    va_end(args2);
}

CSocketException::CSocketException(const char* error_message, ...)
{
    /* create arguments lists and extract them via format */
	std::va_list args, args2;
	va_start(args, error_message);
	va_copy(args2, args);

	/* get passed format exception's size then allocate exception string if anything exists */
	const int32_t size = std::vsnprintf(NULL, 0, error_message, args) + 1;
	if (size > 1)
	{
		mException = (const char*)malloc(size);
		if (mException)
		{
			(void)std::vsnprintf((char*)mException, size, error_message, args2);
		}
	}

	/* free arguments lists */
	va_end(args);
	va_end(args2);
}

CSocketException::~CSocketException()
{
    if (mException)
    {
        free((void*)mException);
    }
}

const char* CSocketException::what() const throw()
{
	return mException;
}

int CSocketException::getErrorCode() const
{
    return mErrorCode;
}
