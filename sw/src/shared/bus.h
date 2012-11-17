/**
 * @addtogroup BUS
 * @brief Public interface of bus protocol.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    bus.h
 * @brief   Declaration of communication driver routines.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _BUS_H_
#define _BUS_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define BUS_BAUDRATE           38400
#define BUS_MAXMSGSIZE         16
#define BUS_MAXBIGMSGSIZE      (64 + 2)

// --- Type definitions --------------------------------------------------------

typedef enum eCommMsgByteIndex {
    eMsgLength,
    eMsgSender,
    eMsgData0,
    eMsgData1,
    eMsgData2,
    eMsgData3,
    eMsgData4,
    eMsgData5,
    eMsgData6,
    eMsgData7,
    eMsgData8,
    eMsgData9,
    eMsgData10,
    eMsgData11,
    eMsgCRCHigh,
    eMsgCRCLow,
    eMsgLast
} eCommMsgByteIndex_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _BUS_H_ */
/** @} */
