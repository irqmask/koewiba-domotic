/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    exceptions.h
 * @brief   Define a set of exceptions which shall be used exclusivly.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2020  christian <irqmask@gmx.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <stdexcept>
#include <cstdarg>
#include <cstdint>

//! Helper macro if noexcept is not defined on certain operating systems.
#define NOEXCEPT noexcept

//! Helper macro to combine the file line and function macro.
#define LOC __FILE__, __LINE__, __FUNCTION__

/**
 * Define a set of exceptions which shall be used exclusivly.
 */
class Exception : public std::exception
{
public:
    /**
     * ctor
     * @param[in] file      Name of source file where the exception occurred
     * @param[in] line      Line in source file where the exception occurred
     * @param[in] function  Name of function where the exception occurred
     */
    Exception(const char *file, uint32_t line, const char *function);

    /**
     * @returns full exception message.
     */
    virtual const char *what() const NOEXCEPT;

protected:
    //! Saved source filename
    const char *file;
    //! Saved line number
    const int line;
    //! Saved function name
    const char *function;

    //! Buffer for exception message
    char message[2048];
    //! Buffer for full message containing also filename, line and function name
    char full_message[4096];

    /**
     * Compile error message from format string and full message from message, file, line and function.
     * @param[in]   format      Format string of error messge.
     */
    //!
    template <class... Args>
    void compileErrorMessage(const char *format, ...);
};

//----------------------------------------------------------------------------
template <class... Args>
void Exception::compileErrorMessage(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message) - 1, format, args);
    va_end(args);
    message[sizeof(message) - 1] = '\0';

    snprintf(full_message, sizeof(full_message) - 1, "%s:%d %s:\n%s", file, line, function, message);
    full_message[sizeof(full_message) - 1] = '\0';
}

/**
 * An invalid parameter has been passed to a function.
 */
class InvalidParameter : public Exception
{
public:
    /**
     * ctor
     * @param[in] file      Name of source file where the exception occurred
     * @param[in] line      Line in source file where the exception occurred
     * @param[in] function  Name of function where the exception occurred
     * @param[in] fmt       Format string of the message
     * @param[in] args      Optional arguments used in the format string
     */
    template <class... Args> InvalidParameter(const char *file, uint32_t line, const char *function, const char *fmt,
                                              Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};

/**
 * A logic error was detected e.g. mismatching values.
 */
class LogicError : public Exception
{
public:
    /**
     * ctor
     * @param[in] file      Name of source file where the exception occurred
     * @param[in] line      Line in source file where the exception occurred
     * @param[in] function  Name of function where the exception occurred
     * @param[in] fmt       Format string of the message
     * @param[in] args      Optional arguments used in the format string
     */
    template <class... Args> LogicError(const char *file, uint32_t line, const char *function, const char *fmt,
                                              Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};

/**
 * An operation failed to execute successfully.
 */
class OperationFailed : public Exception
{
public:
    /**
     * ctor
     * @param[in] file      Name of source file where the exception occurred
     * @param[in] line      Line in source file where the exception occurred
     * @param[in] function  Name of function where the exception occurred
     * @param[in] fmt       Format string of the message
     * @param[in] args      Optional arguments used in the format string
     */
    template <class... Args> OperationFailed(const char *file, uint32_t line, const char *function, const char *fmt,
                                             Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};

/**
 * A resource (e.g. a file) was not found-
 */
class ResourceMissing : public Exception
{
public:
    /**
     * ctor
     * @param[in] file      Name of source file where the exception occurred
     * @param[in] line      Line in source file where the exception occurred
     * @param[in] function  Name of function where the exception occurred
     * @param[in] fmt       Format string of the message
     * @param[in] args      Optional arguments used in the format string
     */
    template <class... Args> ResourceMissing(const char *file, uint32_t line, const char *function, const char *fmt,
                                             Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};

/**
 * Failed to establish a connection to a device or socket.
 */
class ConnectionFailed : public Exception
{
public:
    /**
     * ctor
     * @param[in] file      Name of source file where the exception occurred
     * @param[in] line      Line in source file where the exception occurred
     * @param[in] function  Name of function where the exception occurred
     * @param[in] fmt       Format string of the message
     * @param[in] args      Optional arguments used in the format string
     */
    template <class... Args> ConnectionFailed(const char *file, uint32_t line, const char *function, const char *fmt,
                                              Args... args)
        : Exception(file, line, function)
    {
        compileErrorMessage(fmt, args...);
    };
};

/** @} */
