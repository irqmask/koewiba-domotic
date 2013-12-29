#ifndef _SYSCONSOLE_H_
#define _SYSCONSOLE_H_

#include "system.h"

#ifdef _SYS_WINDOWS_
# include <conio.h>
#endif

# define SYS_cGetChar getch()

#ifdef _SYS_LINUX_
char getch(void);
char getche(void);
#endif

#endif
