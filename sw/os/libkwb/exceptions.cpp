/// Define a set of exceptions which shall be used exclusivly.

#include <cstdarg>
#include <cstring>

#include "exceptions.h"

//----------------------------------------------------------------------------
Exception::Exception(const char *file, uint32_t line, const char *function)
    : std::exception()
    , file(file)
    , line(line)
    , function(function)
{
    memset(message, 0, sizeof(message));
    memset(full_message, 0, sizeof(full_message));
}

//----------------------------------------------------------------------------
const char *Exception::what() const NOEXCEPT
{
    return full_message;
}


