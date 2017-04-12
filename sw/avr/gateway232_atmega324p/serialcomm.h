/**
 * @addtogroup BUS_GATEWAY
 * @brief Serial line driver for receiving and sending of messages.
 *
 * @{
 * @file    serialcomm.h
 * @brief   Serial line driver for receiving and sending of messages.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
#ifndef _SERIALCOMM_H_
#define _SERIALCOMM_H_

// --- Include section ---------------------------------------------------------

#include "queue.h"

// --- Definitions -------------------------------------------------------------

#define set_cts_pin()   (SCOMM_PORTOUT |= (1<<SCOMM_CTS))
#define clear_cts_pin() (SCOMM_PORTOUT &= ~(1<<SCOMM_CTS))
#define cts_state()     (SCOMM_PORTIN & (1<<SCOMM_CTS))

// --- Type definitions --------------------------------------------------------

//! serial status flags set by irq and cleared by application
typedef enum scomm_flags {
    eSCOMM_RECV_BYTE_FL = 0b00000001, // pending data in receive queue
    eSCOMM_RECV_NL_FL   = 0b00000010,
    eSCOMM_FRAMEERR_FL  = 0b00000100, // Receive framing error
    eSCOMM_DTAERR_FL    = 0b00001000, // Data error (non hex char received)
    eSCOMM_QFULL_FL     = 0b00010000, // Receive queue full
    eSCOMM_TX_FL        = 0b00100000, // Currently transmitting
} scomm_flags_t;

//! data of the physical layer
typedef struct serialphy {
    uint8_t         uart;
    scomm_flags_t   flags;
    queue_t         recvQ;
    uint8_t         recv_q_data[200];
    queue_t         sendQ;
    uint8_t         send_q_data[200];
} scomm_phy_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void scomm_initialize_uart1         (scomm_phy_t *phy);
bool serial_phy_initiate_sending    (scomm_phy_t *phy);
void serial_phy_check_q_level       (scomm_phy_t *phy);

#endif // _SERIALCOMM_H_
/** @} */
