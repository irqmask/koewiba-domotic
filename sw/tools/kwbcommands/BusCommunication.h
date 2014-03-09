#ifndef _BUSCOMMUNICATION_H_
#define _BUSCOMMUNICATION_H_

// --- Include section ---------------------------------------------------------

#include "PortableSerialLib.h"

// --- Definitions -------------------------------------------------------------

#define BUSPHY_MAX_DEVICES 4

// --- Type definitions --------------------------------------------------------

typedef struct uartdevice {
    const char*     pcPortName;
    PSerLibHandle_t  psHandle;
} sUartDevice_t;

// --- Local variables ---------------------------------------------------------


// --- Module global variables -------------------------------------------------

sUartDevice_t BUSCOMM__asDevices[BUSPHY_MAX_DEVICES];

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void BUSCOMM_vInit(uint8_t uUartIndex,
                    const char* pcPathFilename,
                    uint16_t uOwnAddress);

void BUSCOMM_vDoCycle(void);

void BUSCOMM_vClose(uint8_t uUartIndex);

void BUSCOMM_vSetSendFlag(uint8_t uFlags);

void BUSCOMM_vSetUartDevice(uint8_t uDeviceIndex,
                            const char* pcPathFilename);


#endif // _BUSCOMMUNICATION_H_
