/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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
#include "prjconf.h"

#include <stdio.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <termios.h>
#elif defined (PRJCONF_WINDOWS) && (_MSC_VER) && _MSC_VER < 1900
    #include <stdarg.h>
    #include <string.h>
#endif

#include "sysconsole.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)

static struct termios old, new;

/* Initialize new terminal i/o settings */
static void init_termios(int echo)
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
static void reset_termios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
static char getch_(int echo)
{
    char ch;
    init_termios(echo);
    ch = (char)getchar();
    reset_termios();
    return ch;
}

/**
 * Read 1 character without echo */
char getch(void)
{
    return getch_(0);
}

/**
 * Read 1 character with echo */
char getche(void)
{
    return getch_(1);
}

#endif

//----------------------------------------------------------------------------
#if defined (PRJCONF_WINDOWS) && (_MSC_VER) && _MSC_VER < 1900
/*int vsnprintf(char *outBuf, size_t size, const char *format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(outBuf, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}*/

int snprintf(char *outBuf, size_t size, const char *format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = _vsnprintf_s(outBuf, size, size - 1, format, ap);
    va_end(ap);

    return count;
}

#endif // defined(_MSC_VER) && _MSC_VER < 1900
