/**
 * @addtogroup BUS
 * @addtogroup BUS_SCHEDUL
 * @brief Message scheduler of bus essages. Used by the master on the bus.
 *
 * @{
 * @file    bus_schedul.c
 * @brief   Bus message scheduler.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <avr/sleep.h>

#include "bus.h"
#include "bus_intern.h"

//TODO remove after debug
#include "led_debug.h"
#include <util/delay.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------



/**
 * Start sending of next token.
 */
static BOOL bSendNextTimeSlotToken(sBus_t* psBus, BOOL discovery)
{
    sNodeInfo_t* node;

    if(TRUE != discovery) 	node = &psBus->asNodeList[psBus->uCurrentNode];
    else 					node = &psBus->asDiscoveryList[psBus->uDiscoverNode];

    if (node->uAddress != 0) {
        psBus->auTokenMsg[1] = node->uAddress | 0x80;
        return BUS__bPhySend(&psBus->sPhy, psBus->auTokenMsg, BUS_TOKEN_MSG_LEN);
    }
    return FALSE;
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize scheduler functions.
 *
 * @param[in] psBus		Handle of the bus.
 */
void BUS__vSchedulConfigure(sBus_t* psBus)
{
    uint8_t ii;

    psBus->uCurrentNode  = 0;
    psBus->uDiscoverNode = 0;
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        psBus->asNodeList[ii].uAddress = 0;
        psBus->asNodeList[ii].uErrCnt = 0;
		psBus->asDiscoveryList[ii].uAddress = ii+1;
    }
    psBus->auTokenMsg[0] = BUS_SYNCBYTE;
    psBus->auTokenMsg[1] = 0;
}

// --- Global functions --------------------------------------------------------

/**
 * Add new node to scheduler list.
 *
 * @param[in] psBus			Handle of the bus.
 * @param[in] uNodeAddress	Node address.
 *
 * @returns TRUE if node has been successfully added, otherwise false.
 */
BOOL BUS_bSchedulAddNode(sBus_t* psBus, uint8_t uNodeAddress)
{
    uint8_t ii    = 0;
    BOOL	found = FALSE;

    // search node in emptynode-list and remove it.
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        if (psBus->asDiscoveryList[ii].uAddress == uNodeAddress) {
        	found = TRUE;
        }
        else if (found) { // shift following elements to avoid gaps in list
        	psBus->asDiscoveryList[ii-1].uAddress = psBus->asDiscoveryList[ii].uAddress;
        	psBus->asDiscoveryList[ii-1].uErrCnt  = psBus->asDiscoveryList[ii].uErrCnt;
        }
    }
    if (found) { // add node to nodelist
		psBus->asDiscoveryList[BUS_MAXNODES-1].uAddress = 0; // delete last field, others were already shifted
		for (ii=0; ii<BUS_MAXNODES; ii++) {
			if (psBus->asNodeList[ii].uAddress == 0) {
				psBus->asNodeList[ii].uAddress = uNodeAddress;
				psBus->asNodeList[ii].uErrCnt = 0;
				break;
			}
		}
    }
    return found;
}

/**
 * Remove node from scheduler list.
 *
 * @param[in] psBus			Handle of the bus.
 * @param[in] uNodeAddress	Node address.
 *
 * @returns TRUE if node has been successfully removed, otherwise false.
 */
BOOL BUS_bSchedulRemNode(sBus_t* psBus, uint8_t uNodeAddress)
{
    uint8_t ii    = 0;
    BOOL	found = FALSE;

    // search and remove node in nodelist
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        if (psBus->asNodeList[ii].uAddress == uNodeAddress) {
        	found = TRUE;
        }
        else if (found) { // shift following elements to avoid gaps in list
        	psBus->asNodeList[ii-1].uAddress = psBus->asNodeList[ii].uAddress;
        	psBus->asNodeList[ii-1].uErrCnt = psBus->asNodeList[ii].uErrCnt;
        }
    }
    if(found) { // add node to discovery-list
		psBus->asNodeList[BUS_MAXNODES-1].uAddress = 0; // delete last field, others were already shifted
		psBus->asNodeList[BUS_MAXNODES-1].uErrCnt  = 0;
		for (ii=0; ii<BUS_MAXNODES; ii++) {
			if (psBus->asDiscoveryList[ii].uAddress == 0) {
				psBus->asDiscoveryList[ii].uAddress = uNodeAddress;
				break;
			}
		}
    }
    return found;
}

/**
 * Count node errors.
 */
void vNodeError(sBus_t* psBus)
{
	if (psBus->bSchedDiscovery) return;
	if (psBus->asNodeList[psBus->uCurrentNode].uErrCnt < 255) {
		psBus->asNodeList[psBus->uCurrentNode].uErrCnt++;
		// Remove node-address from scheduling-list after max number of missing answers.
		if(BUS_MAX_ANSWERFAILS < psBus->asNodeList[psBus->uCurrentNode].uErrCnt) {
			BUS_bSchedulRemNode(psBus, psBus->auTokenMsg[1] & 0x7F);
		}
	}
}

/**
 * Check if current node is the scheduler node.
 */
BOOL bCurrNodeIsMe(sBus_t* psBus)
{
    sNodeInfo_t* node;

    node = &psBus->asNodeList[psBus->uCurrentNode];

    if (node->uAddress == psBus->sCfg.uOwnNodeAddress) {
    	return TRUE;
    }
    return FALSE;
}

/**
 * Schedule nodes on bus.
 * 
 * @param[in] psBus
 * Handle of the bus.
 */
BOOL BUS_bScheduleAndGetMessage(sBus_t* psBus)
{
	static uint8_t 	schedloopcnt = BUS_DISCOVERYLOOPS;
    BOOL			rc 			 = FALSE;

    switch (psBus->eState) {

    case eBus_Idle:
		psBus->bSchedDiscovery = FALSE;
    	if(eSched_Discovery == psBus->eModuleState) psBus->bSchedDiscovery = TRUE;
    	else if ((psBus->uCurrentNode == BUS_MAXNODES) || (psBus->asNodeList[psBus->uCurrentNode].uAddress == 0))
    	{ // if last node in active list is reached reset node-counter and switch to discovery-list
    		psBus->uCurrentNode = 0;
    		psBus->bSchedDiscovery = TRUE;
    	}

    	if((psBus->bSchedDiscovery) && ((psBus->uDiscoverNode == BUS_MAXNODES) || (psBus->asDiscoveryList[psBus->uDiscoverNode].uAddress == 0)))
		{ // if last node in discovery list is reached reset node-counter
    		psBus->uDiscoverNode = 0;
    		if(schedloopcnt) --schedloopcnt;
    	}

        // send token
        if (bSendNextTimeSlotToken(psBus, psBus->bSchedDiscovery))
        {
            CLK_bTimerStart(&psBus->sNodeAnsTimeout, 30);
            psBus->eState = eBus_SendingToken;
        }
        else LED_ERROR_ON;
        break;

    case eBus_SendingToken:
        // wait for finished token sending
        if (!BUS__bPhySending(&psBus->sPhy)) {
        	if (bCurrNodeIsMe(psBus)) {
        		// The scheduler cannot receive its own message,
        		// so we set the state to eBus_GotToken "manually".
        	    psBus->eState = eBus_GotToken;
        	}
        	else {
        		// Token has been sent, now wait for a message.
                psBus->bSchedWaitingForAnswer = TRUE;
                psBus->eState = eBus_ReceivingWait;
        	}
        }
        else {
        	if (CLK_bTimerIsElapsed(&psBus->sNodeAnsTimeout))
        	{
        		vNodeError(psBus);
        		// return to IDLE state
        		psBus->eState = eBus_Idle;
        		LED_ERROR_ON;
        	}
		}
        break;

    case eBus_ReceivingPassive:
    	// any message on the bus resets counter
    	if(eMod_Running == psBus->eModuleState) schedloopcnt = BUS_LOOPS_TILL_SLEEP;
    	break;

    default:
        break;
    }

    rc = BUS__bTrpSendReceive(psBus);
    if (rc) schedloopcnt = BUS_LOOPS_TILL_SLEEP;

    if (psBus->bSchedWaitingForAnswer) {
    	BOOL recEnd = FALSE;
        if (psBus->bSchedMsgReceived) {
			if (TRUE==psBus->bSchedDiscovery) {
				if(BUS_bSchedulAddNode(psBus, psBus->auTokenMsg[1] & 0x7F)) {
					--psBus->uDiscoverNode; // re-establish last used index, because addresses were shifted.
				}
				psBus->bSchedDiscovery = FALSE;
			}
            // TODO check node ID of received message and route eventually
        	//      to other line or net
			recEnd = TRUE;

        }
        else {
            if (CLK_bTimerIsElapsed(&psBus->sNodeAnsTimeout)) {
                vNodeError(psBus);
                // return to IDLE state
				recEnd = TRUE;
				if (0 == schedloopcnt) { // if schedulerloopcount has reached zero ...
					schedloopcnt = BUS_LOOPS_TILL_SLEEP;
					if (eSched_Discovery == psBus->eModuleState) {
					    psBus->eModuleState = eMod_Running;   // ... finish discovery
					} else {
					    psBus->eModuleState = eMod_Sleeping; // ... initiate sleep-mode
					}
				}
            }
        }
        if (recEnd) { // receiving finished
            psBus->bSchedMsgReceived = FALSE;
            psBus->bSchedWaitingForAnswer = FALSE;
            psBus->eState = eBus_Idle;
            if(TRUE != psBus->bSchedDiscovery) 	++psBus->uCurrentNode;
			else 								++psBus->uDiscoverNode;
        }
    }
    return rc;
}

/**
 * Check if schedule is in sleep mode and send sleep command to slaves.
 *
 * @param[in] psBus
 * Handle of the bus.
 */
void BUS_vScheduleCheckAndSetSleep(sBus_t* psBus)
{
    if (eMod_Sleeping == psBus->eModuleState) {
        if (bSendSleepCmd(psBus)) {
            CLK_vControl(FALSE); // disable clock-timer, otherwise
                                 // irq will cause immediate wakeup.

            // sleep till byte is received.
            set_sleep_mode(SLEEP_MODE_IDLE);
            sleep_mode();

            _delay_ms(1); // wait for sys-clock becoming stable
            BUS_vFlushBus(psBus); // Clean bus-buffer
            psBus->eModuleState = eMod_Running;
            CLK_vControl(TRUE); // enable clock-timer
        }
    }
}
/** @} */
/** @} */
