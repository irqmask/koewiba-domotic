
#include "prjconf.h"

#include <stdio.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <termios.h>
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
    ch = getchar();
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

