#ifndef _SYSCONSOLE_H_
#define _SYSCONSOLE_H_

#include "prjconf.h"

#if defined (PRJCONF_WINDOWS) && (_MSC_VER) && _MSC_VER < 1900
  #include <conio.h>
  #include <stdarg.h>
#endif

#define sys_con_getch() getch()

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
char getch(void);
char getche(void);
#elif defined (PRJCONF_WINDOWS) && (_MSC_VER) && _MSC_VER < 1900
int snprintf(char *outBuf, size_t size, const char *format, ...);
#endif

#endif
