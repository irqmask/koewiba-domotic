#ifndef _SYSCONSOLE_H_
#define _SYSCONSOLE_H_

#include "prjconf.h"

#define sys_con_getch() getch()

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
char getch(void);
char getche(void);
#endif

#endif
