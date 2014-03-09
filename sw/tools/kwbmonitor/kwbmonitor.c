#include "system.h"

#ifdef _SYS_WINDOWS_
#  include <windows.h>
#  ifndef _WIN32_WINNT            // Gibt an, dass Windows Vista die mindestens erforderliche Plattform ist.
#    define _WIN32_WINNT 0x0600     // Ändern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.
#  endif
#  define bool  int
#  define true  (1==1)
#  define false (1==0)
#  define snprintf sprintf_s
#  define ThreadFunc(name) DWORD WINAPI name( LPVOID p)
#  define startThread(func)  do{                 \
                               CreateThread(NULL, 0, (func), NULL, 0, NULL);\
                             }while(0);
#  define strcat_s(a,b,c) strcat(a,c) //dirty hack time, es ist dirty hack time...
#  define sprintf_s snprintf
#  define strcpy_s(a,b,c) strcpy(a,c)
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

#include "PortableSerialLib.h"
#include "sysconsole.h"
#include "systime.h"
#include "crc16.h"
#include "KWBMonitorDisplay.h"

//! (netto) length of the message
#define BUS_MAXMSGLEN          16
//! length of message including header and footer
#define BUS_MAXTOTALMSGLEN		(BUS_MAXMSGLEN + 7)

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


const char* g_pcPortname = "COM1";
uint32_t    g_uBaudrate = 38400;
PSL_FlowControl_e g_eFlowControl = PSL_FC_none;

bool g_bDisplayEmptyMessage = true;
bool g_bDisplayTokenMessage = true;

void vListPorts(void)
{
    char buff[200];
    char* iterator = buff;
    int n;

    PSL_ErrorCodes_e ec = PSerLib_getAvailablePorts(buff, sizeof(buff), &n);
    if( ec != PSL_ERROR_none ) {
        KWB__vDisplayPrintMessage("error: %s\n", PSerLib_getErrorMessage(ec));
        if(n>0) {
            KWB__vDisplayPrintMessage("Allthough i found %i devices:\n", n);
        }
    } else {
        KWB__vDisplayPrintMessage("found %i devices:\n", n);
    }
    for( ; *iterator; iterator+=strlen(iterator)+1 )
    {
        KWB__vDisplayPrintMessage("%s\n",iterator);
    }
}

float_t fCurrTimeDiff(sBusHistory_t* psBus)
{
    return (float_t)(psBus->uTimeCurrByte - psBus->uTimeStart) / 1000.0;
}

float_t fLastTimeDiff(sBusHistory_t* psBus)
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
    char        buffer[256], part[256];
    uint16_t    calcedcrc = 0;
    uint8_t     ii, crclen = 0;

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
        snprintf(buffer, sizeof(buffer)-1, "%9.1f | ", fCurrTimeDiff(psBus));
    }
    snprintf(part, sizeof(part-1), "%02X ", uNewByte);
    strcat_s(buffer, sizeof(buffer)-1, part);

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
        crclen = 3 + psBus->uExpectedLength - 2;
        calcedcrc = CRC_uCalc16(&psBus->auMessage[0], crclen);
        msgstatus = eMsgComplete;
    }
    psBus->uCurrMsgBytes++;
    if (psBus->uCurrMsgBytes >= BUS_MAXTOTALMSGLEN+9) {
        msgstatus = eMsgError;
    }

    // fill line
    if (msgstatus != eMsgNothing) {
        for (ii=0; ii<(BUS_MAXTOTALMSGLEN-psBus->uCurrMsgBytes); ii++)
            strcat_s(buffer, sizeof(buffer)-1, "   ");
        psBus->uCurrMsgBytes = 0;
    }

    // print status at end of line
    switch (msgstatus) {
    case eMsgNothing:
        break;
    case eMsgError:
        strcat_s(buffer, sizeof(buffer)-1, "| ERR stray bytes\r\n");
        printf("%s", buffer);
        break;
    case eMsgToken:
        strcat_s(buffer, sizeof(buffer)-1, "| TOKEN\r\n");
        if (g_bDisplayTokenMessage) printf("%s", buffer);
        break;
    case eMsgEmpty:
        strcat_s(buffer, sizeof(buffer)-1, "| EMPTY MESSAGE\r\n");
        if (g_bDisplayEmptyMessage) printf("%s", buffer);
        break;
    case eMsgComplete:
        strcat_s(buffer, sizeof(buffer)-1, "| MESSAGE");
        if (psBus->uExpectedCRC != calcedcrc) {
            snprintf(part, sizeof(part)-1, "  BAD CRC %x %x %d\r\n", psBus->uExpectedCRC, calcedcrc, crclen);
        } else {
            snprintf(part, sizeof(part)-1, " GOOD CRC %x %x %d\r\n", psBus->uExpectedCRC, calcedcrc, crclen);
        }
        strcat_s(buffer, sizeof(buffer)-1, part);
        printf("%s", buffer);
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


bool bParseCommandLine(int iArgC, char* pcArgV[])
{
    int ii;
    bool commandlineerror = false;

    for (ii=1; (ii<iArgC) && (!commandlineerror); ++ii) {
        if (strcmp(pcArgV[ii], "--listports") == 0) {
            vListPorts();
            return false;

        } else if (strcmp(pcArgV[ii], "--help") == 0) {
            KWB__vDisplayPrintUsage();
            return false;

        } else if ((strcmp(pcArgV[ii], "-p") == 0) ||
                   (strcmp(pcArgV[ii], "--port") == 0)) {
            ++ii;
            if (ii<iArgC) {
                g_pcPortname = pcArgV[ii];
            } else {
                KWB__vDisplayPrintMessage("Missing Argument for '%s'\n", pcArgV[ii-1]);
                commandlineerror = true;
            }
        } else if ((strcmp(pcArgV[ii], "-b") == 0) ||
                   (strcmp(pcArgV[ii], "--baudrate") == 0)) {
            ++ii;
            if (ii<iArgC) {
                g_uBaudrate = atoi(pcArgV[ii]);
                if (g_uBaudrate == 0) {
                    KWB__vDisplayPrintMessage("'%s' seems not to be a baud rate\n", pcArgV[ii]);
                    commandlineerror = true;
                }
            } else {
                KWB__vDisplayPrintMessage("Missing Argument for '%s'\n", pcArgV[ii-1]);
                commandlineerror = true;
            }
        } else if ((strcmp(pcArgV[ii], "-fc") == 0) ||
                   (strcmp(pcArgV[ii], "--flowcontrol") == 0 )) {
            ++ii;
            if (ii<iArgC) {
                if (strcmp(pcArgV[ii], "none") == 0) {
                    g_eFlowControl = PSL_FC_none;
                } else if( strcmp(pcArgV[ii], "xonxoff") == 0) {
                    g_eFlowControl = PSL_FC_xon_xoff;
                } else if( strcmp(pcArgV[ii], "rtscts") == 0) {
                    g_eFlowControl = PSL_FC_rts_cts;
                } else {
                    KWB__vDisplayPrintMessage("Unsupported flow control: '%s'\n", pcArgV[ii]);
                    commandlineerror = true;
                }
            } else {
                KWB__vDisplayPrintMessage("Missing Argument for '%s'\n", pcArgV[ii-1]);
                commandlineerror = true;
            }
        } else {
            KWB__vDisplayPrintMessage("Unkown parameter '%s'\n", pcArgV[ii]);
            commandlineerror = true;
        }
    }

    if (commandlineerror) {
        KWB__vDisplayPrintMessage("wrong parameter specified\n\n");
        KWB__vDisplayPrintUsage();
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    char cc;
    PSL_ErrorCodes_e ec = PSL_ERROR_none;

    printf("KWBMONITOR");
    setbuf(stdout, NULL);       // disable buffering of stdout

    do {
        KWB__vDisplayInit();

        if (!bParseCommandLine(argc, argv)) break;

        ec = PSerLib_open(g_pcPortname, &g_sSerPort);
        if (ec == PSL_ERROR_none) {
            ec = PSerLib_setParams(g_sSerPort, g_uBaudrate, PSL_DB_8, PSL_P_none, PSL_SB_1, g_eFlowControl);
            if (ec == PSL_ERROR_none) {
                startThread(&readerThread);
                while(42) {
                    cc = getch();
                    switch (cc) {
                    case 'e':
                    case 'E':
                        if (g_bDisplayEmptyMessage) {
                            g_bDisplayEmptyMessage = false;
                        } else {
                            g_bDisplayEmptyMessage = true;
                        }
                        break;

                    case 't':
                    case 'T':
                        if (g_bDisplayTokenMessage) {
                            g_bDisplayTokenMessage = false;
                        } else {
                            g_bDisplayTokenMessage = true;
                        }
                        break;

                    default:
                        break;
                    }

                    SYS_vSleepMs(10);
                }
            }
        }

        if (ec != PSL_ERROR_none) {
            KWB__vDisplayPrintMessage("error: %s\n", PSerLib_getErrorMessage(ec));
        }

        PSerLib_close(&g_sSerPort);
    } while (0);
    getch();
    KWB__vDisplayClose();

    return ec == PSL_ERROR_none;
}
