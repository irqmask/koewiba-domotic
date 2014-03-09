#include "system.h"

#ifdef _SYS_WINDOWS_
#  ifndef _WIN32_WINNT            // Gibt an, dass Windows Vista die mindestens erforderliche Plattform ist.
#    define _WIN32_WINNT 0x0600     // �ndern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.
#  endif
#  define bool  int
#  define true  (1==1)
#  define false (1==0)
#  define snprintf sprintf_s
#  define ThreadFunc(name) DWORD WINAPI name( LPVOID p)
#  define startThread(func)  do{                 \
                               CreateThread(NULL, 0, (func), NULL, 0, NULL);\
                             }while(0);

#else
#  include <pthread.h>
#  include <unistd.h>
#  include <stdbool.h>
#  include "safe_str_lib.h"
#  define ThreadFunc(name) void *name(void *p)
#  define startThread(func)  do{                 \
                               pthread_t tid;   \
                               pthread_create(&tid, NULL, (func), NULL);\
                             }while(0);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BusCommunication.h"
#include "PortableSerialLib.h"
#include "sysconsole.h"
#include "systime.h"
#include "crc16.h"

PSerLibHandle_t g_sSerPort = PSL_NOPORT_HANDLE;

const char* g_pcPortname = "COM1";
BOOL g_bRunning = TRUE;

static void vListPorts(void)
{
    char buff[200];
    char* iterator = buff;
    int n;

    PSL_ErrorCodes_e ec = PSerLib_getAvailablePorts(buff, sizeof(buff), &n);
    if( ec != PSL_ERROR_none ) {
        printf("error: %s\n", PSerLib_getErrorMessage(ec));
        if(n>0) {
            printf("Allthough i found %i devices:\n", n);
        }
    } else {
        printf("found %i devices:\n", n);
    }
    for( ; *iterator; iterator+=strlen(iterator)+1 )
    {
        printf("%s\n",iterator);
    }
}

static void vDisplayPrintUsage(void)
{
    printf(
        "kwbmonitor - KoeWiBa domotic bus monitor\n"
        "\n"
        "Usage:       kwbmonitor [Options] \n"
        "Options:\n"
        "  -p, --port arg           port to open\n"
        "  --listports              lists all present ports and quits program\n"
        "  --help                   shows this text and quits program\n"
    );
    //refresh();
}

bool bParseCommandLine(int iArgC, char* pcArgV[])
{
    int ii;
    bool commandlineerror = false;

    for (ii=1; (ii<iArgC) && (!commandlineerror); ++ii) {
        if (strcmp(pcArgV[ii], "--listports") == 0) {
            vListPorts();
            return false;

        } else if (strcmp(pcArgV[ii], "--help") == 0) {
            vDisplayPrintUsage();
            return false;

        } else if ((strcmp(pcArgV[ii], "-p") == 0) ||
                   (strcmp(pcArgV[ii], "--port") == 0)) {
            ++ii;
            if (ii<iArgC) {
                g_pcPortname = pcArgV[ii];
            } else {
                printf("Missing Argument for '%s'\n", pcArgV[ii-1]);
                commandlineerror = true;
            }
        } else {
            printf("Unkown parameter '%s'\n", pcArgV[ii]);
            commandlineerror = true;
        }
    }

    if (commandlineerror) {
        printf("wrong parameter specified\n\n");
        vDisplayPrintUsage();
        return false;
    }
    return true;
}

ThreadFunc(readerThread)
{
    while(g_bRunning) {
	BUSCOMM_vDoCycle();
        //SYS_vSleepMs(1);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    char cc;
    setbuf(stdout, NULL);       // disable buffering of stdout

    do {
        if (!bParseCommandLine(argc, argv)) break;

        BUSCOMM_vInit(0, "/dev/ttyUSB0", 9);

	g_bRunning = TRUE;
        startThread(&readerThread);
        printf("Alles OK!\n");
        while (g_bRunning) {
            cc = getch();
            switch (cc) {
            case 'x':
            case 'X':
                g_bRunning = 0;
                break;

            case '1': BUSCOMM_vSetSendFlag(1<<0); break;
            case '2': BUSCOMM_vSetSendFlag(1<<1); break;
            case '3': BUSCOMM_vSetSendFlag(1<<2); break;
            case '4': BUSCOMM_vSetSendFlag(1<<3); break;

            default:
                break;
            }
            SYS_vSleepMs(10);
        }
	g_bRunning = FALSE;
        SYS_vSleepMs(100);
	
    } while (0);
    BUSCOMM_vClose(0);

    return 0;
}
