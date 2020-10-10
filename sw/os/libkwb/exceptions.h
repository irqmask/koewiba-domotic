/// Define a set of exceptions which shall be used exclusivly.

#pragma once

#include <stdexcept>
#include <cstdarg>

#define NOEXCEPT noexcept
#define LOC __FILE__, __LINE__, __FUNCTION__

class Exception : std::exception
{
public:
    Exception(const char* file, uint32_t line, const char* function);

    virtual const char* what() const NOEXCEPT;

protected:
    const char* file;
    const int line;
    const char* function;

    char message[2048];
    char full_message[4096];

    template <class... Args>
    void compileErrorMessage(const char* format, ...);
};

template <class... Args>
void Exception::compileErrorMessage(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message) - 1, format, args);
    va_end(args);
    message[sizeof(message) - 1] = '\0';

    snprintf(full_message, sizeof(full_message) - 1, "%s:%d %s:\n%s", file, line, function, message);
    full_message[sizeof(full_message) - 1] = '\0';
}


class InvalidParameter : Exception
{
public:
	template <class... Args>
	InvalidParameter(const char* file, uint32_t line, const char* function, const char* fmt, Args... args)
        : Exception(file, line, function)
	{
		compileErrorMessage(fmt, args...);
	};
};


class OperationFailed : Exception
{
public: 
    template <class... Args>
    OperationFailed(const char* file, uint32_t line, const char* function, const char* fmt, Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};


class ResourceMissing : Exception
{
public:
    template <class... Args>
    ResourceMissing(const char* file, uint32_t line, const char* function, const char* fmt, Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};


class ConnectionFailed : Exception
{
public:
    template <class... Args>
    ConnectionFailed(const char* file, uint32_t line, const char* function, const char* fmt, Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};
