
#ifdef _MSC_VER
#  ifndef _WIN32_WINNT            // Gibt an, dass Windows Vista die mindestens erforderliche Plattform ist.
#    define _WIN32_WINNT 0x0600     // Ändern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.
#  endif
#  include <windows.h>
#  define bool  int
#  define true  (1==1)
#  define false (1==0)
#  define sleep_ms(x) do{Sleep(x);}while(0)
#  define snprintf sprintf_s
#  define ThreadFunc(name) DWORD WINAPI name( LPVOID p)
#  define startThread(func)  do{                 \
                               CreateThread(NULL, 0, (func), NULL, 0, NULL);\
                             }while(0);

#else
#  include <pthread.h>
#  include <unistd.h>
#  include <stdbool.h>
#  define sleep_ms(x) do{usleep((x)*1000);}while(0)
#  define ThreadFunc(name) void *name(void *p)
#  define startThread(func)  do{                 \
                               pthread_t tid;   \
                               pthread_create(&tid, NULL, (func), NULL);\
                             }while(0);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PortableSerialLib.h"

PSerLibHandle_t port = PSL_NOPORT_HANDLE;

void listPorts(void)
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


void printUsage(void)
{
    printf(
        "PortableSerialLibTest   simple programm to test portable serial port lib\n"
        "\n"
        "Usage:   PortableSerialLibTest [Options] \n"
        "Options:\n"
        "  -p, --port arg           port to open\n"
        "  -b, --baudrate arg       baud rate to use 'baud' shall be an integral number\n"
        "  -fc, --flowcontrol arg   flow control to use. Possible values are:\n"
        "                           none, xonxoff, rtscts\n"
        "  --listports              lists all present ports and quits program\n"
        "  --help                   shows this text and quits program\n"
    );
}


ThreadFunc(readerThread)
{
    unsigned char buffer[200];
    int bytesread, chars = 0;

    while(42) {
        bytesread = 0;
        PSerLib_readBinaryData(port, buffer, 1, &bytesread);
        if (bytesread > 0) {
            printf("%02X ", buffer[0]);
            chars++;
            if (chars >= 16) {
                printf("\r\n");
                chars = 0;
            }
        }
    }
}


int main(int argc, char* argv[])
{
    int i;
    bool cmdlineError=false;
    const char* portname = "COM1";
    int  baudrate = 9600;
    PSL_ErrorCodes_e ec;
    PSL_FlowControl_e flowControl = PSL_FC_none;

    setbuf(stdout, NULL);
    for (i=1; (i<argc)&&(!cmdlineError); ++i) {
        if (strcmp(argv[i], "--listports") == 0) {
            listPorts();
            return 0;
        } else if (strcmp(argv[i], "--help") == 0) {
            printUsage();
            return 0;
        } else if ((strcmp(argv[i], "-p") == 0) ||
                   (strcmp(argv[i], "--port") == 0)) {
            ++i;
            if (i<argc) {
                portname = argv[i];
            } else {
                printf("Missing Argument for '%s'\n", argv[i-1]);
                cmdlineError = true;
            }
        } else if ((strcmp(argv[i], "-b") == 0) ||
                   (strcmp(argv[i], "--baudrate") ==0 )) {
            ++i;
            if (i<argc) {
                baudrate = atoi(argv[i]);
                if (baudrate == 0) {
                    printf("'%s' seems not to be a baud rate\n", argv[i]);
                    cmdlineError = true;
                }
            } else {
                printf("Missing Argument for '%s'\n", argv[i-1]);
                cmdlineError = true;
            }
        } else if ((strcmp(argv[i], "-fc") == 0) ||
                   (strcmp(argv[i], "--flowcontrol") ==0 )) {
            ++i;
            if (i<argc) {
                if (strcmp(argv[i], "none") == 0) {
                    flowControl = PSL_FC_none;
                } else if( strcmp(argv[i], "xonxoff") == 0) {
                    flowControl = PSL_FC_xon_xoff;
                } else if( strcmp(argv[i], "rtscts") == 0) {
                    flowControl = PSL_FC_rts_cts;
                } else {
                    printf("Unsupported flow control: '%s'\n", argv[i]);
                    cmdlineError = true;
                }
            } else {
                printf("Missing Argument for '%s'\n", argv[i-1]);
                cmdlineError = true;
            }
        } else {
            printf("unknown command line option: '%s'\n",argv[i]);
            cmdlineError = true;
        }
    }

    if (cmdlineError) {
        printf("wrong parameter specified\n\n");
        printUsage();
        return 1;
    }

    ec = PSerLib_open(portname, &port);
    if (ec == PSL_ERROR_none) {
        ec = PSerLib_setParams(port, baudrate, PSL_DB_8, PSL_P_none, PSL_SB_1, flowControl);

        if (ec == PSL_ERROR_none) {
            int n = 0;
            char buff[100];

            //readerThread(NULL);
            startThread(&readerThread);
            while(42) {
                snprintf(buff, sizeof(buff), "Hello World! -%d-\n", n);
                //PSerLib_puts(port, buff);
                ++n;
                sleep_ms(1000);
            }
        }
    }

    if (ec != PSL_ERROR_none) {
        printf("error: %s\n", PSerLib_getErrorMessage(ec));
    }

    PSerLib_close(&port);
    return ec == PSL_ERROR_none;
}
