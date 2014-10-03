/**
 * @addtogroup BUS
 * @addtogroup BUS_SCHEDUL
 * @brief Message scheduler of bus messages. Used by the master on the bus.
 *
 * @{
 * @file    bus_schedul.c
 * @brief   Bus message scheduler.
 * @author  Christian Verhalen, Robert Mueller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
# include <avr/io.h>
# include <avr/sleep.h>
# include <util/delay.h>
#endif

#include "bus.h"
#include "bus_scheduler.h"
#include "bus_intern.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Resets the error-counter for uNodeAddress.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @param[in] uNodeAddress  Node address.
 */
void sched_reset_node_error(sSched_t* psSched, uint8_t uNodeAddress)
{
    if (uNodeAddress == psSched->uErrorNode) {    // error-counter is counting for uNodeAddress
        psSched->uErrorNode  = 0;
        psSched->uErrorCount = 0;
    }
}


/**
 * Provides the next node-address to be scheduled.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 Next node-address.
 */
 uint8_t sched_get_next_node(sSched_t* psSched)
 {
     uint8_t nodebit, nodebyte, mask;
     uint8_t foundnode = 0,
             startnode = 0;
     BOOL    secondrun = FALSE;

     // loop until at least one present node or one missing node have been found.
     while (foundnode == 0) {
         // search first present node, beginning at last present node + 1
         startnode = psSched->uCurrentNode;
         do {
             // was last node a discovery node?
             if (!psSched->bSchedDiscovery && !secondrun) {
                 psSched->uCurrentNode++;
                 if (psSched->uCurrentNode > BUS_LASTNODE) {
                     psSched->uCurrentNode = BUS_FIRSTNODE;
                     break; // now a new node shall be discovered
                 }
             }
             // test if node is available
             nodebyte = psSched->asNodeList.uAddress[psSched->uCurrentNode / 8];
             nodebit  = psSched->uCurrentNode % 8;
             mask = (1<<nodebit);
             if ((nodebyte & mask) != 0) {
                 foundnode = psSched->uCurrentNode;
                 psSched->bSchedDiscovery = FALSE;
                 break;
             }
         } while (psSched->uCurrentNode != startnode);

         // search first missing node, beginning at last missing node + 1
         if (foundnode == 0) {
             startnode = psSched->uDiscoverNode;
             do {
                 psSched->uDiscoverNode++;
                 if (psSched->uDiscoverNode > BUS_LASTNODE) {
                     psSched->uDiscoverNode = BUS_FIRSTNODE;
                     if (psSched->uSleepLoopCnt) --psSched->uSleepLoopCnt;
                 }
                 // test if node is NOT available
                 nodebyte = psSched->asNodeList.uAddress[psSched->uDiscoverNode / 8];
                 nodebit  = psSched->uDiscoverNode % 8;
                 mask = (1<<nodebit);
                 if ((nodebyte & mask) == 0) {
                     foundnode = psSched->uDiscoverNode;
                     psSched->bSchedDiscovery = TRUE;
                     break;
                 }
             } while (psSched->uDiscoverNode != startnode);
         }
         secondrun = TRUE;
     }
     return foundnode;
}


/**
 * Add/Remove node to/from scheduler list.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @param[in] uNodeAddress  Node-address.
 * @param[in] bOnline       Node-state.
 */
void sched_set_node_state(sSched_t* psSched, uint8_t uNodeAddress, BOOL bOnline)
{
    uint8_t     *ubyte = &psSched->asNodeList.uAddress[uNodeAddress/8];

    if (bOnline) *ubyte |=  (1<<uNodeAddress%8);
    else         *ubyte &= ~(1<<uNodeAddress%8);
}


/**
 * Counts errors for one node till it gets reset or counter reaches max.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 * @param[in] uNodeAddress  Node-address.
 */
void sched_set_node_error(sSched_t* psSched, uint8_t uNodeAddress)
{
    if (0==psSched->uErrorNode) psSched->uErrorNode = uNodeAddress; // error-counter is free
    else if (uNodeAddress==psSched->uErrorNode) {    // error-counter is counting for uNodeAddress
        psSched->uErrorCount++;
        if (psSched->uErrorCount >= BUS_MAX_ANSWERFAILS) { // max errors counted ...
            sched_set_node_state(psSched, uNodeAddress, FALSE); // set node-address offline
            // If there is no other node online set current-node 0
            psSched->uErrorNode  = 0;
            psSched->uErrorCount = 0;
        }
    }
}


/**
 * Start sending of next token.
 *
 * @param[in] psBus         Handle of the bus-struct.
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 TRUE if token has been successfully sent, otherwise FALSE.
 */
BOOL sched_send_next_timeslot_token(sBus_t* psBus, sSched_t* psSched)
{
    int8_t node;

    node = sched_get_next_node(psSched);
    if (node == 0) {
        return FALSE;
    }
    psSched->auTokenMsg[1] = node | 0x80;

    return bus_phy_send(&psBus->sPhy, psSched->auTokenMsg, BUS_TOKEN_MSG_LEN);
}


/**
 * Check if current node is the scheduler node.
 *
 * @param[in] psBus         Handle of the bus-struct.
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 TRUE if token matches scheduler-address, otherwise FALSE.
 */
BOOL sched_current_node_is_me(sBus_t* psBus, sSched_t* psSched)
{
    uint8_t node;

    node = psSched->auTokenMsg[1] & 0x7F;

    if (node == psBus->sCfg.uOwnNodeAddress) return TRUE;
    return FALSE;
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize scheduler functions.
 *
 * @param[in] psSched       Handle of the scheduler-struct.
 */
void bus_scheduler_configure(sSched_t* psSched)
{
    uint8_t ii;

    psSched->uCurrentNode  = BUS_FIRSTNODE;
    psSched->uDiscoverNode = BUS_FIRSTNODE;
    for (ii=0; ii<BUS_LASTNODE/8; ii++) {
        psSched->asNodeList.uAddress[ii] = 0;
    }
    psSched->auTokenMsg[0] = BUS_SYNCBYTE;
    psSched->auTokenMsg[1] = 0;
    psSched->bSchedDiscovery = TRUE;
    psSched->bSchedWaitingForAnswer = FALSE;
    psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;
}

// --- Global functions --------------------------------------------------------

/**
 * Schedule nodes on bus.
 *
 * @param[in] psBus         Handle of the bus.
 * @param[in] psSched       Handle of the scheduler-struct.
 * @returns                 TRUE if message was received, otherwise FALSE.
 */
BOOL bus_schedule_and_get_message(sBus_t* psBus, sSched_t* psSched )
{
    BOOL msg_received = FALSE;

    switch (psBus->eState) {

    case eBus_Idle:
        // send token
        if (sched_send_next_timeslot_token(psBus, psSched)) {
            clk_timer_start(&psSched->sNodeAnsTimeout, CLOCK_MS_2_TICKS(50));
            psBus->eState = eBus_SendingToken;
        }
        break;

    case eBus_SendingToken:
        // wait for finished token sending
        if (!bus_phy_sending(&psBus->sPhy)) {
            if (sched_current_node_is_me(psBus, psSched)) {
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
        else if (clk_timer_is_elapsed(&psSched->sNodeAnsTimeout)) {
                sched_set_node_error(psSched, psSched->auTokenMsg[1] & 0x7F);
                // return to IDLE state
                psBus->eState = eBus_Idle;
                LED_ERROR_ON;
        }
        break;

    case eBus_ReceivingPassive:
        // any message on the bus resets counter
        if (eMod_Running == psBus->eModuleState) psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;
        break;

    default:
        break;
    }

    msg_received = bus_trp_send_and_receive(psBus);
    //if (msg_received) psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;

    if (psSched->bSchedWaitingForAnswer) {
        BOOL receive_end = FALSE;

        if (psBus->bSchedMsgReceived) {
            if (TRUE == psSched->bSchedDiscovery) {
                sched_set_node_state(psSched, psSched->auTokenMsg[1] & 0x7F, TRUE);
                psSched->bSchedDiscovery = FALSE;
            }
            else sched_reset_node_error(psSched, psSched->auTokenMsg[1] & 0x7F);
            receive_end = TRUE;
        }
        else {
            if (clk_timer_is_elapsed(&psSched->sNodeAnsTimeout)) {
                if (!psSched->bSchedDiscovery) {
                    sched_set_node_error(psSched, psSched->auTokenMsg[1] & 0x7F);
                }
                psSched->bSchedDiscovery = FALSE;
                // return to IDLE state
                receive_end = TRUE;
                if (0 == psSched->uSleepLoopCnt) { // if schedulerloopcount has reached zero ...
                    psSched->uSleepLoopCnt = BUS_LOOPS_TILL_SLEEP;
                    psBus->eModuleState = eMod_Sleeping; // ... initiate sleep-mode
                }
            }
        }
        if (receive_end) { // receiving finished
            psBus->bSchedMsgReceived = FALSE;
            psSched->bSchedWaitingForAnswer = FALSE;
            if (eBus_AckWaitReceiving != psBus->eState) psBus->eState = eBus_Idle;
        }
    }
    return msg_received;
}


/**
 * Check if schedule is in sleep mode and send sleep command to slaves.
 *
 * @param[in] psBus     Handle of the bus.
 */
void bus_schedule_check_and_set_sleep(sBus_t* psBus)
{
    if (eMod_Sleeping == psBus->eModuleState) {
        if (bus_send_sleepcmd(psBus)) {
            clk_control(FALSE); // disable clock-timer, otherwise
                                 // irq will cause immediate wakeup.

            // sleep till byte is received.
#ifdef PRJCONF_UC_AVR
            set_sleep_mode(SLEEP_MODE_IDLE);
            sleep_mode();

            _delay_ms(1); // wait for sys-clock becoming stable
#endif
            bus_flush_bus(psBus); // Clean bus-buffer
            psBus->eModuleState = eMod_Running;
            clk_control(TRUE); // enable clock-timer
        }
    }
}
/** @} */
