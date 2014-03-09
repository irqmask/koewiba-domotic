#include <stdio.h>
#include "system.h"
#include "bus.h"
#include "cmddef_common.h"

#include "BusCommunication.h"

uint8_t g_bLight0 = 0;
uint8_t g_bLight1 = 0;
uint8_t g_bLight2 = 0;
uint8_t g_bLight3 = 0;
uint8_t g_uSendFlags = 0;

static sBus_t g_sBus;

/**
 * Initialize communication over RS845 bus.
 */
void BUSCOMM_vInit(uint8_t uUartIndex,
                    const char* pcPathFilename,
                    uint16_t uOwnAddress)
{
    BUSCOMM_vSetUartDevice(uUartIndex, pcPathFilename);
    BUS_vInitialize(&g_sBus, uUartIndex);
    BUS_vConfigure(&g_sBus, uOwnAddress);
}

void BUSCOMM_vDoCycle(void)
{
    int ii;
    uint16_t sender = 0;
    uint8_t length = 0,
            message[100];
    BOOL rc = TRUE;

    //BUS__bPhyReadByte(&g_sBus.sPhy, &length);
    if (BUS_bGetMessage(&g_sBus)) {
        rc = BUS_bReadMessage(&g_sBus, &sender, &length, message);
    
        if (rc) {
            printf("Message from %d length %d: ", sender, length);
            for (ii=0; ii<length; ii++) {
                printf("%02X " , message[ii]);
            }
            printf("\n");
        }
    }
    
    if (g_uSendFlags & (1<<0)) {
        message[0] = CMD_eStateBitfield;
        message[1] = 1;
        message[2] = g_bLight0;
        message[3] = 0b00000001;
        if (BUS_bSendMessage(&g_sBus, 11, 4, message)) {
            if (g_bLight0) g_bLight0 = 0; else g_bLight0 = 1;
            g_uSendFlags &= ~(1<<0);
        }
    }
    if (g_uSendFlags & (1<<1)) {
        message[0] = CMD_eStateBitfield;
        message[1] = 1;
        message[2] = g_bLight1;
        message[3] = 0b00000001;
        if (BUS_bSendMessage(&g_sBus, 12, 4, message)) {
            if (g_bLight1) g_bLight1 = 0; else g_bLight1 = 1;
            g_uSendFlags &= ~(1<<1);
        }
    }
    if (g_uSendFlags & (1<<2)) {
        message[0] = CMD_eStateBitfield;
        message[1] = 1;
        message[2] = g_bLight2;
        message[3] = 0b00000001;
        if (BUS_bSendMessage(&g_sBus, 21, 4, message)) {
            if (g_bLight2) g_bLight2 = 0; else g_bLight2 = 1;
            g_uSendFlags &= ~(1<<2);
        }
    }
    if (g_uSendFlags & (1<<3)) {
        message[0] = CMD_eStateBitfield;
        message[1] = 1;
        message[2] = g_bLight3;
        message[3] = 0b00000001;
        if (BUS_bSendMessage(&g_sBus, 22, 4, message)) {
            if (g_bLight3) g_bLight3 = 0; else g_bLight3 = 1;
            g_uSendFlags &= ~(1<<3);
        }
    }
}

void BUSCOMM_vClose(uint8_t uUartIndex)
{
    do {
        if (uUartIndex >= BUSPHY_MAX_DEVICES) break;
        PSerLib_close(&BUSCOMM__asDevices[uUartIndex].psHandle);
    } while (FALSE);
}

void BUSCOMM_vSetUartDevice(uint8_t uDeviceIndex,
                            const char* pcPathFilename)
{
    do {
        if (uDeviceIndex >= BUSPHY_MAX_DEVICES) break;
        BUSCOMM__asDevices[uDeviceIndex].pcPortName = pcPathFilename;
    } while (FALSE);
}

void BUSCOMM_vSetSendFlag(uint8_t uFlags)
{
    g_uSendFlags |= uFlags;
    printf("SENDFLAG: %02X\n", g_uSendFlags);
}
