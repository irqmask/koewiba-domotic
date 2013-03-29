/**
 * @addtogroup BUS
 * @addtogroup BUS_TRANSPORT
 * @brief Transport protocol for sending and receiving messages on the bus.
 *
 * @{
 * @file    bus_transport.c
 * @brief   Transport protocol for sending and receiving messages on the bus.
 * @todo    1. Describe file purpose
 * @todo    2. Implement a queue of received messages instead of only one 
            message. Many nodes can send one node a message.
 * @todo    3. Decide if an outgoing message queue is needed. For the first step 
            surely not.
 * @todo    4. Further improvements :-)
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "bus_intern.h"
#include "crc16.h"

// TODO remove after debug
#include <util/delay.h>
#include "led_debug.h"
#include <avr/sleep.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// Reset bus to Idle state.
static void vResetBus(sBus_t* psBus)
{
    psBus->eState = eBus_Idle;
    psBus->sRecvMsg.uOverallLength = 0;
    psBus->sRecvMsg.uLength = 0;
}

// Create empty message depending on bus's configuration.
static void vCreateEmptyMessage(sBus_t* psBus)
{
    psBus->auEmptyMsg[0] = BUS_SYNCBYTE; 
    psBus->auEmptyMsg[1] = psBus->sCfg.uOwnNodeAddress & 0x007F; // local address on bus
    psBus->auEmptyMsg[2] = 0; // length
}

// Start sending the wakeup-byte.
static BOOL bSendWakeupByte(sBus_t* psBus)
{
	uint8_t msg = WAKEUPBYTE;
	if(BUS__bPhySend(&psBus->sPhy, &msg, 1))
	{
		while( BUS__bPhySending(&psBus->sPhy) ) {}; // Wait till message is sent completely.
		return TRUE;
	}
	return FALSE;
}

// Receive and interpret data.
static BOOL bReceive(sBus_t* psBus)
{
    uint16_t crc;
    uint8_t u;
    BOOL    bytereceived;
    
    do {
        if (!(bytereceived = BUS__bPhyDataReceived(&psBus->sPhy)) || psBus->bMsgReceived) {
            break; // No byte received or message not retrieved.
        }
        BUS__bPhyReadByte(&psBus->sPhy, &u);
        
        // 1. byte: check sync byte
        if (psBus->sRecvMsg.uOverallLength == 0) {
            if (u != BUS_SYNCBYTE) {
                vResetBus(psBus);
                break; // not the sync byte, wait for next byte
            }
            
        // 2. byte: check token byte
        } else if (psBus->sRecvMsg.uOverallLength == 1) {
            // token received?
            if (u & TOKENBIT) {
                // is it me?
				vResetBus(psBus); // byte collecting ends after token.
                if ((u & ADDRMASK) == (psBus->sCfg.uOwnNodeAddress & 0x007f)) {
                    psBus->eState = eBus_GotToken;
                } else {
                    psBus->eState = eBus_Idle;
                }
                psBus->sRecvMsg.uOverallLength = 0;
                psBus->sRecvMsg.uLength = 0;
				break;
            }
            else {
                // message received. save sender-address lower byte 
                psBus->sRecvMsg.uSender = u;
                psBus->eState = eBus_ReceivingActive;
            }
        }
        
        // active receiving state, receive and check message
        // and then go back to eBus_Idle state.
        if (psBus->eState == eBus_ReceivingActive) {
            // 3. byte: LE - Length of message from AR to CRCL
            if (psBus->sRecvMsg.uOverallLength == 2) {
                // check correctness of length
                if ((0==u)) {
                	psBus->eState = eBus_Idle;
                	psBus->sRecvMsg.uOverallLength = 0;
#ifdef BUS_SCHEDULER
                	psBus->bSchedMsgReceived = TRUE;
#endif
                	break;
                }
                else if (u > BUS_MAXBIGMSGLEN) {
                	// length is zero or length is too big
                    vResetBus(psBus); // wait for next message on bus
                    break;
                }
                psBus->sRecvMsg.uLength = u;
                
            // 4. byte: AR - Address receiver 7bit
            } else if (psBus->sRecvMsg.uOverallLength == 3) {
                // check correctness of receiver address
                if (u & TOKENBIT) {
                	// bad receiver address. valid range is 0x00..0x7F
                    vResetBus(psBus);
                    break;
                }
                psBus->sRecvMsg.uReceiver = u;
                // hello, is it me you are looking for (or broadcast-message)?
                if ((0 == u) || (u == (psBus->sCfg.uOwnNodeAddress & 0x007f))) {
                    // nothing more to do here.
                } else {
                    // we are not interested in this message
                    psBus->eState = eBus_ReceivingPassive;
                }
                
            // 5. byte: EA - Extended address 4bit sender in higher nibble, 4bit receiver in lower nibble.
            } else if (psBus->sRecvMsg.uOverallLength == 4) {
                psBus->sRecvMsg.uSender |= (((uint16_t)u & 0x00F0) << 4);
                psBus->sRecvMsg.uReceiver |= (((uint16_t)u & 0x000F) << 8);
                // TODO check receiver address again

            // receive data (5th byte till length + 3(SY+AS+LE) - 2(CRC))
            } else if (psBus->sRecvMsg.uOverallLength > 4) {
            	if (psBus->sRecvMsg.uOverallLength == (psBus->sRecvMsg.uLength + 3 - 2)) {
            		psBus->sRecvMsg.uCRC = u << 8;
            		// N th byte: CRCL - Low byte of 16bit CRC

            	} else if (psBus->sRecvMsg.uOverallLength == (psBus->sRecvMsg.uLength + 3 - 1)) {
            		psBus->sRecvMsg.uCRC |= u;
            		crc = CRC_uCalc16(&psBus->sRecvMsg.auBuf[0], psBus->sRecvMsg.uLength + 3 - 2);
            		if (crc == psBus->sRecvMsg.uCRC) {
                		LED_ERROR_OFF;
            			psBus->bMsgReceived = TRUE;
#ifdef BUS_SCHEDULER
            			psBus->bSchedMsgReceived = TRUE;
#endif
            			psBus->eState = eBus_Idle;
            			psBus->sRecvMsg.uOverallLength = 0;
            			break;
            		} else {
            			// invalid length of message
                		LED_ERROR_ON;
            			vResetBus(psBus);
            			break;
            		}

            	} else if(psBus->sRecvMsg.uOverallLength >= psBus->sRecvMsg.uLength + 3) {
            		// invalid length of message
            		vResetBus(psBus);
            		break;
            	}
            }
        }

        psBus->sRecvMsg.auBuf[psBus->sRecvMsg.uOverallLength] = u;
        psBus->sRecvMsg.uOverallLength++;

        // passive receiving state, only count bytes till the end of the message 
        // and then go back to eBus_Idle state.
        if (psBus->eState == eBus_ReceivingPassive) {
            if (psBus->sRecvMsg.uOverallLength >= (psBus->sRecvMsg.uLength + 3)) {
#ifdef BUS_SCHEDULER
                psBus->bSchedMsgReceived = TRUE;
#endif
                vResetBus(psBus);
            }
        }

    } while ( FALSE );
    
    return bytereceived;
}

// Check if data had to be sent or otherwise send empty message.
static void vInitiateSending(sBus_t* psBus)
{
    psBus->eState = eBus_Sending;
    // is there a pending message to be sent?
    if (psBus->sSendMsg.uOverallLength != 0) {
        // initiate sending of the message
        BUS__bPhySend(&psBus->sPhy,
        		      psBus->sSendMsg.auBuf,
        		      psBus->sSendMsg.uOverallLength);
    } else {
        // send empty message
        BUS__bPhySend(&psBus->sPhy, psBus->auEmptyMsg, BUS_EMPTY_MSG_LEN);
    }
    
}

// Check if data has been sent.
static void vSend(sBus_t* psBus)
{
    // data completely sent?
    if (BUS__bPhySending(&psBus->sPhy) == FALSE) {
        psBus->eState = eBus_Idle;
        psBus->sSendMsg.uOverallLength = 0;
        psBus->sSendMsg.uLength = 0;
    }
}

// --- Module global functions -------------------------------------------------

/**
 * Manage sending and receiving of messages.
 *
 * @param[in]   psBus       Handle of the bus.
 *
 * @returns TRUE, if there is a pending received message, otherwise FALSE.
 */
BOOL BUS__bTrpSendReceive(sBus_t* psBus)
{
	uint8_t bb = 0;

    switch (psBus->eState) {
    case eBus_GotToken:
        // initiate sending of message
    	vInitiateSending(psBus);
        break;
    
    case eBus_Sending:
        // message is currently being sent
        vSend(psBus);
        break;
    
    case eBus_InitWait:
    case eBus_Idle:
    case eBus_ReceivingWait:
    case eBus_ReceivingActive:
    case eBus_ReceivingPassive:
    default:
    	// repeat receiving while queue is not empty
        while (!psBus->bMsgReceived && bb<2) {
        	if (bReceive(psBus) == FALSE) break;
        	bb++;
        }
        break;
    }
    
    return psBus->bMsgReceived;
}

/**
 * Start sending of sleep command.
 *
 * @param[in]   psBus       Handle of the bus.
 *
 * @returns TRUE, if the sleep command has been sent, otherwise FALSE.
 */
BOOL BUS__bSendSleepCmd(sBus_t* psBus)
{
    uint16_t crc;
    uint8_t msg[8];

    msg[0] = BUS_SYNCBYTE;                  // SY - Syncronization byte: '�' = 0b10011010
    msg[1] = psBus->sCfg.uOwnNodeAddress;   // AS - Address sender on bus 7bit
    msg[2] = sizeof(msg) - 3;              // LE - Length of message from AR to CRCL
    msg[3] = 0x00;                          // AR - Address receiver 7bit (Broadcast)
    msg[4] = 0x00;                          // EA - Extended address 4bit sender in higher nibble, 4bit receiver in lower nibble.
    msg[5] = SLEEPCOMMAND;                  // MD - Sleep-Command
    crc = CRC_uCalc16(&msg[0], 6);
    msg[6] = crc >> 8;
    msg[7] = crc & 0xFF;

    if( BUS__bPhySend(&psBus->sPhy, msg, sizeof msg) ) {
        while( BUS__bPhySending(&psBus->sPhy) ) {}; // Wait till message is sent completely.
        return TRUE;
    }
    return FALSE;
}

// --- Global functions --------------------------------------------------------

/**
 * Configure the bus interface and data.
 * @note Has to be called before BUS_vInitialize() is called the first time.
 *
 * @param[in]   psBus       Handle of the bus.
 * @param[in]   uNodeAddress Address of this node.
 */
void BUS_vConfigure(sBus_t* psBus, uint16_t uNodeAddress)
{
    psBus->sCfg.uOwnNodeAddress = uNodeAddress;
    vCreateEmptyMessage(psBus);
#ifdef BUS_SCHEDULER
    BUS__vSchedulConfigure(psBus);
#endif
}

/**
 * Initialize Bus interface.
 *
 * @param[in]   psBus       Handle of the bus.
 * @param[in]   uUart       Number of the UART. 0=first.
 */
void BUS_vInitialize(sBus_t* psBus, uint8_t uUart)
{
    psBus->eState = eBus_Idle;
#ifdef BUS_SCHEDULER
    psBus->eModuleState = eSched_Discovery;
#else
    psBus->eModuleState = eMod_Running;
#endif
    vCreateEmptyMessage(psBus);
    BUS__vPhyInitialize(&psBus->sPhy, uUart);
}

/**
 * Flushes phy-buffer and resets bus.
 *
 * @param[in]   psBus       Handle of the bus.
 */
void BUS_vFlushBus(sBus_t* psBus)
{
	BUS__uPhyFlush(&psBus->sPhy);
	vResetBus(psBus);
}

/**
 * Checks if there is a pending message to be read.
 *
 * @param[in]   psBus       Handle of the bus.
 * 
 * @returns TRUE, if there is a pending message.
 */
BOOL BUS_bGetMessage(sBus_t* psBus)
{
    return BUS__bTrpSendReceive(psBus);  
}

/**
 * Reads a pending message.
 *
 * @param[in] 	psBus       Handle of the bus.
 * @param[out] 	puSender    Sender of the message.
 * @param[out] 	puLen       (Netto) Length of the message.
 * @param[out]  puMsg       Received message.
 *
 * @returns TRUE, if a message has been received and.
 */
BOOL BUS_bReadMessage(sBus_t*  psBus, 
                      uint16_t* puSender,
                      uint8_t* puLen,
                      uint8_t* puMsg)
{
    uint8_t len = 0;
    
    do {
        // is there a new message pending?
        if (psBus->bMsgReceived == FALSE) {
            break;
        }
        *puSender   = psBus->sRecvMsg.uSender;
        
        while (len < psBus->sRecvMsg.uLength - 4) {
            puMsg[len] = psBus->sRecvMsg.auBuf[5 + len];
            len ++;
        }
        // reset bus to IDLE state, so we are ready to receive the next message
        psBus->bMsgReceived = FALSE;
        psBus->sRecvMsg.uLength = 0;
        psBus->sRecvMsg.uOverallLength = 0;
        psBus->eState = eBus_Idle;

        *puLen      = len;
        return TRUE;
    } while ( FALSE );
    
    return FALSE;
}

/** 
 * Send a message.
 *
 * @param[in]   psBus       Handle of the bus.
 * @param[in]   uReceiver   Receiver address of the message.
 * @param[in]   uLen        (Netto) Length of the message.
 * @param[in]   puMsg       Message to send.
 *
 * @returns TRUE, if the message has successfully been queued.
 * @note Use BUS_bIsIdle() to check if message is successfully transmitted.
 */
BOOL BUS_bSendMessage(sBus_t*    psBus,
                      uint16_t   uReceiver,
                      uint8_t    uLen,
                      uint8_t*   puMsg)
{
    uint16_t crc;

    do {
        // Wakeup bus
        if(eMod_Sleeping == psBus->eModuleState) {
        	bSendWakeupByte(psBus);
        	psBus->eModuleState = eMod_Running;
        	BUS_vFlushBus(psBus);
        }

    	// check length of message to be sent.
        if (uLen == 0 || uLen > BUS_MAXMSGLEN) {
        	break;
        }
        // prepare message header
        psBus->sSendMsg.uOverallLength = 0;
        psBus->sSendMsg.uLength = 0;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = BUS_SYNCBYTE;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = psBus->sCfg.uOwnNodeAddress & 0x007F;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = uLen + 4;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = uReceiver & 0x007F;
        // EA - Extended address 4bit sender in higher nibble, 4bit receiver in lower nibble.
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] =
            (((uReceiver & 0x0F00) >> 8) |
            ((psBus->sCfg.uOwnNodeAddress & 0x0F00) >> 4));
        // copy data
        while (uLen--) {
        	psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength] = puMsg[psBus->sSendMsg.uLength];
        	psBus->sSendMsg.uLength++;
        	psBus->sSendMsg.uOverallLength++;
        }
        // calculate and send CRC
        crc = CRC_uCalc16(&psBus->sSendMsg.auBuf[0], psBus->sSendMsg.uOverallLength);
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = crc >> 8;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = crc & 0xFF;
        return TRUE;
    } while ( FALSE );
    return FALSE;
}

/**
 * Check if bus is in IDLE state.
 *
 * @param[in]   psBus       Handle of the bus.
 * @returns TRUE, if bus is IDLE.
 */
BOOL BUS_bIsIdle(sBus_t*       psBus)
{
    return (psBus->eState == eBus_Idle);
}

/**
 * Set module into sleep mode.
 *
 * @param[in]   psBus       Handle of the bus.
 */
void BUS_vSleep(sBus_t*       psBus)
{
	psBus->eModuleState = eMod_Sleeping;
	//@TODO: Hier muss die Sleep-Methode rein sobald die Hardware in der Lage ist den Controller wieder aufzuwecken.
	CLK_vControl(FALSE); // disable clock-timer, otherwise
	// irq will cause immediate wakeup.

	// sleep till byte is received.
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
	if(BUS__bPhyDataReceived(&psBus->sPhy)) {
		psBus->eModuleState = eMod_Running;
	}

	_delay_ms(1); // wait for sys-clock becoming stable
	BUS_vFlushBus(psBus); // Clean bus-buffer
	CLK_vControl(TRUE); // enable clock-timer
}

/** @} */
/** @} */
