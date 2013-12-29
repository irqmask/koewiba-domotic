#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include "KWBMonitorDisplay.h"

void KWB__vDisplayInit(void)
{
    //initscr();
    //raw();                  /* Line buffering disabled  */
    //keypad(stdscr, TRUE);   /* We get F1, F2 etc..      */
    //noecho();               /* Don't echo() while we do getch */
}

void KWB__vDisplayClose(void)
{
    //endwin();
}

void KWB__vDisplayPrintUsage(void)
{
    printf(
        "kwbmonitor - KoeWiBa domotic bus monitor\n"
        "\n"
        "Usage:       kwbmonitor [Options] \n"
        "Options:\n"
        "  -p, --port arg           port to open\n"
        "  -b, --baudrate arg       baud rate to use 'baud' shall be an integral number\n"
        "  -fc, --flowcontrol arg   flow control to use. Possible values are:\n"
        "                           none, xonxoff, rtscts\n"
        "  --listports              lists all present ports and quits program\n"
        "  --help                   shows this text and quits program\n"
    );
    //refresh();
}

void KWB__vDisplayPrintMessage(const char* pcFormat, ...)
{
    va_list args;

    va_start( args, pcFormat );
    vprintf(pcFormat, args);
    //refresh();
}

