
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
#include "bus.h"
#include "systime.h"

typedef double float_t;

typedef struct bushistory {
    uSysTime_t      uTimeStart;
    uSysTime_t      uTimeLastByte;
    uSysTime_t      uTimeCurrByte;
    uint8_t         uCurrMsgBytes;
    uint8_t         uExpectedLength;
    uint16_t        uExpectedCRC;
    uint8_t         uCurrSender;
    uint8_t         auMessage[BUS_MAXTOTALMSGLEN+10];
} sBusHistory_t;

PSerLibHandle_t g_sSerPort = PSL_NOPORT_HANDLE;
sBusHistory_t   g_sBusHistory;


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
}

float_t uCurrTimeDiff(sBusHistory_t* psBus)
{
    return (float_t)(psBus->uTimeCurrByte - psBus->uTimeStart) / 1000.0;
}

float_t uLastTimeDiff(sBusHistory_t* psBus)
{
    return (float_t)(psBus->uTimeLastByte - psBus->uTimeStart) / 1000.0;
}

void vThdInitBusHistory(sBusHistory_t* psBus)
{
    psBus->uTimeStart = SYS_uGetTimeUSecs();
    psBus->uTimeCurrByte = 0;
    psBus->uTimeLastByte = 0;
    psBus->uCurrMsgBytes = 0;
}

void vThdParseMessage(uint8_t uNewByte, sBusHistory_t* psBus)
{
    uint8_t i;

    enum {
        eMsgNothing,
        eMsgError,
        eMsgToken,
        eMsgEmpty,
        eMsgComplete
    } msgstatus;

    msgstatus = eMsgNothing;

    psBus->uTimeLastByte = psBus->uTimeCurrByte;
    psBus->uTimeCurrByte = SYS_uGetTimeUSecs();

    if (psBus->uCurrMsgBytes == 0) {
        printf("%9.1f | ", uCurrTimeDiff(psBus));
    }
    printf("%02X ", uNewByte);

    // compute status of message
    if (psBus->uCurrMsgBytes == 0) {
        // check for sync byte
        if (uNewByte != 0x9a) {
            msgstatus = eMsgError;
        }
        psBus->auMessage[psBus->uCurrMsgBytes] = uNewByte;

    } else if (psBus->uCurrMsgBytes == 1) {
        // check if token or message
        if (uNewByte & 0x80) {
            msgstatus = eMsgToken;
        }
        psBus->uCurrSender = uNewByte & 0x7F;
        psBus->auMessage[psBus->uCurrMsgBytes] = uNewByte;

    } else if (psBus->uCurrMsgBytes == 2) {
        // check length
        if (uNewByte == 0) {
            msgstatus = eMsgEmpty;
        } else if (uNewByte > BUS_MAXTOTALMSGLEN) {
            msgstatus = eMsgError;
        } else {
            psBus->uExpectedLength = uNewByte;
        }
        psBus->auMessage[psBus->uCurrMsgBytes] = uNewByte;

    } else if (psBus->uCurrMsgBytes == 3) {
        // check address receiver
        if (uNewByte & 0x80) {
            msgstatus = eMsgError;
        } else
            psBus->auMessage[psBus->uCurrMsgBytes] = uNewByte;

    } else if (psBus->uCurrMsgBytes == 4) {
        // extended address
        psBus->auMessage[psBus->uCurrMsgBytes] = uNewByte;

    } else if (psBus->uCurrMsgBytes < 3 + psBus->uExpectedLength - 2) {
        // message data
        psBus->auMessage[psBus->uCurrMsgBytes] = uNewByte;

    } else if (psBus->uCurrMsgBytes == 3 + psBus->uExpectedLength - 2) {
        // CRC high byte
        psBus->uExpectedCRC = uNewByte << 8;

    } else if (psBus->uCurrMsgBytes == 3 + psBus->uExpectedLength - 1) {
        // CRC high byte
        psBus->uExpectedCRC |= (uNewByte & 0xFF);
        msgstatus = eMsgComplete;
    }
    psBus->uCurrMsgBytes++;
    if (psBus->uCurrMsgBytes >= BUS_MAXTOTALMSGLEN+9) {
        msgstatus = eMsgError;
    }

    // fill line
    if (msgstatus != eMsgNothing) {
        for (i=0; i<(BUS_MAXTOTALMSGLEN-psBus->uCurrMsgBytes); i++) printf("   ");
        psBus->uCurrMsgBytes = 0;
    }

    // print status at end of line
    switch (msgstatus) {
    case eMsgNothing:
        break;
    case eMsgError:
        printf("| ERR stray bytes\r\n");
        break;
    case eMsgToken:
        printf("| TOKEN\r\n");
        break;
    case eMsgEmpty:
        printf("| EMPTY MESSAGE\r\n");
        break;
    case eMsgComplete:
        printf("| MESSAGE");
        if (psBus->uExpectedCRC != 0) {
            printf(" BAD CRC\r\n");
        } else {
            printf("\r\n");
        }
        break;
    default:
        break;
    }
}


ThreadFunc(readerThread)
{
    unsigned char buffer[200];
    int bytesread;

    vThdInitBusHistory(&g_sBusHistory);

    while(42) {
        bytesread = 0;
        PSerLib_readBinaryData(g_sSerPort, buffer, 1, &bytesread);
        if (bytesread > 0) {
            vThdParseMessage(buffer[0], &g_sBusHistory);
        }
    }
    return 0;
}


int main(int argc, char* argv[])
{
    int i;
    bool cmdlineError=false;
    const char* portname = "COM1";
    int  baudrate = 38400;
    PSL_ErrorCodes_e ec;
    PSL_FlowControl_e flowControl = PSL_FC_none;

    setbuf(stdout, NULL);       // disable buffering of stdout

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

    ec = PSerLib_open(portname, &g_sSerPort);
    if (ec == PSL_ERROR_none) {
        ec = PSerLib_setParams(g_sSerPort, baudrate, PSL_DB_8, PSL_P_none, PSL_SB_1, flowControl);
        if (ec == PSL_ERROR_none) {
            startThread(&readerThread);
            while(42) {
                sleep_ms(1000);
            }
        }
    }

    if (ec != PSL_ERROR_none) {
        printf("error: %s\n", PSerLib_getErrorMessage(ec));
    }

    PSerLib_close(&g_sSerPort);
    return ec == PSL_ERROR_none;
}
