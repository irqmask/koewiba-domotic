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

#include <util/delay.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Add new node to scheduler list.
 *
 * @param[in] psBus			Handle of the bus.
 * @param[in] uNodeAddress	Node address.
 *
 * @returns TRUE if node has been successfully added, otherwise false.
 */
BOOL BUS_bSchedulAddNode(sSched_t* psSched, uint8_t uNodeAddress)
{
    uint8_t ii    = 0;
    BOOL	found = FALSE;

    // search node in emptynode-list and remove it.
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        if (psSched->asDiscoveryList[ii].uAddress == uNodeAddress) {
        	found = TRUE;
        }
        else if (found) { // shift following elements to avoid gaps in list
            psSched->asDiscoveryList[ii-1].uAddress = psSched->asDiscoveryList[ii].uAddress;
            psSched->asDiscoveryList[ii-1].uErrCnt  = psSched->asDiscoveryList[ii].uErrCnt;
        }
    }
    if (found) { // add node to nodelist
        psSched->asDiscoveryList[BUS_MAXNODES-1].uAddress = 0; // delete last field, others were already shifted
		for (ii=0; ii<BUS_MAXNODES; ii++) {
			if (psSched->asNodeList[ii].uAddress == 0) {
			    psSched->asNodeList[ii].uAddress = uNodeAddress;
			    psSched->asNodeList[ii].uErrCnt = 0;
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
BOOL BUS_bSchedulRemNode(sSched_t* psBus, uint8_t uNodeAddress)
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
    if (found) { // add node to discovery-list
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
 * Start sending of next token.
 */
static BOOL bSendNextTimeSlotToken(sBus_t* psBus, sSched_t* psSched, BOOL bDiscovery)
{
    sNodeInfo_t* node;

    if (TRUE != bDiscovery) node = &psSched->asNodeList[psSched->uCurrentNode];
    else 					node = &psSched->asDiscoveryList[psSched->uDiscoverNode];

    if (node->uAddress != 0) {
        psSched->auTokenMsg[1] = node->uAddress | 0x80;
        return BUS__bPhySend(&psBus->sPhy, psSched->auTokenMsg, BUS_TOKEN_MSG_LEN);
    }
    return FALSE;
}

/**
 * Count node errors.
 */
void vNodeError(sSched_t* psSched)
{
	if (psSched->bSchedDiscovery) return;
	if (psSched->asNodeList[psSched->uCurrentNode].uErrCnt < 255) {
	    psSched->asNodeList[psSched->uCurrentNode].uErrCnt++;
		// Remove node-address from scheduling-list after max number of missing answers.
		if(BUS_MAX_ANSWERFAILS < psSched->asNodeList[psSched->uCurrentNode].uErrCnt) {
			BUS_bSchedulRemNode(psSched, psSched->auTokenMsg[1] & 0x7F);
		}
	}
}

/**
 * Check if current node is the scheduler node.
 */
BOOL bCurrNodeIsMe(sBus_t* psBus, sSched_t* psSched)
{
    sNodeInfo_t* node;

    node = &psSched->asNodeList[psSched->uCurrentNode];

    if (node->uAddress == psBus->sCfg.uOwnNodeAddress) {
    	return TRUE;
    }
    return FALSE;
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize scheduler functions.
 *
 * @param[in] psBus		Handle of the bus.
 */
void BUS_vSchedulConfigure(sSched_t* psSched)
{
    uint8_t ii;

    psSched->uCurrentNode  = 0;
    psSched->uDiscoverNode = 0;
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        psSched->asNodeList[ii].uAddress = 0;
        psSched->asNodeList[ii].uErrCnt = 0;
        psSched->asDiscoveryList[ii].uAddress = ii+1;
    }
    psSched->auTokenMsg[0] = BUS_SYNCBYTE;
    psSched->auTokenMsg[1] = 0;
}

// --- Global functions --------------------------------------------------------

/**
 * Schedule nodes on bus.
 * 
 * @param[in] psBus
 * Handle of the bus.
 */
BOOL BUS_bScheduleAndGetMessage(sBus_t* psBus, sSched_t* psSched )
{
	static uint8_t 	schedloopcnt = BUS_DISCOVERYLOOPS;
    BOOL			rc 			 = FALSE;

    switch (psBus->eState) {

    case eBus_Idle:
        psSched->bSchedDiscovery = FALSE;
    	if(eMod_Discovery == psBus->eModuleState) psSched->bSchedDiscovery = TRUE;
    	else if ((psSched->uCurrentNode == BUS_MAXNODES) ||
                 (psSched->asNodeList[psSched->uCurrentNode].uAddress == 0)) { // if last node in active list is reached reset node-counter and switch to discovery-list
    	    psSched->uCurrentNode = 0;
    	    psSched->bSchedDiscovery = TRUE;
    	}

    	if((psSched->bSchedDiscovery) && ((psSched->uDiscoverNode == BUS_MAXNODES) || (psSched->asDiscoveryList[psSched->uDiscoverNode].uAddress == 0)))
		{ // if last node in discovery list is reached reset node-counter
    	    psSched->uDiscoverNode = 0;
    		if(schedloopcnt) --schedloopcnt;
    	}

        // send token
    	if (bSendNextTimeSlotToken(psBus, psSched, psSched->bSchedDiscovery)) {
    	    CLK_bTimerStart(&psSched->sNodeAnsTimeout, CLOCK_MS_2_TICKS(30));
            psBus->eState = eBus_SendingToken;
        } else LED_ERROR_ON;
        break;

    case eBus_SendingToken:
        // wait for finished token sending
        if (!BUS__bPhySending(&psBus->sPhy)) {
        	if (bCurrNodeIsMe(psBus, psSched)) {
        		// The scheduler cannot receive its own message,
        		// so we set the state to eBus_GotToken "manually".
        	    psBus->eState = eBus_GotToken;
        	}
        	else {
        		// Token has been sent, now wait for a message.
        	    psSched->bSchedWaitingForAnswer = TRUE;
                psBus->eState = eBus_ReceivingWait;
        	}
        }
        else {
        	if (CLK_bTimerIsElapsed(&psSched->sNodeAnsTimeout)) {
        		vNodeError(psSched);
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

    if (psSched->bSchedWaitingForAnswer) {
    	BOOL recEnd = FALSE;
        if (psBus->bSchedMsgReceived) {
			if (TRUE==psSched->bSchedDiscovery) {
				if(BUS_bSchedulAddNode(psSched, psSched->auTokenMsg[1] & 0x7F)) {
					--psSched->uDiscoverNode; // re-establish last used index, because addresses were shifted.
				}
				psSched->bSchedDiscovery = FALSE;
			}
            // TODO check node ID of received message and route eventually
        	//      to other line or net
			recEnd = TRUE;

        }
        else {
            if (CLK_bTimerIsElapsed(&psSched->sNodeAnsTimeout)) {
                vNodeError(psSched);
                // return to IDLE state
				recEnd = TRUE;
				if (0 == schedloopcnt) { // if schedulerloopcount has reached zero ...
					schedloopcnt = BUS_LOOPS_TILL_SLEEP;
					if (eMod_Discovery == psBus->eModuleState) {
					    psBus->eModuleState = eMod_Running;   // ... finish discovery
					} else {
					    psBus->eModuleState = eMod_Sleeping; // ... initiate sleep-mode
					}
				}
            }
        }
        if (recEnd) { // receiving finished
            psBus->bSchedMsgReceived = FALSE;
            psSched->bSchedWaitingForAnswer = FALSE;
            if(eBus_AckWaitReceiving != psBus->eState) psBus->eState = eBus_Idle;
            if(TRUE != psSched->bSchedDiscovery) 	++psSched->uCurrentNode;
			else 								    ++psSched->uDiscoverNode;
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
        if (BUS__bSendSleepCmd(psBus)) {
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
